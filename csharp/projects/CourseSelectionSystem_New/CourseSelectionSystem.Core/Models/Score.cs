using System.ComponentModel.DataAnnotations;

namespace CourseSelectionSystem.Core.Models;

public class Score
{
    [Key]
    public required int ID { get; set; }

    public required string SNO { get; set; }

    public required string CNO { get; set; }

    public required int GRADE { get; set; }

    public required DateTime RECORDEDAT { get; set; }
}
