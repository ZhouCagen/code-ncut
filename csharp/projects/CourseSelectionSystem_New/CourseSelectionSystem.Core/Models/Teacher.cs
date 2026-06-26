using System.ComponentModel.DataAnnotations;

namespace CourseSelectionSystem.Core.Models;

public class Teacher
{
    [Key]
    public required string TNO { get; set; }

    public required string TNAME { get; set; }

    public required string TSEX { get; set; }

    public required string TDEPT { get; set; }

    public required string COLLEGEID { get; set; }
}
