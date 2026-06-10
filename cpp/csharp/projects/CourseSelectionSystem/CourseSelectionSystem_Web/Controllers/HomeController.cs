using System.Data;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Data.SqlClient;

namespace CourseSelectionSystem_Web.Controllers;

public class HomeController : Controller
{
    private readonly Database _db;

    public HomeController(Database db)
    {
        _db = db;
    }

    public IActionResult Login() => View();

    [HttpPost]
    public IActionResult Login(string userId, string role)
    {
        if (role == "Teacher") return RedirectToAction("Teacher", new { id = userId });
        return RedirectToAction("Student", new { id = userId });
    }

    public async Task<IActionResult> Student(string id)
    {
        ViewData["UserID"] = id;

        var courseRepo = new CourseRepository(_db);
        DataTable dtCourses = await courseRepo.GetCourseDataAsync();

        var scoreRepo = new ScoreRepository(_db);
        DataTable dtMyScores = await scoreRepo.GetMyScoresAsync(id);

        return View(Tuple.Create(dtCourses, dtMyScores));
    }

    [HttpPost] 
    public async Task<IActionResult> SelectCourse(string userId, string courseId)
    {
        var repo = new ScoreRepository(_db);
        bool success = await repo.SelectCourseAsync(userId, courseId);
        return Json(new { success = success, msg = success ? "选课成功！" : "选课失败，可能已选过。" });
    }

    [HttpPost] 
    public async Task<IActionResult> WithdrawCourse(string userId, string courseId)
    {
        var repo = new ScoreRepository(_db);
        int rows = await repo.DeleteScoreAsync(userId, courseId);
        return Json(new { success = rows > 0, msg = rows > 0 ? "退课成功！" : "退课失败。" });
    }

    public async Task<IActionResult> Teacher(string id)
    {
        ViewData["TeacherID"] = id;

        string sqlTeacher = "SELECT CNO FROM Teacher WHERE TNO = @Tno";
        DataTable dtTeacher = await _db.ExecuteQueryAsync(sqlTeacher, new[] { new SqlParameter("@Tno", id) });

        if (dtTeacher.Rows.Count == 0) return Content("未找到您负责的课程。");
        string cno = dtTeacher.Rows[0]["CNO"].ToString()!;
        ViewData["CourseID"] = cno;

        string sqlStudents = @"SELECT Student.SNO, Student.SNAME, Score.GRADE FROM Score JOIN Student ON Score.SNO = Student.SNO WHERE Score.CNO = @Cno";
        DataTable dtStudents = await _db.ExecuteQueryAsync(sqlStudents, new[] { new SqlParameter("@Cno", cno) });

        return View(dtStudents);
    }

    [HttpPost] 
    public async Task<IActionResult> UpdateGrade(string sno, string cno, int grade)
    {
        var repo = new ScoreRepository(_db);
        var scoreObj = new Score { SNO = sno, CNO = cno, GRADE = grade };
        await repo.UpdateScoreAsync(scoreObj);
        return Json(new { success = true });
    }

    public IActionResult Homework()
    {
        return View(new DataTable()); 
    }

    [HttpPost]
    public async Task<IActionResult> Homework(string task, string p1, string p2)
    {
        DataTable result = new DataTable();
        try
        {
            int.TryParse(p1, out int val1);
            int.TryParse(p2, out int val2);

            switch (task)
            {
                case "1": result = await new StudentRepository(_db).GetStudentBirthYearAsync(); break;
                case "2": result = await new StudentRepository(_db).GetDeptCountAsync(); break;
                case "3": result = await new ScoreRepository(_db).GetSpecificCourseScoreAsync(); break;
                case "4": result = await new TeacherRepository(_db).GetPopularTeachersAsync(); break;
                case "5": result = await new StudentRepository(_db).GetStudentsNoCourseAsync(); break;
                case "6": result = await new ScoreRepository(_db).GetMaxGradePerStudentAsync(); break;
                case "7": result = await new TeacherRepository(_db).GetTeachersByCourseAsync("数据结构"); break;
                case "8": result = await new StudentRepository(_db).GetStudentsByTeacherNameAsync("郑大力"); break;
                case "9":
                    await new StudentRepository(_db).CreateS1TableAsync();
                    ViewData["Msg"] = "S1 表创建成功！";
                    break;
                case "10":
                    await new ScoreRepository(_db).UpdateScoreViaViewAsync();
                    ViewData["Msg"] = "视图更新成功，分数已加。";
                    break;
                case "11": result = await new ScoreRepository(_db).ProcedureGetScoreAsync(val1, val2); break;
                case "12": result = await new ScoreRepository(_db).ProcedureDeleteScoreAsync(val1, val2); break;
                case "13": result = await new CourseRepository(_db).GetCoursesByCreditAsync(val1); break;
            }
        }
        catch (System.Exception ex)
        {
            ViewData["Msg"] = "执行出错: " + ex.Message;
        }

        return View(result);
    }
}