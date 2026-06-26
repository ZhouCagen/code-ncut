using System.Data;
using CourseSelectionSystem.Core.Data;
using CourseSelectionSystem.Core.Models;
using Microsoft.Data.Sqlite;

namespace CourseSelectionSystem.Core.Repositories;

public class CoursewareRepository : BaseRepository<Courseware>
{
    public CoursewareRepository(Database database)
        : base(database, "FILENAME") { }

    public async Task<List<Courseware>> GetCoursewaresByCourseAsync(string cno)
    {
        string sql = """
            SELECT *
            FROM [Courseware]
            WHERE [CNO] = @Cno
            ORDER BY [UPLOADEDAT] DESC
            """;

        SqliteParameter[] parameters = [new("@Cno", cno)];

        DataTable dataTable = await _database.ExecuteQueryAsync(sql, parameters);

        return DataTableConvertToList(dataTable);
    }

    public async Task<DataTable> GetCoursewareDataAsync(string cno)
    {
        string sql = """
            SELECT
                CW.[ID],
                CW.[CNO],
                C.[CNAME],
                CW.[FILENAME],
                CW.[FILEPATH],
                CW.[UPLOADEDBY],
                CW.[UPLOADEDAT]
            FROM [Courseware] CW
            JOIN [Course] C ON CW.[CNO] = C.[CNO]
            WHERE CW.[CNO] = @Cno
            ORDER BY CW.[UPLOADEDAT] DESC
            """;

        SqliteParameter[] parameters = [new("@Cno", cno)];

        return await _database.ExecuteQueryAsync(sql, parameters);
    }
}
