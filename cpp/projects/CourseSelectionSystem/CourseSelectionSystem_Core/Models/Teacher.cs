using System.ComponentModel.DataAnnotations;

public class Teacher
{
    [Key]
    public required string TNO { get; set; }
    public required string TNAME { get; set; }
    public required string TSEX { get; set; }
    public required string TDept { get; set; }
    public string? CNO { get; set; }
}