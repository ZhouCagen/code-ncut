using System.Data;
using CourseSelectionSystem.Core.Data;
using CourseSelectionSystem.Core.Models;
using Microsoft.Data.Sqlite;

namespace CourseSelectionSystem.Core.Repositories;

public class ScoreRepository : BaseRepository<Score>
{
    public ScoreRepository(Database database)
        : base(database) { }

    public async Task<bool> HasPassedCourseAsync(string sno, string cno)
    {
        string sql = """
            SELECT COUNT(*)
            FROM [Score]
            WHERE [SNO] = @Sno
              AND [CNO] = @Cno
              AND [GRADE] >= 60
            """;

        SqliteParameter[] parameters = [new("@Sno", sno), new("@Cno", cno)];

        object? result = await _database.ExecuteScalarAsync(sql, parameters);

        return Convert.ToInt32(result) > 0;
    }

    public async Task<DataTable> GetMyScoresAsync(string sno)
    {
        string sql = """
            SELECT
                S.[SNO],
                S.[CNO],
                C.[CNAME],
                C.[CCREDIT],
                S.[GRADE],
                S.[RECORDEDAT]
            FROM [Score] S
            JOIN [Course] C ON S.[CNO] = C.[CNO]
            WHERE S.[SNO] = @Sno
            ORDER BY S.[CNO] ASC
            """;

        SqliteParameter[] parameters = [new("@Sno", sno)];

        return await _database.ExecuteQueryAsync(sql, parameters);
    }

    public async Task<Score?> GetScoreAsync(string sno, string cno)
    {
        string sql = """
            SELECT *
            FROM [Score]
            WHERE [SNO] = @Sno
              AND [CNO] = @Cno
            """;

        SqliteParameter[] parameters = [new("@Sno", sno), new("@Cno", cno)];

        DataTable dataTable = await _database.ExecuteQueryAsync(sql, parameters);

        return DataTableConvertToList(dataTable).FirstOrDefault();
    }

    public async Task<int> InsertScoreAsync(string sno, string cno, int grade)
    {
        string sql = """
            INSERT INTO [Score]
            (
                [SNO],
                [CNO],
                [GRADE],
                [RECORDEDAT]
            )
            VALUES
            (
                @Sno,
                @Cno,
                @Grade,
                @RecordedAt
            )
            """;

        SqliteParameter[] parameters =
        [
            new("@Sno", sno),
            new("@Cno", cno),
            new("@Grade", grade),
            new("@RecordedAt", DateTime.Now),
        ];

        return await _database.ExecuteNonQueryAsync(sql, parameters);
    }

    public async Task<int> UpdateScoreAsync(string sno, string cno, int grade)
    {
        string sql = """
            UPDATE [Score]
            SET
                [GRADE] = @Grade,
                [RECORDEDAT] = @RecordedAt
            WHERE [SNO] = @Sno
              AND [CNO] = @Cno
            """;

        SqliteParameter[] parameters =
        [
            new("@Grade", grade),
            new("@RecordedAt", DateTime.Now),
            new("@Sno", sno),
            new("@Cno", cno),
        ];

        return await _database.ExecuteNonQueryAsync(sql, parameters);
    }

    public async Task<DataTable> GetScoresByCourseAsync(string cno)
    {
        string sql = """
            SELECT
                S.[SNO],
                ST.[SNAME],
                S.[CNO],
                C.[CNAME],
                S.[GRADE]
            FROM [Score] S
            JOIN [Student] ST ON S.[SNO] = ST.[SNO]
            JOIN [Course] C ON S.[CNO] = C.[CNO]
            WHERE S.[CNO] = @Cno
            ORDER BY S.[GRADE] DESC
            """;

        SqliteParameter[] parameters = [new("@Cno", cno)];

        return await _database.ExecuteQueryAsync(sql, parameters);
    }
}
