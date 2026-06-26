using CourseSelectionSystem.Core.DTOs;
using CourseSelectionSystem.Core.Models;
using CourseSelectionSystem.Core.Repositories;

namespace CourseSelectionSystem.Core.Services;

public class LotteryService
{
    private readonly CourseRepository _courseRepository;
    private readonly CourseSelectionRepository _courseSelectionRepository;

    public LotteryService(
        CourseRepository courseRepository,
        CourseSelectionRepository courseSelectionRepository
    )
    {
        _courseRepository = courseRepository;
        _courseSelectionRepository = courseSelectionRepository;
    }

    public async Task<ServiceResult> DrawLotsAsync(string collegeId, string cno)
    {
        var course = await _courseRepository.GetByIdAsync(cno);

        if (course == null)
        {
            return new ServiceResult { Success = false, Message = "课程不存在" };
        }

        if (course.COLLEGEID != collegeId)
        {
            return new ServiceResult { Success = false, Message = "不能对其他学院课程进行抽签" };
        }

        if (course.CAPACITY <= 0)
        {
            return new ServiceResult { Success = false, Message = "课程容量必须大于 0" };
        }

        var pendingSelections = await _courseSelectionRepository.GetPendingSelectionsAsync(cno);

        if (pendingSelections.Count == 0)
        {
            return new ServiceResult { Success = false, Message = "当前课程没有待抽签学生" };
        }

        var shuffledSelections = pendingSelections.OrderBy(_ => Guid.NewGuid()).ToList();

        var winnerIds = shuffledSelections
            .Take(course.CAPACITY)
            .Select(selection => selection.ID)
            .ToHashSet();

        foreach (var selection in pendingSelections)
        {
            SelectionStatus status = winnerIds.Contains(selection.ID)
                ? SelectionStatus.Won
                : SelectionStatus.Lost;

            await _courseSelectionRepository.UpdateStatusAsync(selection.ID, status);
        }

        return new ServiceResult { Success = true, Message = "抽签完成" };
    }
}
