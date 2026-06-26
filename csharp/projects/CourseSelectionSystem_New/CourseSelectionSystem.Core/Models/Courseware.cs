using System.ComponentModel.DataAnnotations;

namespace CourseSelectionSystem.Core.Models;

public class Courseware
{
    [Key]
    public required int ID { get; set; }

    public required string CNO { get; set; }

    public required string FILENAME { get; set; }

    public required string FILEPATH { get; set; }

    public required string UPLOADEDBY { get; set; }

    public required DateTime UPLOADEDAT { get; set; }
}
