using System.ComponentModel.DataAnnotations;

namespace CourseSelectionSystem.Core.Models;

public class Message
{
    [Key]
    public required int ID { get; set; }

    public required string FROMUSERID { get; set; }

    public required string TOUSERID { get; set; }

    public required string CONTENT { get; set; }

    public required DateTime CREATEDAT { get; set; }

    public bool ISREAD { get; set; }
}
