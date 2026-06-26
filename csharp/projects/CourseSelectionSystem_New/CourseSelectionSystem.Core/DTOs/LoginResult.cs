using CourseSelectionSystem.Core.Models;

namespace CourseSelectionSystem.Core.DTOs;

public class LoginResult
{
    public bool Success { get; set; }

    public required string Message { get; set; }

    public UserRole? Role { get; set; }

    public string? USERID { get; set; }

    public string? ACCOUNT { get; set; }

    public string? RELATEDID { get; set; }

    public string? COLLEGEID { get; set; }
}
