using System.Data;
using Microsoft.Data.SqlClient;

public class ScoreRepository : BaseRepository<Score>
{
    public ScoreRepository(Database database) : base(database) {}

    public async Task<DataTable> GetMyScoresAsync(string studentId)
    {
        string sql = @"
                SELECT 
                    S.CNO, 
                    C.CNAME, 
                    C.CCREDIT, 
                    S.GRADE 
                FROM Score S
                JOIN Course C ON S.CNO = C.CNO
                WHERE S.SNO = @Sno";

        var parameters = new[] {
                new SqlParameter("@Sno", studentId)
            };

        return await _database.ExecuteQueryAsync(sql, parameters);
    }

    public async Task<bool> SelectCourseAsync(string studentId, string courseId)
    {

        string checkSql = "SELECT COUNT(*) FROM Score WHERE SNO = @Sno AND CNO = @Cno";

        var checkParams = new[] {
            new SqlParameter("@Sno", studentId),
            new SqlParameter("@Cno", courseId)
        };

        DataTable dt = await _database.ExecuteQueryAsync(checkSql, checkParams);
        if (dt.Rows.Count > 0 && Convert.ToInt32(dt.Rows[0][0]) > 0)
        {
            return false; 
        }

        var insertParams = new[] {
            new SqlParameter("@Sno", studentId),
            new SqlParameter("@Cno", courseId)
        };

        string insertSql = "INSERT INTO Score (SNO, CNO, GRADE) VALUES (@Sno, @Cno, 0)";

        await _database.ExecuteNonQueryAsync(insertSql, insertParams);
        return true;
    }

    public override Task<int> DeleteAsync(object id)
    {
        throw new NotSupportedException("Score表是联合主键(SNO+CNO), 不能使用单参数删除! 请使用 DeleteScoreAsync(sno, cno)。");
    }

    public override Task<int> UpdateAsync(Score entity)
    {
        throw new NotSupportedException("Score表是联合主键, 通用更新无法定位! 请使用 UpdateScoreAsync。");
    }

    public override Task<Score?> GetByIdAsync(object id)
    {
        throw new NotSupportedException("Score表是联合主键, 不能按单ID查询! 请使用 GetScoreAsync(sno, cno)。");
    }

    public override Task<DataTable?> SearchByNameAsync(string keyword)
    {
        throw new NotSupportedException("Score 表不支持按名搜索！");
    }

    public async Task<int> DeleteScoreAsync(string sno, string cno)
    {
        string sql = "DELETE FROM Score WHERE SNO = @SNO AND CNO = @CNO";
        SqlParameter[] parameter = 
        [
            new("@SNO", sno),
            new("@CNO", cno)
        ];

        return await _database.ExecuteNonQueryAsync(sql, parameter);
    }

    public async Task<int> UpdateScoreAsync(Score score)
    {
        string sql = "UPDATE Score SET Grade = @Grade WHERE SNO = @SNO AND CNO = @CNO";

        SqlParameter[] parameter =
        [
            new("@Grade", score.GRADE),
            new("@SNO", score.SNO),
            new("@CNO", score.CNO)
        ];

        return await _database.ExecuteNonQueryAsync(sql, parameter);
    } 

    public async Task<Score?> GetScoreAsync(string sno, string cno)
    {
        string sql = "SELECT * FROM Score WHERE SNO = @SNO AND CNO = @CNO";
        SqlParameter[] parameter = 
        [
            new("@SNO", sno),
            new("@CNO", cno)
        ];

        DataTable datatable = await _database.ExecuteQueryAsync(sql, parameter);
        if(datatable.Rows.Count == 0) return null;

        DataRow row = datatable.Rows[0];
        return new Score
        {
            SNO = row["SNO"].ToString()!,
            CNO = row["CNO"].ToString()!,
            GRADE = (int)row["GRADE"]
        };
    }



    public async Task<DataTable> GetSpecificCourseScoreAsync()
    {
        string sql = @"
            SELECT Student.SNO, Course.CNAME, Score.GRADE 
            FROM Score 
            JOIN Student ON Score.SNO = Student.SNO
            JOIN Course ON Score.CNO = Course.CNO
            WHERE Score.CNO IN ('CS103', 'CS123') 
            ORDER BY Score.CNO ASC, Score.GRADE DESC";
        
        return await _database.ExecuteQueryAsync(sql);
    }

    public async Task<DataTable> GetMaxGradePerStudentAsync()
    {
        string sql = @"
            SELECT Student.SNAME, Score.CNO, T.MaxGrade
            FROM Student
            JOIN Score ON Student.SNO = Score.SNO
            JOIN (SELECT SNO, MAX(GRADE) as MaxGrade FROM Score GROUP BY SNO) T 
            ON Score.SNO = T.SNO AND Score.GRADE = T.MaxGrade";
        
        return await _database.ExecuteQueryAsync(sql);
    }

    public async Task UpdateScoreViaViewAsync()
    {
        string viewSql = @"
            IF OBJECT_ID('STUDENT_CS', 'V') IS NULL
            EXEC('CREATE VIEW STUDENT_CS AS SELECT * FROM Student WHERE SDEPT = ''计算机系''')";
        await _database.ExecuteNonQueryAsync(viewSql);

        string updateSql = @"
            UPDATE Score SET GRADE = GRADE + 5 
            WHERE CNO = 'CS101' 
            AND SNO IN (SELECT SNO FROM STUDENT_CS)";
        await _database.ExecuteNonQueryAsync(updateSql);
    }

    public async Task<DataTable> ProcedureGetScoreAsync(int low, int high)
    {
        SqlParameter[] parameter =
        [ 
            new("@low", low), new("@high", high) 
        ];
        return await _database.ExecuteProcedureAsync("Score_Select", parameter);
    }

    public async Task<DataTable> ProcedureDeleteScoreAsync(int low, int high)
    {
        SqlParameter[] parameter = 
        [ 
            new("@low", low), new("@high", high) 
        ];
        return await _database.ExecuteProcedureAsync("Score_Delete", parameter);
    }

}