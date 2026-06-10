using System.Data;
using System.Linq;
using System.Collections.Generic;
using System.Threading.Tasks;
using System.Reflection;
using Microsoft.Data.SqlClient;
using System.ComponentModel.DataAnnotations;

public abstract class BaseRepository<T> where T : class
{
    protected readonly Database _database;
    protected readonly string _tableName; 
    protected readonly string?  _primaryKeyName;
    protected readonly string? _searchTableName;

    private readonly PropertyInfo[] _properties;

    public BaseRepository(Database database, string? searchTableName = null)
    {
        _database = database;
        _tableName = typeof(T).Name;
        _properties = typeof(T).GetProperties();
        _searchTableName = searchTableName;
        var keyProperty = _properties.FirstOrDefault(properity => properity.GetCustomAttribute<KeyAttribute>() != null);
        _primaryKeyName = keyProperty?.Name;
    }

    public virtual async Task<int> AddAsync(T entity)
    {
        string columns = string.Join(",", _properties.Select(property => property.Name));
        string values = string.Join(",", _properties.Select(property => $"@{property.Name}"));
        
        string sql = $"INSERT INTO {_tableName} ({columns}) VALUES ({values})";

        var parameters = new List<SqlParameter>();
        foreach(var property in _properties)
        {
            object value = property.GetValue(entity) ?? DBNull.Value;
            parameters.Add(new SqlParameter($"@{property.Name}", value));
        }

        return await _database.ExecuteNonQueryAsync(sql, parameters.ToArray());
    }

    private List<T> DataTableConvertToList(DataTable datatable)
    {
        var list = new List<T>();

        foreach(DataRow row in datatable.Rows)
        {
            T item = Activator.CreateInstance<T>();
            foreach(var property in _properties)
            {
                if(datatable.Columns.Contains(property.Name))
                {
                    object value = row[property.Name];
                    if(value != DBNull.Value)
                    {
                        property.SetValue(item, value);
                    }
                }
            }
            list.Add(item);
        }

        return list;
    }

    public virtual async Task<List<T>> GetAllAsync()
    {
        string sql = $"SELECT * FROM {_tableName}";

        DataTable datatable = await _database.ExecuteQueryAsync(sql);

        return DataTableConvertToList(datatable);
    }

    public virtual async Task<T?> GetByIdAsync(object id)
    {   
        if(string.IsNullOrEmpty(_primaryKeyName))
        {
            return null;
        }

        string sql = $"SELECT * FROM {_tableName} WHERE {_primaryKeyName} = @Id";

        DataTable datatable = await _database.ExecuteQueryAsync(sql, new SqlParameter("@Id", id));

        var list = DataTableConvertToList(datatable);
        return list.FirstOrDefault();
    }

    public virtual async Task<DataTable?> SearchByNameAsync(string keyword)
    {
        if(string.IsNullOrEmpty(_searchTableName))
        {
            return null;
        }

        string sql = $"SELECT * FROM {_tableName} WHERE {_searchTableName} LIKE @Key";
            
        SqlParameter[] parameters = [ new("@Key", $"%{keyword}%") ];
        return await _database.ExecuteQueryAsync(sql, parameters);
    }

    public virtual async Task<int> DeleteAsync(object id)
    {
        if(string.IsNullOrEmpty(_primaryKeyName))
        {
            return 0;
        }

        string sql = $"DELETE FROM {_tableName} WHERE {_primaryKeyName} = @Id";

        return await _database.ExecuteNonQueryAsync(sql, new SqlParameter("@Id", id));
    }

    public virtual async Task<int> UpdateAsync(T entity)
    {
        if(string.IsNullOrEmpty(_primaryKeyName))
        {
            return 0;
        }

        var updateProperties = _properties.Where(p => p.Name != _primaryKeyName);

        string setString = string.Join(",", updateProperties.Select(property => $"{property.Name} = @{property.Name}"));
        string sql = $"UPDATE {_tableName} SET {setString} WHERE {_primaryKeyName}=@{_primaryKeyName}";

        var parameters = new List<SqlParameter>();
        foreach(var property in _properties)
        {
            object value = property.GetValue(entity) ?? DBNull.Value;
            parameters.Add(new SqlParameter($"@{property.Name}", value));
        }

        return await _database.ExecuteNonQueryAsync(sql, parameters.ToArray());
    }
}