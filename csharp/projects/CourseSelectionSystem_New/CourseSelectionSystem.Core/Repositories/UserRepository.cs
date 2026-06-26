using System.Data;
using CourseSelectionSystem.Core.Data;
using CourseSelectionSystem.Core.Models;
using Microsoft.Data.Sqlite;

namespace CourseSelectionSystem.Core.Repositories;

public class UserRepository : BaseRepository<User>
{
    public UserRepository(Database database)
        : base(database, "ACCOUNT") { }

    public async Task<User?> FindByAccountAsync(string account)
    {
        string sql = """
            SELECT *
            FROM [User]
            WHERE [ACCOUNT] = @Account
            """;

        SqliteParameter[] parameters = [new("@Account", account)];

        DataTable dataTable = await _database.ExecuteQueryAsync(sql, parameters);

        return DataTableConvertToList(dataTable).FirstOrDefault();
    }

    public async Task<List<User>> GetUsersByRoleAsync(UserRole role)
    {
        string sql = """
            SELECT *
            FROM [User]
            WHERE [ROLE] = @Role
            """;

        SqliteParameter[] parameters = [new("@Role", (int)role)];

        DataTable dataTable = await _database.ExecuteQueryAsync(sql, parameters);

        return DataTableConvertToList(dataTable);
    }

    public async Task<List<User>> GetUsersByCollegeAsync(string collegeId)
    {
        string sql = """
            SELECT *
            FROM [User]
            WHERE [COLLEGEID] = @CollegeId
            """;

        SqliteParameter[] parameters = [new("@CollegeId", collegeId)];

        DataTable dataTable = await _database.ExecuteQueryAsync(sql, parameters);

        return DataTableConvertToList(dataTable);
    }
}
