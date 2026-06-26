using CourseSelectionSystem.Core.DTOs;
using CourseSelectionSystem.Core.Models;
using CourseSelectionSystem.Core.Services;
using Microsoft.AspNetCore.Mvc;

namespace CourseSelectionSystem.Web.Controllers;

public class AccountController : Controller
{
    private readonly AuthService _authService;

    public AccountController(AuthService authService)
    {
        _authService = authService;
    }

    [HttpGet]
    public IActionResult Login()
    {
        return View();
    }

    [HttpPost]
    public async Task<IActionResult> Login(string account, string password)
    {
        LoginResult result = await _authService.LoginAsync(account, password);

        if (!result.Success || result.Role == null)
        {
            ViewBag.Error = result.Message;
            return View();
        }

        HttpContext.Session.SetString("USERID", result.USERID ?? "");
        HttpContext.Session.SetString("ACCOUNT", result.ACCOUNT ?? "");
        HttpContext.Session.SetString("ROLE", result.Role.Value.ToString());
        HttpContext.Session.SetString("RELATEDID", result.RELATEDID ?? "");
        HttpContext.Session.SetString("COLLEGEID", result.COLLEGEID ?? "");

        return result.Role.Value switch
        {
            UserRole.Student => RedirectToAction("Index", "Student"),
            UserRole.Teacher => RedirectToAction("Index", "Teacher"),
            UserRole.CollegeAdmin => RedirectToAction("Index", "CollegeAdmin"),
            UserRole.SystemAdmin => RedirectToAction("Index", "SystemAdmin"),
            _ => RedirectToAction("Login"),
        };
    }

    [HttpGet]
    public IActionResult Logout()
    {
        HttpContext.Session.Clear();

        return RedirectToAction("Login");
    }
}
