using CourseSelectionSystem.Core.DTOs;
using CourseSelectionSystem.Core.Models;
using Microsoft.AspNetCore.Mvc;

namespace CourseSelectionSystem.Web.Controllers;

public abstract class BaseController : Controller
{
    protected string? CurrentUserId => HttpContext.Session.GetString("USERID");

    protected string? CurrentAccount => HttpContext.Session.GetString("ACCOUNT");

    protected string? CurrentRelatedId => HttpContext.Session.GetString("RELATEDID");

    protected string? CurrentCollegeId => HttpContext.Session.GetString("COLLEGEID");

    protected UserRole? CurrentRole
    {
        get
        {
            string? value = HttpContext.Session.GetString("ROLE");

            if (Enum.TryParse(value, out UserRole role))
            {
                return role;
            }

            return null;
        }
    }

    protected IActionResult? RequireLogin()
    {
        if (string.IsNullOrWhiteSpace(CurrentUserId))
        {
            return RedirectToAction("Login", "Account");
        }

        return null;
    }

    protected IActionResult? RequireRole(UserRole role)
    {
        IActionResult? loginResult = RequireLogin();

        if (loginResult != null)
        {
            return loginResult;
        }

        if (CurrentRole != role)
        {
            return RedirectToAction("Login", "Account");
        }

        return null;
    }

    protected void SetMessage(ServiceResult result)
    {
        TempData["Message"] = result.Message;
        TempData["Success"] = result.Success ? "true" : "false";
    }

    protected void SetMessage(string message, bool success = true)
    {
        TempData["Message"] = message;
        TempData["Success"] = success ? "true" : "false";
    }
}
