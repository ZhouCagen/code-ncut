using System.ComponentModel.DataAnnotations;

namespace CourseSelectionSystem.Web.Models;

public class LoginViewModel
{
    [Required(ErrorMessage = "请输入账号")]
    public string Account { get; set; } = string.Empty;

    [Required(ErrorMessage = "请输入密码")]
    public string Password { get; set; } = string.Empty;

    public string? ErrorMessage { get; set; }
}
