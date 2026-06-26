using System.ComponentModel.DataAnnotations;

namespace CourseSelectionSystem.Core.Models;

public class CoursePrerequisite
{
    [Key]
    public required int ID { get; set; }

    public required string CNO { get; set; }

    public required string PREREQUISITECNO { get; set; }
}
