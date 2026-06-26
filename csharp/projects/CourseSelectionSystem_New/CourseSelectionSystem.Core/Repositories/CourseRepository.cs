using System.Data;
using CourseSelectionSystem.Core.Data;
using CourseSelectionSystem.Core.Models;
using Microsoft.Data.Sqlite;

namespace CourseSelectionSystem.Core.Repositories;

public class CourseRepository : BaseRepository<Course>
{
    public CourseRepository(Database database)
        : base(database, "CNAME") { }

    public async Task<List<Course>> GetCoursesByCollegeAsync(string collegeId)
    {
        string sql = """
            SELECT *
            FROM [Course]
            WHERE [COLLEGEID] = @CollegeId
            ORDER BY [CNO] ASC
            """;

        SqliteParameter[] parameters = [new("@CollegeId", collegeId)];

        DataTable dataTable = await _database.ExecuteQueryAsync(sql, parameters);

        return DataTableConvertToList(dataTable);
    }

    public async Task<List<Course>> GetCoursesByTeacherAsync(string teacherId)
    {
        string sql = """
            SELECT *
            FROM [Course]
            WHERE [TNO] = @TeacherId
            ORDER BY [CNO] ASC
            """;

        SqliteParameter[] parameters = [new("@TeacherId", teacherId)];

        DataTable dataTable = await _database.ExecuteQueryAsync(sql, parameters);

        return DataTableConvertToList(dataTable);
    }

    public async Task<List<Course>> GetOpenCoursesAsync()
    {
        string sql = """
            SELECT *
            FROM [Course]
            WHERE [ISOPEN] = 1
            ORDER BY [CNO] ASC
            """;

        DataTable dataTable = await _database.ExecuteQueryAsync(sql);

        return DataTableConvertToList(dataTable);
    }

    public async Task<DataTable> GetCoursesByCreditAsync(decimal minCredit)
    {
        string sql = """
            SELECT *
            FROM [Course]
            WHERE [CCREDIT] >= @Credit
            ORDER BY [CCREDIT] DESC
            """;

        SqliteParameter[] parameters = [new("@Credit", minCredit)];

        return await _database.ExecuteQueryAsync(sql, parameters);
    }

    public async Task<DataTable> GetCourseSelectionCountAsync()
    {
        string sql = """
            SELECT
                C.[CNO],
                C.[CNAME],
                C.[CCREDIT],
                C.[CAPACITY],
                COUNT(CS.[SNO]) AS [StudentCount]
            FROM [Course] C
            LEFT JOIN [CourseSelection] CS ON C.[CNO] = CS.[CNO]
            GROUP BY C.[CNO], C.[CNAME], C.[CCREDIT], C.[CAPACITY]
            ORDER BY C.[CNO] ASC
            """;

        return await _database.ExecuteQueryAsync(sql);
    }

    public async Task<int> UpdateCourseIntroductionAsync(string cno, string introduction)
    {
        string sql = """
            UPDATE [Course]
            SET [INTRODUCTION] = @Introduction
            WHERE [CNO] = @Cno
            """;

        SqliteParameter[] parameters = [new("@Introduction", introduction), new("@Cno", cno)];

        return await _database.ExecuteNonQueryAsync(sql, parameters);
    }

    public async Task<int> UpdateCourseOpenStatusAsync(string cno, bool isOpen)
    {
        string sql = """
            UPDATE [Course]
            SET [ISOPEN] = @IsOpen
            WHERE [CNO] = @Cno
            """;

        SqliteParameter[] parameters = [new("@IsOpen", isOpen), new("@Cno", cno)];

        return await _database.ExecuteNonQueryAsync(sql, parameters);
    }
}
