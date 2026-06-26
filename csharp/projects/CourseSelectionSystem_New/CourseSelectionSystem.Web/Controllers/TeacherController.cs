using CourseSelectionSystem.Core.Models;
using CourseSelectionSystem.Core.Repositories;
using CourseSelectionSystem.Core.Services;
using Microsoft.AspNetCore.Mvc;

namespace CourseSelectionSystem.Web.Controllers;

public class TeacherController : BaseController
{
    private readonly TeacherCourseService _teacherCourseService;
    private readonly ScoreService _scoreService;
    private readonly CoursewareService _coursewareService;
    private readonly MessageService _messageService;
    private readonly UserRepository _userRepository;

    public TeacherController(
        TeacherCourseService teacherCourseService,
        ScoreService scoreService,
        CoursewareService coursewareService,
        MessageService messageService,
        UserRepository userRepository
    )
    {
        _teacherCourseService = teacherCourseService;
        _scoreService = scoreService;
        _coursewareService = coursewareService;
        _messageService = messageService;
        _userRepository = userRepository;
    }

    [HttpGet]
    public async Task<IActionResult> Index(string? cno)
    {
        IActionResult? auth = RequireRole(UserRole.Teacher);

        if (auth != null)
        {
            return auth;
        }

        string teacherId = CurrentRelatedId!;
        string userId = CurrentUserId!;

        var courses = await _teacherCourseService.GetMyCoursesAsync(teacherId);

        if (string.IsNullOrWhiteSpace(cno))
        {
            cno = courses.FirstOrDefault()?.CNO;
        }

        ViewBag.MyCourses = courses;
        ViewBag.SelectedCno = cno;
        ViewBag.Users = await _userRepository.GetAllAsync();
        ViewBag.Messages = await _messageService.GetMessagesForUserAsync(userId);

        if (!string.IsNullOrWhiteSpace(cno))
        {
            ViewBag.WonStudents = await _teacherCourseService.GetWonStudentsAsync(teacherId, cno);
            ViewBag.Coursewares = await _coursewareService.GetCoursewaresByCourseAsync(cno);
            ViewBag.ScoreStatistics = await _scoreService.GetScoreStatisticsAsync(teacherId, cno);
        }

        return View();
    }

    [HttpPost]
    public async Task<IActionResult> UpdateIntroduction(string cno, string introduction)
    {
        IActionResult? auth = RequireRole(UserRole.Teacher);

        if (auth != null)
        {
            return auth;
        }

        var result = await _teacherCourseService.UpdateIntroductionAsync(
            CurrentRelatedId!,
            cno,
            introduction
        );

        SetMessage(result);

        return RedirectToAction("Index", new { cno });
    }

    [HttpPost]
    public async Task<IActionResult> PublishCourse(string cno)
    {
        IActionResult? auth = RequireRole(UserRole.Teacher);

        if (auth != null)
        {
            return auth;
        }

        var result = await _teacherCourseService.PublishCourseAsync(CurrentRelatedId!, cno);

        SetMessage(result);

        return RedirectToAction("Index", new { cno });
    }

    [HttpPost]
    public async Task<IActionResult> CloseCourse(string cno)
    {
        IActionResult? auth = RequireRole(UserRole.Teacher);

        if (auth != null)
        {
            return auth;
        }

        var result = await _teacherCourseService.CloseCourseAsync(CurrentRelatedId!, cno);

        SetMessage(result);

        return RedirectToAction("Index", new { cno });
    }

    [HttpPost]
    public async Task<IActionResult> RecordScore(string cno, string sno, int grade)
    {
        IActionResult? auth = RequireRole(UserRole.Teacher);

        if (auth != null)
        {
            return auth;
        }

        var result = await _scoreService.RecordScoreAsync(CurrentRelatedId!, cno, sno, grade);

        SetMessage(result);

        return RedirectToAction("Index", new { cno });
    }

    [HttpPost]
    public async Task<IActionResult> AddCourseware(string cno, string fileName, string filePath)
    {
        IActionResult? auth = RequireRole(UserRole.Teacher);

        if (auth != null)
        {
            return auth;
        }

        var result = await _coursewareService.AddCoursewareAsync(
            CurrentRelatedId!,
            cno,
            fileName,
            filePath
        );

        SetMessage(result);

        return RedirectToAction("Index", new { cno });
    }

    [HttpPost]
    public async Task<IActionResult> SendMessage(string toUserId, string content, string? cno)
    {
        IActionResult? auth = RequireRole(UserRole.Teacher);

        if (auth != null)
        {
            return auth;
        }

        var result = await _messageService.SendMessageAsync(CurrentUserId!, toUserId, content);

        SetMessage(result);

        return RedirectToAction("Index", new { cno });
    }
}
