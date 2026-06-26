using System.ComponentModel.DataAnnotations;

namespace CourseSelectionSystem.Core.Models;

public class Course
{
    [Key]
    public required string CNO { get; set; }

    public required string CNAME { get; set; }

    public required int CTIME { get; set; }

    public required decimal CCREDIT { get; set; }

    public required string TNO { get; set; }

    public required string COLLEGEID { get; set; }

    public string? INTRODUCTION { get; set; }

    public int CAPACITY { get; set; }

    public bool ISOPEN { get; set; }
}
