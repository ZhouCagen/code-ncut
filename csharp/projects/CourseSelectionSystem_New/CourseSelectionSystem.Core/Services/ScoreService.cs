using System.Data;
using CourseSelectionSystem.Core.DTOs;
using CourseSelectionSystem.Core.Repositories;

namespace CourseSelectionSystem.Core.Services;

public class ScoreService
{
    private readonly CourseRepository _courseRepository;
    private readonly CourseSelectionRepository _courseSelectionRepository;
    private readonly ScoreRepository _scoreRepository;

    public ScoreService(
        CourseRepository courseRepository,
        CourseSelectionRepository courseSelectionRepository,
        ScoreRepository scoreRepository
    )
    {
        _courseRepository = courseRepository;
        _courseSelectionRepository = courseSelectionRepository;
        _scoreRepository = scoreRepository;
    }

    public async Task<ServiceResult> RecordScoreAsync(
        string teacherId,
        string cno,
        string sno,
        int grade
    )
    {
        if (grade < 0 || grade > 100)
        {
            return new ServiceResult { Success = false, Message = "成绩必须在 0 到 100 之间" };
        }

        var course = await _courseRepository.GetByIdAsync(cno);

        if (course == null)
        {
            return new ServiceResult { Success = false, Message = "课程不存在" };
        }

        if (course.TNO != teacherId)
        {
            return new ServiceResult { Success = false, Message = "不能录入其他教师课程的成绩" };
        }

        DataTable wonStudents = await _courseSelectionRepository.GetWonStudentsByCourseAsync(cno);

        if (!ContainsStudent(wonStudents, sno))
        {
            return new ServiceResult
            {
                Success = false,
                Message = "该学生不是本课程中签学生，不能录入成绩",
            };
        }

        var oldScore = await _scoreRepository.GetScoreAsync(sno, cno);

        if (oldScore == null)
        {
            await _scoreRepository.InsertScoreAsync(sno, cno, grade);
        }
        else
        {
            await _scoreRepository.UpdateScoreAsync(sno, cno, grade);
        }

        return new ServiceResult { Success = true, Message = "成绩录入成功" };
    }

    public async Task<ScoreStatistics> GetScoreStatisticsAsync(string teacherId, string cno)
    {
        var course = await _courseRepository.GetByIdAsync(cno);

        if (course == null)
        {
            throw new InvalidOperationException("课程不存在");
        }

        if (course.TNO != teacherId)
        {
            throw new InvalidOperationException("不能统计其他教师课程的成绩");
        }

        DataTable scoreTable = await _scoreRepository.GetScoresByCourseAsync(cno);

        int excellentCount = 0;
        int goodCount = 0;
        int mediumCount = 0;
        int passCount = 0;
        int failCount = 0;

        foreach (DataRow row in scoreTable.Rows)
        {
            int grade = Convert.ToInt32(row["GRADE"]);

            if (grade >= 90)
            {
                excellentCount++;
            }
            else if (grade >= 80)
            {
                goodCount++;
            }
            else if (grade >= 70)
            {
                mediumCount++;
            }
            else if (grade >= 60)
            {
                passCount++;
            }
            else
            {
                failCount++;
            }
        }

        int totalCount = scoreTable.Rows.Count;
        int passedTotal = excellentCount + goodCount + mediumCount + passCount;

        decimal passRate =
            totalCount == 0 ? 0 : Math.Round((decimal)passedTotal * 100 / totalCount, 2);

        return new ScoreStatistics
        {
            CNO = course.CNO,
            CNAME = course.CNAME,
            TotalCount = totalCount,
            ExcellentCount = excellentCount,
            GoodCount = goodCount,
            MediumCount = mediumCount,
            PassCount = passCount,
            FailCount = failCount,
            PassRate = passRate,
        };
    }

    private static bool ContainsStudent(DataTable dataTable, string sno)
    {
        foreach (DataRow row in dataTable.Rows)
        {
            if (row["SNO"].ToString() == sno)
            {
                return true;
            }
        }

        return false;
    }
}
