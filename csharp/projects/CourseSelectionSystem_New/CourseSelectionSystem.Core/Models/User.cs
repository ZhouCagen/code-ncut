using System.ComponentModel.DataAnnotations;

namespace CourseSelectionSystem.Core.Models;

public enum UserRole
{
    SystemAdmin = 1,
    CollegeAdmin = 2,
    Teacher = 3,
    Student = 4,
}

public class User
{
    [Key]
    public required string USERID { get; set; }

    public required string ACCOUNT { get; set; }

    public required string PASSWORDHASH { get; set; }

    public required UserRole ROLE { get; set; }

    public string? RELATEDID { get; set; }

    public string? COLLEGEID { get; set; }
}
