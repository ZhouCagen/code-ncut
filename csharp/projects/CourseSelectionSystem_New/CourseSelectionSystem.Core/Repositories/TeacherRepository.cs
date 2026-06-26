using System.Data;
using CourseSelectionSystem.Core.Data;
using CourseSelectionSystem.Core.Models;
using Microsoft.Data.Sqlite;

namespace CourseSelectionSystem.Core.Repositories;

public class TeacherRepository : BaseRepository<Teacher>
{
    public TeacherRepository(Database database)
        : base(database, "TNAME") { }

    public async Task<List<Teacher>> GetTeachersByCollegeAsync(string collegeId)
    {
        string sql = """
            SELECT *
            FROM [Teacher]
            WHERE [COLLEGEID] = @CollegeId
            ORDER BY [TNO] ASC
            """;

        SqliteParameter[] parameters = [new("@CollegeId", collegeId)];

        DataTable dataTable = await _database.ExecuteQueryAsync(sql, parameters);

        return DataTableConvertToList(dataTable);
    }

    public async Task<DataTable> GetTeachersByCourseAsync(string courseName)
    {
        string sql = """
            SELECT
                T.[TNO],
                T.[TNAME],
                T.[TDEPT],
                C.[CNO],
                C.[CNAME]
            FROM [Teacher] T
            JOIN [Course] C ON T.[TNO] = C.[TNO]
            WHERE C.[CNAME] = @CourseName
            """;

        SqliteParameter[] parameters = [new("@CourseName", courseName)];

        return await _database.ExecuteQueryAsync(sql, parameters);
    }

    public async Task<DataTable> GetPopularTeachersAsync()
    {
        string sql = """
            SELECT
                T.[TNO],
                T.[TNAME],
                COUNT(CS.[SNO]) AS [StudentCount]
            FROM [Teacher] T
            JOIN [Course] C ON T.[TNO] = C.[TNO]
            JOIN [CourseSelection] CS ON C.[CNO] = CS.[CNO]
            WHERE CS.[STATUS] = 1
            GROUP BY T.[TNO], T.[TNAME]
            ORDER BY [StudentCount] DESC
            """;

        return await _database.ExecuteQueryAsync(sql);
    }
}
