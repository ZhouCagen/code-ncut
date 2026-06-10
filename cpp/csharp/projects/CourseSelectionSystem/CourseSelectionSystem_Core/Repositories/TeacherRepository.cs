using System.Data;
using Microsoft.Data.SqlClient;

public class TeacherRepository : BaseRepository<Teacher>
{
    public TeacherRepository(Database database) : base(database, "TNAME") {}

    public async Task<DataTable> GetPopularTeachersAsync()
    {
        string sql = @"
            SELECT TNAME FROM Teacher 
            WHERE CNO IN (
                SELECT CNO FROM Score 
                GROUP BY CNO 
                HAVING COUNT(*) > 3
            )";
        return await _database.ExecuteQueryAsync(sql);
    } 

    public async Task<DataTable> GetTeachersByCourseAsync(string courseName)
    {
        string sql = @"
            SELECT TNAME, CNO FROM Teacher 
            WHERE CNO IN (SELECT CNO FROM Course WHERE CNAME = @CName)";
        
        return await _database.ExecuteQueryAsync(sql, new SqlParameter("@CName", courseName));
    }


}