using System.ComponentModel.DataAnnotations;

public class Student
{
    [Key]
    public required string SNO { get; set; }
    public required string SNAME { get; set; }
    public required string SSEX { get; set; }
    public required int SAGE { get; set; }
    public required string SDEPT { get; set; }
}