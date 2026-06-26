using CourseSelectionSystem.Core.Data;
using CourseSelectionSystem.Core.Models;
using CourseSelectionSystem.Core.Repositories;
using CourseSelectionSystem.Core.Services;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Data.Sqlite;

namespace CourseSelectionSystem.Web.Controllers;

public class SystemAdminController : BaseController
{
    private readonly Database _database;
    private readonly UserRepository _userRepository;
    private readonly StudentRepository _studentRepository;
    private readonly TeacherRepository _teacherRepository;
    private readonly CourseRepository _courseRepository;

    public SystemAdminController(
        Database database,
        UserRepository userRepository,
        StudentRepository studentRepository,
        TeacherRepository teacherRepository,
        CourseRepository courseRepository
    )
    {
        _database = database;
        _userRepository = userRepository;
        _studentRepository = studentRepository;
        _teacherRepository = teacherRepository;
        _courseRepository = courseRepository;
    }

    [HttpGet]
    public async Task<IActionResult> Index()
    {
        IActionResult? auth = RequireRole(UserRole.SystemAdmin);

        if (auth != null)
        {
            return auth;
        }

        ViewBag.Colleges = await _database.ExecuteQueryAsync(
            "SELECT * FROM [College] ORDER BY [COLLEGEID] ASC"
        );

        ViewBag.Users = await _userRepository.GetAllAsync();
        ViewBag.Students = await _studentRepository.GetAllAsync();
        ViewBag.Teachers = await _teacherRepository.GetAllAsync();
        ViewBag.Courses = await _courseRepository.GetAllAsync();

        return View();
    }

    [HttpPost]
    public async Task<IActionResult> AddCollege(string collegeId, string collegeName)
    {
        IActionResult? auth = RequireRole(UserRole.SystemAdmin);

        if (auth != null)
        {
            return auth;
        }

        string sql = """
            INSERT INTO [College] ([COLLEGEID], [COLLEGENAME])
            VALUES (@CollegeId, @CollegeName)
            """;

        await _database.ExecuteNonQueryAsync(
            sql,
            new SqliteParameter("@CollegeId", collegeId),
            new SqliteParameter("@CollegeName", collegeName)
        );

        SetMessage("学院添加成功");

        return RedirectToAction("Index");
    }

    [HttpPost]
    public async Task<IActionResult> DeleteCollege(string collegeId)
    {
        IActionResult? auth = RequireRole(UserRole.SystemAdmin);

        if (auth != null)
        {
            return auth;
        }

        await _database.ExecuteNonQueryAsync(
            "DELETE FROM [College] WHERE [COLLEGEID] = @CollegeId",
            new SqliteParameter("@CollegeId", collegeId)
        );

        SetMessage("学院删除成功");

        return RedirectToAction("Index");
    }

    [HttpPost]
    public async Task<IActionResult> AddUser(
        string userId,
        string account,
        string password,
        UserRole role,
        string? relatedId,
        string? collegeId
    )
    {
        IActionResult? auth = RequireRole(UserRole.SystemAdmin);

        if (auth != null)
        {
            return auth;
        }

        var user = new User
        {
            USERID = userId,
            ACCOUNT = account,
            PASSWORDHASH = PasswordHasher.HashPassword(password),
            ROLE = role,
            RELATEDID = relatedId,
            COLLEGEID = collegeId,
        };

        await _userRepository.AddAsync(user);

        SetMessage("账号添加成功");

        return RedirectToAction("Index");
    }

    [HttpPost]
    public async Task<IActionResult> DeleteUser(string userId)
    {
        IActionResult? auth = RequireRole(UserRole.SystemAdmin);

        if (auth != null)
        {
            return auth;
        }

        await _userRepository.DeleteAsync(userId);

        SetMessage("账号删除成功");

        return RedirectToAction("Index");
    }
}
