using System.ComponentModel.DataAnnotations;

namespace CourseSelectionSystem.Core.Models;

public class College
{
    [Key]
    public required string COLLEGEID { get; set; }

    public required string COLLEGENAME { get; set; }
}
