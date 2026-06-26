using System.Data;
using CourseSelectionSystem.Core.DTOs;
using CourseSelectionSystem.Core.Models;
using CourseSelectionSystem.Core.Repositories;

namespace CourseSelectionSystem.Core.Services;

public class StudentCourseService
{
    private readonly StudentRepository _studentRepository;
    private readonly CourseRepository _courseRepository;
    private readonly CoursePrerequisiteRepository _coursePrerequisiteRepository;
    private readonly CourseSelectionRepository _courseSelectionRepository;
    private readonly ScoreRepository _scoreRepository;
    private readonly CoursewareRepository _coursewareRepository;

    public StudentCourseService(
        StudentRepository studentRepository,
        CourseRepository courseRepository,
        CoursePrerequisiteRepository coursePrerequisiteRepository,
        CourseSelectionRepository courseSelectionRepository,
        ScoreRepository scoreRepository,
        CoursewareRepository coursewareRepository
    )
    {
        _studentRepository = studentRepository;
        _courseRepository = courseRepository;
        _coursePrerequisiteRepository = coursePrerequisiteRepository;
        _courseSelectionRepository = courseSelectionRepository;
        _scoreRepository = scoreRepository;
        _coursewareRepository = coursewareRepository;
    }

    public async Task<List<Course>> GetOpenCoursesAsync()
    {
        return await _courseRepository.GetOpenCoursesAsync();
    }

    public async Task<DataTable> GetMySelectionsAsync(string sno)
    {
        return await _courseSelectionRepository.GetStudentSelectionsAsync(sno);
    }

    public async Task<DataTable> GetMyScoresAsync(string sno)
    {
        return await _scoreRepository.GetMyScoresAsync(sno);
    }

    public async Task<List<Courseware>> GetCoursewaresAsync(string cno)
    {
        return await _coursewareRepository.GetCoursewaresByCourseAsync(cno);
    }

    public async Task<CourseSelectionResult> SelectCourseAsync(string sno, string cno)
    {
        var student = await _studentRepository.GetByIdAsync(sno);

        if (student == null)
        {
            return new CourseSelectionResult
            {
                Success = false,
                Message = "学生不存在",
                SNO = sno,
                CNO = cno,
            };
        }

        var course = await _courseRepository.GetByIdAsync(cno);

        if (course == null)
        {
            return new CourseSelectionResult
            {
                Success = false,
                Message = "课程不存在",
                SNO = sno,
                CNO = cno,
            };
        }

        if (!course.ISOPEN)
        {
            return new CourseSelectionResult
            {
                Success = false,
                Message = "该课程暂未开放选课",
                SNO = sno,
                CNO = cno,
            };
        }

        bool alreadySelected = await _courseSelectionRepository.HasSelectedCourseAsync(sno, cno);

        if (alreadySelected)
        {
            return new CourseSelectionResult
            {
                Success = false,
                Message = "您已经选择过该课程",
                SNO = sno,
                CNO = cno,
            };
        }

        bool hasPrerequisite = await HasFinishedPrerequisitesAsync(sno, cno);

        if (!hasPrerequisite)
        {
            return new CourseSelectionResult
            {
                Success = false,
                Message = "您没有学习该课程的先修课程，不能进行选课！",
                SNO = sno,
                CNO = cno,
            };
        }

        await _courseSelectionRepository.SelectCourseAsync(sno, cno);

        return new CourseSelectionResult
        {
            Success = true,
            Message = "选课申请成功，等待学院抽签",
            SNO = sno,
            CNO = cno,
        };
    }

    private async Task<bool> HasFinishedPrerequisitesAsync(string sno, string cno)
    {
        var prerequisites = await _coursePrerequisiteRepository.GetPrerequisitesAsync(cno);

        foreach (var prerequisite in prerequisites)
        {
            bool passed = await _scoreRepository.HasPassedCourseAsync(
                sno,
                prerequisite.PREREQUISITECNO
            );

            if (!passed)
            {
                return false;
            }
        }

        return true;
    }
}
