using System.Data;
using Microsoft.Data.SqlClient;

public class Database
{
    private readonly string _connectionString;

    public Database(string connectionString)
    {
        _connectionString = connectionString;
    }

    private async Task<SqlConnection> GetOpenConnectionAsync()
    {
        var connect = new SqlConnection(_connectionString);
        await connect.OpenAsync();
        return connect;
    }

    private async Task<T> ExecuteCoreAsync<T>(string sql, SqlParameter[]? parameters, Func<SqlCommand, Task<T>> func)
    {
        using var connection = await GetOpenConnectionAsync();
        using var command = new SqlCommand(sql, connection);
        if(parameters != null) command.Parameters.AddRange(parameters);

        return await func(command);
    }

    public async Task<int> ExecuteNonQueryAsync(string sql, params SqlParameter[]? parameters)
    {
        return await ExecuteCoreAsync(sql, parameters, async (command) => 
        { 
            return await command.ExecuteNonQueryAsync(); 
        });
    }

    public async Task<DataTable> ExecuteQueryAsync(string sql, params SqlParameter[]? parameters)
    {
        return await ExecuteCoreAsync(sql, parameters, (command) =>
        {
            using var adapter =  new SqlDataAdapter(command);
            DataTable datatable = new DataTable();
            adapter.Fill(datatable);
            return Task.FromResult(datatable);
        });
    }

    public async Task<DataTable> ExecuteProcedureAsync(string sql, params SqlParameter[]? parameters)
    {
        return await ExecuteCoreAsync(sql, parameters, (command) =>
        {
            command.CommandType = CommandType.StoredProcedure;
            using var adapter = new SqlDataAdapter(command);
            DataTable datatable = new DataTable();
            adapter.Fill(datatable);
            return Task.FromResult(datatable);
        });
    }
}