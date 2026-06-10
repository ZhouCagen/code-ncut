using System.Data;
using Microsoft.Data.SqlClient;

public class CourseRepository : BaseRepository<Course>
{
    public CourseRepository(Database database) : base(database, "CNAME") {} 

    public async Task<DataTable> GetCoursesByCreditAsync(int minCredit)
    {
        string sql = "SELECT * FROM Course WHERE CCREDIT >= @Credit ORDER BY CCREDIT DESC";
        
        SqlParameter[] parameters = [ new("@Credit", minCredit) ];
        
        return await _database.ExecuteQueryAsync(sql, parameters);
    }

    public async Task<DataTable> GetCourseDataAsync()
    {
        string sql = @"
            SELECT 
                C.CNO, 
                C.CNAME, 
                C.CCREDIT,
                COUNT(S.SNO) AS StudentCount
            FROM Course C
            LEFT JOIN Score S ON C.CNO = S.CNO  
            GROUP BY C.CNO, C.CNAME, C.CCREDIT
            ORDER BY C.CNO ASC";

        return await _database.ExecuteQueryAsync(sql);
    }
}