using CourseSelectionSystem.Core.Models;
using CourseSelectionSystem.Core.Repositories;
using CourseSelectionSystem.Core.Services;
using Microsoft.AspNetCore.Mvc;

namespace CourseSelectionSystem.Web.Controllers;

public class StudentController : BaseController
{
    private readonly StudentCourseService _studentCourseService;
    private readonly MessageService _messageService;
    private readonly UserRepository _userRepository;

    public StudentController(
        StudentCourseService studentCourseService,
        MessageService messageService,
        UserRepository userRepository
    )
    {
        _studentCourseService = studentCourseService;
        _messageService = messageService;
        _userRepository = userRepository;
    }

    [HttpGet]
    public async Task<IActionResult> Index(string? coursewareCno)
    {
        IActionResult? auth = RequireRole(UserRole.Student);

        if (auth != null)
        {
            return auth;
        }

        string sno = CurrentRelatedId!;
        string userId = CurrentUserId!;

        ViewBag.OpenCourses = await _studentCourseService.GetOpenCoursesAsync();
        ViewBag.MySelections = await _studentCourseService.GetMySelectionsAsync(sno);
        ViewBag.MyScores = await _studentCourseService.GetMyScoresAsync(sno);
        ViewBag.Messages = await _messageService.GetMessagesForUserAsync(userId);
        ViewBag.Users = await _userRepository.GetAllAsync();
        ViewBag.SelectedCoursewareCno = coursewareCno;

        if (!string.IsNullOrWhiteSpace(coursewareCno))
        {
            ViewBag.Coursewares = await _studentCourseService.GetCoursewaresAsync(coursewareCno);
        }

        return View();
    }

    [HttpPost]
    public async Task<IActionResult> SelectCourse(string cno)
    {
        IActionResult? auth = RequireRole(UserRole.Student);

        if (auth != null)
        {
            return auth;
        }

        var result = await _studentCourseService.SelectCourseAsync(CurrentRelatedId!, cno);

        SetMessage(result.Message, result.Success);

        return RedirectToAction("Index");
    }

    [HttpPost]
    public async Task<IActionResult> SendMessage(string toUserId, string content)
    {
        IActionResult? auth = RequireRole(UserRole.Student);

        if (auth != null)
        {
            return auth;
        }

        var result = await _messageService.SendMessageAsync(CurrentUserId!, toUserId, content);

        SetMessage(result);

        return RedirectToAction("Index");
    }

    [HttpPost]
    public async Task<IActionResult> MarkMessageRead(int messageId)
    {
        IActionResult? auth = RequireRole(UserRole.Student);

        if (auth != null)
        {
            return auth;
        }

        var result = await _messageService.MarkAsReadAsync(messageId);

        SetMessage(result);

        return RedirectToAction("Index");
    }
}
