using CourseSelectionSystem.Core.Models;

namespace CourseSelectionSystem.Core.DTOs;

public class CourseStudentResult
{
    public required string CNO { get; set; }

    public required string CNAME { get; set; }

    public required string SNO { get; set; }

    public required string SNAME { get; set; }

    public required string SSEX { get; set; }

    public required string SDEPT { get; set; }

    public SelectionStatus STATUS { get; set; }
}
