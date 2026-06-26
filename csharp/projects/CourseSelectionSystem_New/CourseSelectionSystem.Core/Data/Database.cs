using System.Data;
using Microsoft.Data.Sqlite;

namespace CourseSelectionSystem.Core.Data;

public class Database
{
    private readonly string _connectionString;

    public Database(string connectionString)
    {
        _connectionString = connectionString;
    }

    private async Task<SqliteConnection> GetOpenConnectionAsync()
    {
        var connection = new SqliteConnection(_connectionString);
        await connection.OpenAsync();

        using var command = connection.CreateCommand();
        command.CommandText = "PRAGMA foreign_keys = ON;";
        await command.ExecuteNonQueryAsync();

        return connection;
    }

    private async Task<T> ExecuteCoreAsync<T>(
        string sql,
        SqliteParameter[]? parameters,
        Func<SqliteCommand, Task<T>> func
    )
    {
        using var connection = await GetOpenConnectionAsync();
        using var command = new SqliteCommand(sql, connection);

        if (parameters != null)
        {
            foreach (var parameter in parameters)
            {
                command.Parameters.Add(parameter);
            }
        }

        return await func(command);
    }

    public async Task<int> ExecuteNonQueryAsync(string sql, params SqliteParameter[]? parameters)
    {
        return await ExecuteCoreAsync(
            sql,
            parameters,
            async command =>
            {
                return await command.ExecuteNonQueryAsync();
            }
        );
    }

    public async Task<object?> ExecuteScalarAsync(string sql, params SqliteParameter[]? parameters)
    {
        return await ExecuteCoreAsync(
            sql,
            parameters,
            async command =>
            {
                return await command.ExecuteScalarAsync();
            }
        );
    }

    public async Task<DataTable> ExecuteQueryAsync(string sql, params SqliteParameter[]? parameters)
    {
        return await ExecuteCoreAsync(
            sql,
            parameters,
            async command =>
            {
                using var reader = await command.ExecuteReaderAsync();

                var dataTable = new DataTable();

                for (int i = 0; i < reader.FieldCount; i++)
                {
                    string columnName = reader.GetName(i);

                    if (string.IsNullOrWhiteSpace(columnName))
                    {
                        columnName = $"Column{i}";
                    }

                    columnName = MakeUniqueColumnName(dataTable, columnName);

                    Type columnType = typeof(object);

                    try
                    {
                        columnType = reader.GetFieldType(i);
                    }
                    catch
                    {
                        columnType = typeof(object);
                    }

                    dataTable.Columns.Add(
                        columnName,
                        Nullable.GetUnderlyingType(columnType) ?? columnType
                    );
                }

                while (await reader.ReadAsync())
                {
                    DataRow row = dataTable.NewRow();

                    for (int i = 0; i < reader.FieldCount; i++)
                    {
                        row[i] = await reader.IsDBNullAsync(i) ? DBNull.Value : reader.GetValue(i);
                    }

                    dataTable.Rows.Add(row);
                }

                return dataTable;
            }
        );
    }

    private static string MakeUniqueColumnName(DataTable dataTable, string columnName)
    {
        if (!dataTable.Columns.Contains(columnName))
        {
            return columnName;
        }

        int index = 1;

        while (dataTable.Columns.Contains($"{columnName}_{index}"))
        {
            index++;
        }

        return $"{columnName}_{index}";
    }

    public Task<DataTable> ExecuteProcedureAsync(
        string procedureName,
        params SqliteParameter[]? parameters
    )
    {
        throw new NotSupportedException("SQLite 不支持 SQL Server 风格的存储过程。");
    }
}
