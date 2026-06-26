using System.Data;
using CourseSelectionSystem.Core.Data;
using CourseSelectionSystem.Core.Models;
using Microsoft.Data.Sqlite;

namespace CourseSelectionSystem.Core.Repositories;

public class StudentRepository : BaseRepository<Student>
{
    public StudentRepository(Database database)
        : base(database, "SNAME") { }

    public async Task<List<Student>> GetStudentsByCollegeAsync(string collegeId)
    {
        string sql = """
            SELECT *
            FROM [Student]
            WHERE [COLLEGEID] = @CollegeId
            ORDER BY [SNO] ASC
            """;

        SqliteParameter[] parameters = [new("@CollegeId", collegeId)];

        DataTable dataTable = await _database.ExecuteQueryAsync(sql, parameters);

        return DataTableConvertToList(dataTable);
    }

    public async Task<DataTable> GetStudentBirthYearAsync()
    {
        string sql = """
            SELECT
                [SNO],
                [SNAME],
                CAST(strftime('%Y', 'now') AS INTEGER) - [SAGE] AS [BirthYear]
            FROM [Student]
            """;

        return await _database.ExecuteQueryAsync(sql);
    }

    public async Task<DataTable> GetDeptCountAsync()
    {
        string sql = """
            SELECT
                [SDEPT],
                COUNT(*) AS [Total]
            FROM [Student]
            GROUP BY [SDEPT]
            ORDER BY [Total] DESC
            """;

        return await _database.ExecuteQueryAsync(sql);
    }

    public async Task<DataTable> GetStudentsNoCourseAsync()
    {
        string sql = """
            SELECT
                [SNAME],
                [SDEPT]
            FROM [Student]
            WHERE [SNO] NOT IN
            (
                SELECT DISTINCT [SNO]
                FROM [CourseSelection]
            )
            """;

        return await _database.ExecuteQueryAsync(sql);
    }
}
