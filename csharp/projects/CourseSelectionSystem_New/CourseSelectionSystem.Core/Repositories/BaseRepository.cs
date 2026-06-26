using System.ComponentModel.DataAnnotations;
using System.Data;
using System.Reflection;
using CourseSelectionSystem.Core.Data;
using Microsoft.Data.Sqlite;

namespace CourseSelectionSystem.Core.Repositories;

public abstract class BaseRepository<T>
    where T : class
{
    protected readonly Database _database;
    protected readonly string _tableName;
    protected readonly string? _primaryKeyName;
    protected readonly string? _searchColumnName;

    private readonly PropertyInfo[] _properties;

    protected BaseRepository(Database database, string? searchColumnName = null)
    {
        _database = database;
        _tableName = typeof(T).Name;
        _properties = typeof(T).GetProperties();
        _searchColumnName = searchColumnName;

        var keyProperty = _properties.FirstOrDefault(property =>
            property.GetCustomAttribute<KeyAttribute>() != null
        );

        _primaryKeyName = keyProperty?.Name;
    }

    protected static string SqlName(string name)
    {
        return $"[{name}]";
    }

    private static bool IsAutoIncrementKey(PropertyInfo property)
    {
        return property.Name == "ID" && property.PropertyType == typeof(int);
    }

    private static object? ConvertValue(object value, Type targetType)
    {
        if (value == DBNull.Value)
        {
            return null;
        }

        Type realType = Nullable.GetUnderlyingType(targetType) ?? targetType;

        if (realType.IsEnum)
        {
            return Enum.ToObject(realType, value);
        }

        return Convert.ChangeType(value, realType);
    }

    protected List<T> DataTableConvertToList(DataTable dataTable)
    {
        var list = new List<T>();

        foreach (DataRow row in dataTable.Rows)
        {
            T item = Activator.CreateInstance<T>();

            foreach (var property in _properties)
            {
                if (!dataTable.Columns.Contains(property.Name))
                {
                    continue;
                }

                object value = row[property.Name];

                if (value == DBNull.Value)
                {
                    continue;
                }

                object? convertedValue = ConvertValue(value, property.PropertyType);
                property.SetValue(item, convertedValue);
            }

            list.Add(item);
        }

        return list;
    }

    public virtual async Task<int> AddAsync(T entity)
    {
        var insertProperties = _properties
            .Where(property => !IsAutoIncrementKey(property))
            .ToArray();

        string columns = string.Join(
            ",",
            insertProperties.Select(property => SqlName(property.Name))
        );

        string values = string.Join(",", insertProperties.Select(property => $"@{property.Name}"));

        string sql = $"INSERT INTO {SqlName(_tableName)} ({columns}) VALUES ({values})";

        var parameters = new List<SqliteParameter>();

        foreach (var property in insertProperties)
        {
            object value = property.GetValue(entity) ?? DBNull.Value;
            parameters.Add(new SqliteParameter($"@{property.Name}", value));
        }

        return await _database.ExecuteNonQueryAsync(sql, parameters.ToArray());
    }

    public virtual async Task<List<T>> GetAllAsync()
    {
        string sql = $"SELECT * FROM {SqlName(_tableName)}";

        DataTable dataTable = await _database.ExecuteQueryAsync(sql);

        return DataTableConvertToList(dataTable);
    }

    public virtual async Task<T?> GetByIdAsync(object id)
    {
        if (string.IsNullOrEmpty(_primaryKeyName))
        {
            return null;
        }

        string sql = $"SELECT * FROM {SqlName(_tableName)} WHERE {SqlName(_primaryKeyName)} = @Id";

        DataTable dataTable = await _database.ExecuteQueryAsync(
            sql,
            new SqliteParameter("@Id", id)
        );

        var list = DataTableConvertToList(dataTable);

        return list.FirstOrDefault();
    }

    public virtual async Task<DataTable?> SearchByNameAsync(string keyword)
    {
        if (string.IsNullOrEmpty(_searchColumnName))
        {
            return null;
        }

        string sql = $"""
            SELECT *
            FROM {SqlName(_tableName)}
            WHERE {SqlName(_searchColumnName)} LIKE @Keyword
            """;

        SqliteParameter[] parameters = [new("@Keyword", $"%{keyword}%")];

        return await _database.ExecuteQueryAsync(sql, parameters);
    }

    public virtual async Task<int> DeleteAsync(object id)
    {
        if (string.IsNullOrEmpty(_primaryKeyName))
        {
            return 0;
        }

        string sql = $"DELETE FROM {SqlName(_tableName)} WHERE {SqlName(_primaryKeyName)} = @Id";

        return await _database.ExecuteNonQueryAsync(sql, new SqliteParameter("@Id", id));
    }

    public virtual async Task<int> UpdateAsync(T entity)
    {
        if (string.IsNullOrEmpty(_primaryKeyName))
        {
            return 0;
        }

        var updateProperties = _properties
            .Where(property => property.Name != _primaryKeyName)
            .ToArray();

        string setString = string.Join(
            ",",
            updateProperties.Select(property => $"{SqlName(property.Name)} = @{property.Name}")
        );

        string sql =
            $"UPDATE {SqlName(_tableName)} SET {setString} WHERE {SqlName(_primaryKeyName)} = @{_primaryKeyName}";

        var parameters = new List<SqliteParameter>();

        foreach (var property in _properties)
        {
            object value = property.GetValue(entity) ?? DBNull.Value;
            parameters.Add(new SqliteParameter($"@{property.Name}", value));
        }

        return await _database.ExecuteNonQueryAsync(sql, parameters.ToArray());
    }
}
