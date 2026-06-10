using System.Data;
using Microsoft.Data.SqlClient;

public class StudentRepository : BaseRepository<Student>
{
    public StudentRepository(Database database) : base(database, "SNAME") {}
    
    public async Task<DataTable> GetStudentBirthYearAsync()
    {
        string sql = "SELECT SNO, SNAME, (2025-SAGE) AS BirthYear FROM Student";
        return await _database.ExecuteQueryAsync(sql);
    }

    public async Task<DataTable> GetDeptCountAsync()
    {
        string sql = "SELECT SDEPT, COUNT(*) AS Total FROM Student GROUP BY SDEPT ORDER BY Total DESC";
        return await _database.ExecuteQueryAsync(sql);
    }

    public async Task<DataTable> GetStudentsNoCourseAsync()
    {
        string sql = "SELECT SNAME, SDEPT FROM Student WHERE SNO NOT IN (SELECT DISTINCT SNO FROM Score)";
        return await _database.ExecuteQueryAsync(sql);
    }

    public async Task<DataTable> GetStudentsByTeacherNameAsync(string teacherName)
    {
        string sql = @"
            SELECT S.* FROM Student S
            JOIN Score Sc ON S.SNO = Sc.SNO
            JOIN Teacher T ON Sc.CNO = T.CNO
            WHERE T.TNAME = @TName";
        
        return await _database.ExecuteQueryAsync(sql, new SqlParameter("@TName", teacherName));
    }

    public async Task CreateS1TableAsync()
    {
        string sql = @"
            IF OBJECT_ID('S1', 'U') IS NOT NULL DROP TABLE S1;
            CREATE TABLE S1(SNO CHAR(20), SNAME NVARCHAR(20), SD NVARCHAR(50), SA INT);
            INSERT INTO S1 SELECT SNO, SNAME, SDEPT, SAGE FROM Student WHERE SDEPT = '计算机系';";
        
        await _database.ExecuteNonQueryAsync(sql);
    }

    
}