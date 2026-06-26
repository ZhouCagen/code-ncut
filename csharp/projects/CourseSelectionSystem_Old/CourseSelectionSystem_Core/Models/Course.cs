using System.ComponentModel.DataAnnotations;

public class Course
{
    [Key]
    public required string CNO { get; set; }
    public required string CNAME{ get; set; }
    public required int CTIME { get; set; }
    public required decimal CCREDIT { get; set; }
}