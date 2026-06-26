using System.Data;
using CourseSelectionSystem.Core.Data;
using CourseSelectionSystem.Core.Models;
using Microsoft.Data.Sqlite;

namespace CourseSelectionSystem.Core.Repositories;

public class CourseSelectionRepository : BaseRepository<CourseSelection>
{
    public CourseSelectionRepository(Database database)
        : base(database) { }

    private static SqliteParameter IntParameter(string name, int value)
    {
        return new SqliteParameter(name, SqliteType.Integer) { Value = value };
    }

    public async Task<bool> HasSelectedCourseAsync(string sno, string cno)
    {
        string sql = """
            SELECT COUNT(*)
            FROM [CourseSelection]
            WHERE [SNO] = @Sno
              AND [CNO] = @Cno
              AND [STATUS] <> @Dropped
            """;

        SqliteParameter[] parameters =
        [
            new("@Sno", sno),
            new("@Cno", cno),
            IntParameter("@Dropped", (int)SelectionStatus.Dropped),
        ];

        object? result = await _database.ExecuteScalarAsync(sql, parameters);

        return Convert.ToInt32(result) > 0;
    }

    public async Task<int> SelectCourseAsync(string sno, string cno)
    {
        string sql = """
            INSERT INTO [CourseSelection]
            (
                [SNO],
                [CNO],
                [STATUS],
                [SELECTEDAT]
            )
            VALUES
            (
                @Sno,
                @Cno,
                @Status,
                @SelectedAt
            )
            """;

        SqliteParameter[] parameters =
        [
            new("@Sno", sno),
            new("@Cno", cno),
            IntParameter("@Status", (int)SelectionStatus.Pending),
            new("@SelectedAt", DateTime.Now),
        ];

        return await _database.ExecuteNonQueryAsync(sql, parameters);
    }

    public async Task<List<CourseSelection>> GetPendingSelectionsAsync(string cno)
    {
        string sql = """
            SELECT *
            FROM [CourseSelection]
            WHERE [CNO] = @Cno
              AND [STATUS] = @Status
            ORDER BY [SELECTEDAT] ASC
            """;

        SqliteParameter[] parameters =
        [
            new("@Cno", cno),
            IntParameter("@Status", (int)SelectionStatus.Pending),
        ];

        DataTable dataTable = await _database.ExecuteQueryAsync(sql, parameters);

        return DataTableConvertToList(dataTable);
    }

    public async Task<int> UpdateStatusAsync(int id, SelectionStatus status)
    {
        string sql = """
            UPDATE [CourseSelection]
            SET [STATUS] = @Status
            WHERE [ID] = @Id
            """;

        SqliteParameter[] parameters = [IntParameter("@Status", (int)status), new("@Id", id)];

        return await _database.ExecuteNonQueryAsync(sql, parameters);
    }

    public async Task<DataTable> GetStudentSelectionsAsync(string sno)
    {
        string sql = """
            SELECT
                CS.[ID],
                CS.[SNO],
                CS.[CNO],
                C.[CNAME],
                C.[CCREDIT],
                C.[CTIME],
                CS.[STATUS],
                CS.[SELECTEDAT]
            FROM [CourseSelection] CS
            JOIN [Course] C ON CS.[CNO] = C.[CNO]
            WHERE CS.[SNO] = @Sno
            ORDER BY CS.[SELECTEDAT] DESC
            """;

        SqliteParameter[] parameters = [new("@Sno", sno)];

        return await _database.ExecuteQueryAsync(sql, parameters);
    }

    public async Task<DataTable> GetWonStudentsByCourseAsync(string cno)
    {
        string sql = """
            SELECT
                S.[SNO],
                S.[SNAME],
                S.[SSEX],
                S.[SDEPT],
                C.[CNO],
                C.[CNAME]
            FROM [CourseSelection] CS
            JOIN [Student] S ON CS.[SNO] = S.[SNO]
            JOIN [Course] C ON CS.[CNO] = C.[CNO]
            WHERE CS.[CNO] = @Cno
              AND CS.[STATUS] = @Status
            ORDER BY S.[SNO] ASC
            """;

        SqliteParameter[] parameters =
        [
            new("@Cno", cno),
            IntParameter("@Status", (int)SelectionStatus.Won),
        ];

        return await _database.ExecuteQueryAsync(sql, parameters);
    }

    public async Task<DataTable> GetCourseSelectionCountAsync()
    {
        string sql = """
            SELECT
                C.[CNO],
                C.[CNAME],
                COUNT(CS.[SNO]) AS [StudentCount]
            FROM [Course] C
            LEFT JOIN [CourseSelection] CS ON C.[CNO] = CS.[CNO]
            GROUP BY C.[CNO], C.[CNAME]
            ORDER BY [StudentCount] DESC
            """;

        return await _database.ExecuteQueryAsync(sql);
    }
}
