using CourseSelectionSystem.Core.DTOs;
using CourseSelectionSystem.Core.Models;
using CourseSelectionSystem.Core.Repositories;

namespace CourseSelectionSystem.Core.Services;

public class CoursewareService
{
    private readonly CourseRepository _courseRepository;
    private readonly CoursewareRepository _coursewareRepository;

    public CoursewareService(
        CourseRepository courseRepository,
        CoursewareRepository coursewareRepository
    )
    {
        _courseRepository = courseRepository;
        _coursewareRepository = coursewareRepository;
    }

    public async Task<List<Courseware>> GetCoursewaresByCourseAsync(string cno)
    {
        return await _coursewareRepository.GetCoursewaresByCourseAsync(cno);
    }

    public async Task<ServiceResult> AddCoursewareAsync(
        string teacherId,
        string cno,
        string fileName,
        string filePath
    )
    {
        var course = await _courseRepository.GetByIdAsync(cno);

        if (course == null)
        {
            return new ServiceResult { Success = false, Message = "课程不存在" };
        }

        if (course.TNO != teacherId)
        {
            return new ServiceResult { Success = false, Message = "不能给其他教师的课程上传课件" };
        }

        var courseware = new Courseware
        {
            ID = 0,
            CNO = cno,
            FILENAME = fileName,
            FILEPATH = filePath,
            UPLOADEDBY = teacherId,
            UPLOADEDAT = DateTime.Now,
        };

        await _coursewareRepository.AddAsync(courseware);

        return new ServiceResult { Success = true, Message = "课件上传成功" };
    }
}
