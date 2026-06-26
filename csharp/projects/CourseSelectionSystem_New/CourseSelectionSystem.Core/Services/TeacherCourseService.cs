using System.Data;
using CourseSelectionSystem.Core.DTOs;
using CourseSelectionSystem.Core.Models;
using CourseSelectionSystem.Core.Repositories;

namespace CourseSelectionSystem.Core.Services;

public class TeacherCourseService
{
    private readonly CourseRepository _courseRepository;
    private readonly CourseSelectionRepository _courseSelectionRepository;

    public TeacherCourseService(
        CourseRepository courseRepository,
        CourseSelectionRepository courseSelectionRepository
    )
    {
        _courseRepository = courseRepository;
        _courseSelectionRepository = courseSelectionRepository;
    }

    public async Task<List<Course>> GetMyCoursesAsync(string teacherId)
    {
        return await _courseRepository.GetCoursesByTeacherAsync(teacherId);
    }

    public async Task<ServiceResult> UpdateIntroductionAsync(
        string teacherId,
        string cno,
        string introduction
    )
    {
        var course = await _courseRepository.GetByIdAsync(cno);

        if (course == null)
        {
            return new ServiceResult { Success = false, Message = "课程不存在" };
        }

        if (course.TNO != teacherId)
        {
            return new ServiceResult { Success = false, Message = "不能修改其他教师的课程" };
        }

        await _courseRepository.UpdateCourseIntroductionAsync(cno, introduction);

        return new ServiceResult { Success = true, Message = "课程介绍更新成功" };
    }

    public async Task<ServiceResult> PublishCourseAsync(string teacherId, string cno)
    {
        return await UpdateCourseOpenStatusAsync(teacherId, cno, true, "课程发布成功");
    }

    public async Task<ServiceResult> CloseCourseAsync(string teacherId, string cno)
    {
        return await UpdateCourseOpenStatusAsync(teacherId, cno, false, "课程关闭成功");
    }

    public async Task<DataTable> GetWonStudentsAsync(string teacherId, string cno)
    {
        var course = await _courseRepository.GetByIdAsync(cno);

        if (course == null)
        {
            throw new InvalidOperationException("课程不存在");
        }

        if (course.TNO != teacherId)
        {
            throw new InvalidOperationException("不能查询其他教师的课程学生名单");
        }

        return await _courseSelectionRepository.GetWonStudentsByCourseAsync(cno);
    }

    private async Task<ServiceResult> UpdateCourseOpenStatusAsync(
        string teacherId,
        string cno,
        bool isOpen,
        string successMessage
    )
    {
        var course = await _courseRepository.GetByIdAsync(cno);

        if (course == null)
        {
            return new ServiceResult { Success = false, Message = "课程不存在" };
        }

        if (course.TNO != teacherId)
        {
            return new ServiceResult { Success = false, Message = "不能操作其他教师的课程" };
        }

        await _courseRepository.UpdateCourseOpenStatusAsync(cno, isOpen);

        return new ServiceResult { Success = true, Message = successMessage };
    }
}
