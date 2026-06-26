using System.ComponentModel.DataAnnotations;

namespace CourseSelectionSystem.Core.Models;

public enum SelectionStatus
{
    Pending = 0,
    Won = 1,
    Lost = 2,
    Dropped = 3,
}

public class CourseSelection
{
    [Key]
    public required int ID { get; set; }

    public required string SNO { get; set; }

    public required string CNO { get; set; }

    public required SelectionStatus STATUS { get; set; }

    public required DateTime SELECTEDAT { get; set; }
}
