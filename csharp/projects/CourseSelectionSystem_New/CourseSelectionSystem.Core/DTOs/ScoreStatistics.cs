namespace CourseSelectionSystem.Core.DTOs;

public class ScoreStatistics
{
    public required string CNO { get; set; }

    public required string CNAME { get; set; }

    public int TotalCount { get; set; }

    public int ExcellentCount { get; set; }

    public int GoodCount { get; set; }

    public int MediumCount { get; set; }

    public int PassCount { get; set; }

    public int FailCount { get; set; }

    public decimal PassRate { get; set; }
}
