namespace CourseSelectionSystem.Core.DTOs;

public class CourseSelectionCount
{
    public required string CNO { get; set; }

    public required string CNAME { get; set; }

    public int StudentCount { get; set; }
}
