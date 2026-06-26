using System;
using System.Data;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using CourseSelectionSystem.Core.Data;
using CourseSelectionSystem.Core.DTOs;
using CourseSelectionSystem.Core.Models;
using CourseSelectionSystem.Core.Repositories;
using CourseSelectionSystem.Core.Services;
using Microsoft.Data.Sqlite;

internal static class Program
{
    private static int _passed;
    private static int _failed;

    public static async Task Main()
    {
        string dbPath = Path.Combine(
            Path.GetTempPath(),
            $"course-selection-backend-test-{Guid.NewGuid():N}.db"
        );

        string connectionString = new SqliteConnectionStringBuilder
        {
            DataSource = dbPath,
        }.ToString();

        var database = new Database(connectionString);

        Console.WriteLine("========== CourseSelectionSystem Core 后端冒烟测试 ==========");
        Console.WriteLine($"测试数据库: {dbPath}");
        Console.WriteLine();

        try
        {
            await CreateSchemaAsync(database);
            await SeedDataAsync(database);
            await RunAllTestsAsync(database);
        }
        catch (Exception error)
        {
            Fail($"测试程序整体崩了: {error.GetType().Name}: {error.Message}");
            Console.WriteLine(error);
        }
        finally
        {
            try
            {
                if (File.Exists(dbPath))
                {
                    File.Delete(dbPath);
                }
            }
            catch
            {
                // 测试数据库删不掉也不影响主项目。
            }
        }

        Console.WriteLine();
        Console.WriteLine("========== 测试结束 ==========");
        Console.WriteLine($"通过: {_passed}");
        Console.WriteLine($"失败: {_failed}");

        Environment.ExitCode = _failed == 0 ? 0 : 1;
    }

    private static async Task RunAllTestsAsync(Database database)
    {
        var userRepository = new UserRepository(database);
        var studentRepository = new StudentRepository(database);
        var teacherRepository = new TeacherRepository(database);
        var courseRepository = new CourseRepository(database);
        var prerequisiteRepository = new CoursePrerequisiteRepository(database);
        var selectionRepository = new CourseSelectionRepository(database);
        var scoreRepository = new ScoreRepository(database);
        var coursewareRepository = new CoursewareRepository(database);
        var messageRepository = new MessageRepository(database);

        var authService = new AuthService(userRepository);

        var studentCourseService = new StudentCourseService(
            studentRepository,
            courseRepository,
            prerequisiteRepository,
            selectionRepository,
            scoreRepository,
            coursewareRepository
        );

        var teacherCourseService = new TeacherCourseService(courseRepository, selectionRepository);

        var lotteryService = new LotteryService(courseRepository, selectionRepository);

        var scoreService = new ScoreService(courseRepository, selectionRepository, scoreRepository);

        var coursewareService = new CoursewareService(courseRepository, coursewareRepository);

        var messageService = new MessageService(messageRepository);

        await RunAsync(
            "AuthService 登录测试",
            async () =>
            {
                LoginResult ok = await authService.LoginAsync("student1", "123456");

                Expect(
                    "学生账号可以登录",
                    ok.Success && ok.Role == UserRole.Student && ok.RELATEDID == "S001"
                );

                LoginResult wrongPassword = await authService.LoginAsync(
                    "student1",
                    "wrong-password"
                );

                Expect("错误密码会被拦住", !wrongPassword.Success);

                LoginResult missing = await authService.LoginAsync("not-exist", "123456");

                Expect("不存在账号会被拦住", !missing.Success);
            }
        );

        await RunAsync(
            "Repository 基础查询测试",
            async () =>
            {
                var student = await studentRepository.GetByIdAsync("S001");

                Expect("按主键查询学生", student != null && student.SNAME == "张三");

                var teachers = await teacherRepository.GetTeachersByCollegeAsync("COL01");

                Expect("按学院查询教师", teachers.Count == 2);

                var courses = await courseRepository.GetCoursesByTeacherAsync("T001");

                Expect("按教师查询课程", courses.Any(course => course.CNO == "C101"));

                DataTable birthYearTable = await studentRepository.GetStudentBirthYearAsync();

                Expect(
                    "SQLite 出生年份查询能执行",
                    birthYearTable.Rows.Count >= 2 && birthYearTable.Columns.Contains("BirthYear")
                );
            }
        );

        await RunAsync(
            "StudentCourseService 学生选课测试",
            async () =>
            {
                var openCourses = await studentCourseService.GetOpenCoursesAsync();

                Expect("开放课程能查到", openCourses.Any(course => course.CNO == "C101"));

                Expect(
                    "未开放课程不会出现在开放课程列表",
                    openCourses.All(course => course.CNO != "C400")
                );

                CourseSelectionResult closedResult = await studentCourseService.SelectCourseAsync(
                    "S001",
                    "C400"
                );

                Expect("未开放课程不能选", !closedResult.Success);

                CourseSelectionResult prerequisiteOk = await studentCourseService.SelectCourseAsync(
                    "S001",
                    "C200"
                );

                Expect("已通过先修课的学生可以选高级课程", prerequisiteOk.Success);

                CourseSelectionResult prerequisiteFail =
                    await studentCourseService.SelectCourseAsync("S002", "C200");

                Expect("没通过先修课的学生不能选高级课程", !prerequisiteFail.Success);

                CourseSelectionResult select1 = await studentCourseService.SelectCourseAsync(
                    "S001",
                    "C101"
                );

                Expect("学生第一次选 C101 成功", select1.Success);

                CourseSelectionResult duplicate = await studentCourseService.SelectCourseAsync(
                    "S001",
                    "C101"
                );

                Expect("重复选同一门课会失败", !duplicate.Success);

                CourseSelectionResult select2 = await studentCourseService.SelectCourseAsync(
                    "S002",
                    "C101"
                );

                Expect("第二个学生选 C101 成功", select2.Success);

                DataTable mySelections = await studentCourseService.GetMySelectionsAsync("S001");

                Expect("学生能查自己的选课记录", mySelections.Rows.Count >= 2);
            }
        );

        await RunAsync(
            "LotteryService 抽签测试",
            async () =>
            {
                ServiceResult wrongCollege = await lotteryService.DrawLotsAsync("COL02", "C101");

                Expect("学院管理员不能抽其他学院课程", !wrongCollege.Success);

                ServiceResult drawResult = await lotteryService.DrawLotsAsync("COL01", "C101");

                Expect("本学院课程可以抽签", drawResult.Success);

                DataTable statusTable = await database.ExecuteQueryAsync(
                    """
                    SELECT [STATUS], COUNT(*) AS [Total]
                    FROM [CourseSelection]
                    WHERE [CNO] = 'C101'
                    GROUP BY [STATUS]
                    """
                );

                int wonCount = GetStatusCount(statusTable, SelectionStatus.Won);
                int lostCount = GetStatusCount(statusTable, SelectionStatus.Lost);

                Expect("C101 容量为 1，所以正好 1 个中签", wonCount == 1);
                Expect("C101 两个待抽签学生，所以正好 1 个落选", lostCount == 1);
            }
        );

        await RunAsync(
            "TeacherCourseService 教师课程测试",
            async () =>
            {
                var myCourses = await teacherCourseService.GetMyCoursesAsync("T001");

                Expect("教师能看到自己的课程", myCourses.Any(course => course.CNO == "C101"));

                ServiceResult introOk = await teacherCourseService.UpdateIntroductionAsync(
                    "T001",
                    "C101",
                    "新的课程介绍"
                );

                Expect("任课教师可以改课程介绍", introOk.Success);

                ServiceResult introWrongTeacher =
                    await teacherCourseService.UpdateIntroductionAsync("T002", "C101", "乱改介绍");

                Expect("其他教师不能改这门课", !introWrongTeacher.Success);

                ServiceResult closeOk = await teacherCourseService.CloseCourseAsync("T001", "C101");

                Expect("任课教师可以关闭课程", closeOk.Success);

                ServiceResult publishOk = await teacherCourseService.PublishCourseAsync(
                    "T001",
                    "C101"
                );

                Expect("任课教师可以重新发布课程", publishOk.Success);

                DataTable wonStudents = await teacherCourseService.GetWonStudentsAsync(
                    "T001",
                    "C101"
                );

                Expect("教师可以查看自己课程的中签学生", wonStudents.Rows.Count == 1);
            }
        );

        await RunAsync(
            "ScoreService 成绩测试",
            async () =>
            {
                DataTable wonStudents = await selectionRepository.GetWonStudentsByCourseAsync(
                    "C101"
                );

                string winnerSno = wonStudents.Rows[0]["SNO"].ToString()!;

                ServiceResult invalidGrade = await scoreService.RecordScoreAsync(
                    "T001",
                    "C101",
                    winnerSno,
                    101
                );

                Expect("超过 100 分不能录入", !invalidGrade.Success);

                ServiceResult wrongTeacher = await scoreService.RecordScoreAsync(
                    "T002",
                    "C101",
                    winnerSno,
                    95
                );

                Expect("非任课教师不能录入成绩", !wrongTeacher.Success);

                ServiceResult scoreOk = await scoreService.RecordScoreAsync(
                    "T001",
                    "C101",
                    winnerSno,
                    95
                );

                Expect("任课教师可以给中签学生录入成绩", scoreOk.Success);

                ScoreStatistics statistics = await scoreService.GetScoreStatisticsAsync(
                    "T001",
                    "C101"
                );

                Expect("成绩统计总人数正确", statistics.TotalCount == 1);
                Expect("优秀人数正确", statistics.ExcellentCount == 1);
                Expect("及格率正确", statistics.PassRate == 100);

                DataTable scoreTable = await studentCourseService.GetMyScoresAsync(winnerSno);

                bool hasC101Score = scoreTable
                    .Rows.Cast<DataRow>()
                    .Any(row =>
                        row["CNO"].ToString() == "C101" && Convert.ToInt32(row["GRADE"]) == 95
                    );

                Expect("学生能查询到自己的 C101 成绩", hasC101Score);
            }
        );

        await RunAsync(
            "CoursewareService 课件测试",
            async () =>
            {
                ServiceResult uploadOk = await coursewareService.AddCoursewareAsync(
                    "T001",
                    "C101",
                    "chapter01.pdf",
                    "/fake/path/chapter01.pdf"
                );

                Expect("任课教师可以上传课件", uploadOk.Success);

                ServiceResult uploadWrongTeacher = await coursewareService.AddCoursewareAsync(
                    "T002",
                    "C101",
                    "bad.pdf",
                    "/fake/path/bad.pdf"
                );

                Expect("其他教师不能给这门课上传课件", !uploadWrongTeacher.Success);

                var coursewares = await coursewareService.GetCoursewaresByCourseAsync("C101");

                Expect(
                    "能查到课程课件",
                    coursewares.Count == 1 && coursewares[0].FILENAME == "chapter01.pdf"
                );
            }
        );

        await RunAsync(
            "MessageService 留言测试",
            async () =>
            {
                ServiceResult emptyMessage = await messageService.SendMessageAsync(
                    "U_STU1",
                    "U_TEA1",
                    "   "
                );

                Expect("空留言不能发送", !emptyMessage.Success);

                ServiceResult sendOk = await messageService.SendMessageAsync(
                    "U_STU1",
                    "U_TEA1",
                    "老师您好，我想咨询一下课程安排。"
                );

                Expect("正常留言可以发送", sendOk.Success);

                DataTable conversation = await messageService.GetConversationAsync(
                    "U_STU1",
                    "U_TEA1"
                );

                Expect("双方会话能查到", conversation.Rows.Count == 1);

                int messageId = Convert.ToInt32(conversation.Rows[0]["ID"]);

                ServiceResult readOk = await messageService.MarkAsReadAsync(messageId);

                Expect("留言可以标记已读", readOk.Success);

                DataTable messages = await messageService.GetMessagesForUserAsync("U_TEA1");

                Expect("用户消息列表能查到留言", messages.Rows.Count >= 1);
            }
        );
    }

    private static async Task CreateSchemaAsync(Database database)
    {
        string schema = """
            CREATE TABLE [College]
            (
                [COLLEGEID] TEXT PRIMARY KEY,
                [COLLEGENAME] TEXT NOT NULL
            );

            CREATE TABLE [Student]
            (
                [SNO] TEXT PRIMARY KEY,
                [SNAME] TEXT NOT NULL,
                [SSEX] TEXT NOT NULL,
                [SAGE] INTEGER NOT NULL,
                [SDEPT] TEXT NOT NULL,
                [COLLEGEID] TEXT NOT NULL,
                FOREIGN KEY ([COLLEGEID]) REFERENCES [College]([COLLEGEID])
            );

            CREATE TABLE [Teacher]
            (
                [TNO] TEXT PRIMARY KEY,
                [TNAME] TEXT NOT NULL,
                [TSEX] TEXT NOT NULL,
                [TDEPT] TEXT NOT NULL,
                [COLLEGEID] TEXT NOT NULL,
                FOREIGN KEY ([COLLEGEID]) REFERENCES [College]([COLLEGEID])
            );

            CREATE TABLE [User]
            (
                [USERID] TEXT PRIMARY KEY,
                [ACCOUNT] TEXT NOT NULL UNIQUE,
                [PASSWORDHASH] TEXT NOT NULL,
                [ROLE] INTEGER NOT NULL,
                [RELATEDID] TEXT NULL,
                [COLLEGEID] TEXT NULL
            );

            CREATE TABLE [Course]
            (
                [CNO] TEXT PRIMARY KEY,
                [CNAME] TEXT NOT NULL,
                [CTIME] INTEGER NOT NULL,
                [CCREDIT] REAL NOT NULL,
                [TNO] TEXT NOT NULL,
                [COLLEGEID] TEXT NOT NULL,
                [INTRODUCTION] TEXT NULL,
                [CAPACITY] INTEGER NOT NULL,
                [ISOPEN] INTEGER NOT NULL,
                FOREIGN KEY ([TNO]) REFERENCES [Teacher]([TNO]),
                FOREIGN KEY ([COLLEGEID]) REFERENCES [College]([COLLEGEID])
            );

            CREATE TABLE [CoursePrerequisite]
            (
                [ID] INTEGER PRIMARY KEY AUTOINCREMENT,
                [CNO] TEXT NOT NULL,
                [PREREQUISITECNO] TEXT NOT NULL,
                FOREIGN KEY ([CNO]) REFERENCES [Course]([CNO]),
                FOREIGN KEY ([PREREQUISITECNO]) REFERENCES [Course]([CNO])
            );

            CREATE TABLE [CourseSelection]
            (
                [ID] INTEGER PRIMARY KEY AUTOINCREMENT,
                [SNO] TEXT NOT NULL,
                [CNO] TEXT NOT NULL,
                [STATUS] INTEGER NOT NULL,
                [SELECTEDAT] TEXT NOT NULL,
                FOREIGN KEY ([SNO]) REFERENCES [Student]([SNO]),
                FOREIGN KEY ([CNO]) REFERENCES [Course]([CNO])
            );

            CREATE TABLE [Score]
            (
                [ID] INTEGER PRIMARY KEY AUTOINCREMENT,
                [SNO] TEXT NOT NULL,
                [CNO] TEXT NOT NULL,
                [GRADE] INTEGER NOT NULL,
                [RECORDEDAT] TEXT NOT NULL,
                FOREIGN KEY ([SNO]) REFERENCES [Student]([SNO]),
                FOREIGN KEY ([CNO]) REFERENCES [Course]([CNO])
            );

            CREATE TABLE [Courseware]
            (
                [ID] INTEGER PRIMARY KEY AUTOINCREMENT,
                [CNO] TEXT NOT NULL,
                [FILENAME] TEXT NOT NULL,
                [FILEPATH] TEXT NOT NULL,
                [UPLOADEDBY] TEXT NOT NULL,
                [UPLOADEDAT] TEXT NOT NULL,
                FOREIGN KEY ([CNO]) REFERENCES [Course]([CNO])
            );

            CREATE TABLE [Message]
            (
                [ID] INTEGER PRIMARY KEY AUTOINCREMENT,
                [FROMUSERID] TEXT NOT NULL,
                [TOUSERID] TEXT NOT NULL,
                [CONTENT] TEXT NOT NULL,
                [CREATEDAT] TEXT NOT NULL,
                [ISREAD] INTEGER NOT NULL
            );
            """;

        await ExecuteScriptAsync(database, schema);
    }

    private static async Task SeedDataAsync(Database database)
    {
        string studentPasswordHash = PasswordHasher.HashPassword("123456");
        string teacherPasswordHash = PasswordHasher.HashPassword("123456");
        string adminPasswordHash = PasswordHasher.HashPassword("123456");

        string seed = $"""
            INSERT INTO [College] ([COLLEGEID], [COLLEGENAME]) VALUES
            ('COL01', '计算机学院'),
            ('COL02', '数学学院');

            INSERT INTO [Student] ([SNO], [SNAME], [SSEX], [SAGE], [SDEPT], [COLLEGEID]) VALUES
            ('S001', '张三', '男', 20, '软件工程', 'COL01'),
            ('S002', '李四', '女', 21, '计算机科学与技术', 'COL01');

            INSERT INTO [Teacher] ([TNO], [TNAME], [TSEX], [TDEPT], [COLLEGEID]) VALUES
            ('T001', '王老师', '男', '软件工程系', 'COL01'),
            ('T002', '赵老师', '女', '计算机系', 'COL01'),
            ('T003', '钱老师', '男', '数学系', 'COL02');

            INSERT INTO [User] ([USERID], [ACCOUNT], [PASSWORDHASH], [ROLE], [RELATEDID], [COLLEGEID]) VALUES
            ('U_STU1', 'student1', '{studentPasswordHash}', {(int)
                UserRole.Student}, 'S001', 'COL01'),
            ('U_STU2', 'student2', '{studentPasswordHash}', {(int)
                UserRole.Student}, 'S002', 'COL01'),
            ('U_TEA1', 'teacher1', '{teacherPasswordHash}', {(int)
                UserRole.Teacher}, 'T001', 'COL01'),
            ('U_TEA2', 'teacher2', '{teacherPasswordHash}', {(int)
                UserRole.Teacher}, 'T002', 'COL01'),
            ('U_ADMIN1', 'admin1', '{adminPasswordHash}', {(int)
                UserRole.CollegeAdmin}, NULL, 'COL01');

            INSERT INTO [Course]
            ([CNO], [CNAME], [CTIME], [CCREDIT], [TNO], [COLLEGEID], [INTRODUCTION], [CAPACITY], [ISOPEN])
            VALUES
            ('C100', '程序设计基础', 48, 3.0, 'T001', 'COL01', '基础课', 10, 1),
            ('C101', '数据库系统', 48, 3.0, 'T001', 'COL01', '数据库课程', 1, 1),
            ('C200', '高级数据库', 32, 2.0, 'T001', 'COL01', '需要先修课', 2, 1),
            ('C300', '计算机网络', 48, 3.0, 'T002', 'COL01', '网络课程', 2, 1),
            ('C400', '关闭的课程', 32, 2.0, 'T001', 'COL01', '暂不开放', 2, 0);

            INSERT INTO [CoursePrerequisite] ([CNO], [PREREQUISITECNO]) VALUES
            ('C200', 'C100');

            INSERT INTO [Score] ([SNO], [CNO], [GRADE], [RECORDEDAT]) VALUES
            ('S001', 'C100', 88, '2026-01-01 10:00:00'),
            ('S002', 'C100', 50, '2026-01-01 10:00:00');
            """;

        await ExecuteScriptAsync(database, seed);
    }

    private static async Task ExecuteScriptAsync(Database database, string script)
    {
        string[] statements = script.Split(
            ';',
            StringSplitOptions.RemoveEmptyEntries | StringSplitOptions.TrimEntries
        );

        foreach (string statement in statements)
        {
            if (string.IsNullOrWhiteSpace(statement))
            {
                continue;
            }

            await database.ExecuteNonQueryAsync(statement);
        }
    }

    private static async Task RunAsync(string title, Func<Task> test)
    {
        Console.WriteLine($"\n--- {title} ---");

        try
        {
            await test();
        }
        catch (Exception error)
        {
            Fail($"{title} 抛异常: {error.GetType().Name}: {error.Message}");
        }
    }

    private static void Expect(string name, bool condition)
    {
        if (condition)
        {
            Pass(name);
        }
        else
        {
            Fail(name);
        }
    }

    private static void Pass(string name)
    {
        _passed++;
        Console.WriteLine($"[PASS] {name}");
    }

    private static void Fail(string name)
    {
        _failed++;
        Console.WriteLine($"[FAIL] {name}");
    }

    private static int GetStatusCount(DataTable table, SelectionStatus status)
    {
        foreach (DataRow row in table.Rows)
        {
            if (Convert.ToInt32(row["STATUS"]) == (int)status)
            {
                return Convert.ToInt32(row["Total"]);
            }
        }

        return 0;
    }
}
