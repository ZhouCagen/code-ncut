using CourseSelectionSystem.Core.Models;
using CourseSelectionSystem.Core.Repositories;
using CourseSelectionSystem.Core.Services;
using Microsoft.AspNetCore.Mvc;

namespace CourseSelectionSystem.Web.Controllers;

public class CollegeAdminController : BaseController
{
    private readonly StudentRepository _studentRepository;
    private readonly TeacherRepository _teacherRepository;
    private readonly CourseRepository _courseRepository;
    private readonly LotteryService _lotteryService;
    private readonly MessageService _messageService;
    private readonly UserRepository _userRepository;

    public CollegeAdminController(
        StudentRepository studentRepository,
        TeacherRepository teacherRepository,
        CourseRepository courseRepository,
        LotteryService lotteryService,
        MessageService messageService,
        UserRepository userRepository
    )
    {
        _studentRepository = studentRepository;
        _teacherRepository = teacherRepository;
        _courseRepository = courseRepository;
        _lotteryService = lotteryService;
        _messageService = messageService;
        _userRepository = userRepository;
    }

    [HttpGet]
    public async Task<IActionResult> Index()
    {
        IActionResult? auth = RequireRole(UserRole.CollegeAdmin);

        if (auth != null)
        {
            return auth;
        }

        string collegeId = CurrentCollegeId!;
        string userId = CurrentUserId!;

        ViewBag.Students = await _studentRepository.GetStudentsByCollegeAsync(collegeId);
        ViewBag.Teachers = await _teacherRepository.GetTeachersByCollegeAsync(collegeId);
        ViewBag.Courses = await _courseRepository.GetCoursesByCollegeAsync(collegeId);
        ViewBag.Users = await _userRepository.GetUsersByCollegeAsync(collegeId);
        ViewBag.Messages = await _messageService.GetMessagesForUserAsync(userId);

        return View();
    }

    [HttpPost]
    public async Task<IActionResult> AddStudent(Student student)
    {
        IActionResult? auth = RequireRole(UserRole.CollegeAdmin);

        if (auth != null)
        {
            return auth;
        }

        student.COLLEGEID = CurrentCollegeId!;

        await _studentRepository.AddAsync(student);

        SetMessage("学生添加成功");

        return RedirectToAction("Index");
    }

    [HttpPost]
    public async Task<IActionResult> DeleteStudent(string sno)
    {
        IActionResult? auth = RequireRole(UserRole.CollegeAdmin);

        if (auth != null)
        {
            return auth;
        }

        await _studentRepository.DeleteAsync(sno);

        SetMessage("学生删除成功");

        return RedirectToAction("Index");
    }

    [HttpPost]
    public async Task<IActionResult> AddTeacher(Teacher teacher)
    {
        IActionResult? auth = RequireRole(UserRole.CollegeAdmin);

        if (auth != null)
        {
            return auth;
        }

        teacher.COLLEGEID = CurrentCollegeId!;

        await _teacherRepository.AddAsync(teacher);

        SetMessage("教师添加成功");

        return RedirectToAction("Index");
    }

    [HttpPost]
    public async Task<IActionResult> DeleteTeacher(string tno)
    {
        IActionResult? auth = RequireRole(UserRole.CollegeAdmin);

        if (auth != null)
        {
            return auth;
        }

        await _teacherRepository.DeleteAsync(tno);

        SetMessage("教师删除成功");

        return RedirectToAction("Index");
    }

    [HttpPost]
    public async Task<IActionResult> AddCourse(Course course)
    {
        IActionResult? auth = RequireRole(UserRole.CollegeAdmin);

        if (auth != null)
        {
            return auth;
        }

        course.COLLEGEID = CurrentCollegeId!;

        await _courseRepository.AddAsync(course);

        SetMessage("课程添加成功");

        return RedirectToAction("Index");
    }

    [HttpPost]
    public async Task<IActionResult> DeleteCourse(string cno)
    {
        IActionResult? auth = RequireRole(UserRole.CollegeAdmin);

        if (auth != null)
        {
            return auth;
        }

        await _courseRepository.DeleteAsync(cno);

        SetMessage("课程删除成功");

        return RedirectToAction("Index");
    }

    [HttpPost]
    public async Task<IActionResult> DrawLots(string cno)
    {
        IActionResult? auth = RequireRole(UserRole.CollegeAdmin);

        if (auth != null)
        {
            return auth;
        }

        var result = await _lotteryService.DrawLotsAsync(CurrentCollegeId!, cno);

        SetMessage(result);

        return RedirectToAction("Index");
    }

    [HttpPost]
    public async Task<IActionResult> SendMessage(string toUserId, string content)
    {
        IActionResult? auth = RequireRole(UserRole.CollegeAdmin);

        if (auth != null)
        {
            return auth;
        }

        var result = await _messageService.SendMessageAsync(CurrentUserId!, toUserId, content);

        SetMessage(result);

        return RedirectToAction("Index");
    }
}
