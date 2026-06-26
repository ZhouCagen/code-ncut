using System.Data;
using CourseSelectionSystem.Core.Data;
using CourseSelectionSystem.Core.Models;
using Microsoft.Data.Sqlite;

namespace CourseSelectionSystem.Core.Repositories;

public class CoursePrerequisiteRepository : BaseRepository<CoursePrerequisite>
{
    public CoursePrerequisiteRepository(Database database)
        : base(database) { }

    public async Task<List<CoursePrerequisite>> GetPrerequisitesAsync(string cno)
    {
        string sql = """
            SELECT *
            FROM [CoursePrerequisite]
            WHERE [CNO] = @Cno
            """;

        SqliteParameter[] parameters = [new("@Cno", cno)];

        DataTable dataTable = await _database.ExecuteQueryAsync(sql, parameters);

        return DataTableConvertToList(dataTable);
    }

    public async Task<DataTable> GetPrerequisiteCourseInfoAsync(string cno)
    {
        string sql = """
            SELECT
                CP.[CNO],
                C.[CNAME],
                CP.[PREREQUISITECNO],
                PC.[CNAME] AS [PrerequisiteCourseName]
            FROM [CoursePrerequisite] CP
            JOIN [Course] C ON CP.[CNO] = C.[CNO]
            JOIN [Course] PC ON CP.[PREREQUISITECNO] = PC.[CNO]
            WHERE CP.[CNO] = @Cno
            """;

        SqliteParameter[] parameters = [new("@Cno", cno)];

        return await _database.ExecuteQueryAsync(sql, parameters);
    }
}
