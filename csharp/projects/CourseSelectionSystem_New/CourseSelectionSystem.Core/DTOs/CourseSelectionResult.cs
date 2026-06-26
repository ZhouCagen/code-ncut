namespace CourseSelectionSystem.Core.DTOs;

public class CourseSelectionResult
{
    public bool Success { get; set; }

    public required string Message { get; set; }

    public string? SNO { get; set; }

    public string? CNO { get; set; }
}
