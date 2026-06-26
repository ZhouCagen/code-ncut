PRAGMA foreign_keys = ON;

INSERT INTO [College] ([COLLEGEID], [COLLEGENAME])
VALUES
('CS', '计算机学院'),
('MATH', '数学学院');

INSERT INTO [Student] ([SNO], [SNAME], [SSEX], [SAGE], [SDEPT], [COLLEGEID])
VALUES
('2023001', '张三', '男', 20, '计算机系', 'CS'),
('2023002', '李四', '女', 21, '计算机系', 'CS'),
('2023003', '王五', '男', 20, '软件工程系', 'CS');

INSERT INTO [Teacher] ([TNO], [TNAME], [TSEX], [TDEPT], [COLLEGEID])
VALUES
('T001', '刘老师', '男', '计算机系', 'CS'),
('T002', '陈老师', '女', '软件工程系', 'CS');

INSERT INTO [User] ([USERID], [ACCOUNT], [PASSWORDHASH], [ROLE], [RELATEDID], [COLLEGEID])
VALUES
('U_SYS', 'admin', '123456', 1, NULL, NULL),
('U_COLLEGE_CS', 'csadmin', '123456', 2, NULL, 'CS'),
('U_T001', 'T001', '123456', 3, 'T001', 'CS'),
('U_T002', 'T002', '123456', 3, 'T002', 'CS'),
('U_2023001', '2023001', '123456', 4, '2023001', 'CS'),
('U_2023002', '2023002', '123456', 4, '2023002', 'CS'),
('U_2023003', '2023003', '123456', 4, '2023003', 'CS');

INSERT INTO [Course]
(
    [CNO],
    [CNAME],
    [CTIME],
    [CCREDIT],
    [TNO],
    [COLLEGEID],
    [INTRODUCTION],
    [CAPACITY],
    [ISOPEN]
)
VALUES
('CS101', 'C#程序设计', 48, 3.00, 'T001', 'CS', '学习 C# 基础语法和 ASP.NET Core 开发。', 2, 1),
('CS102', '数据库原理', 56, 3.50, 'T001', 'CS', '学习关系数据库、SQL 和数据库设计。', 2, 1),
('CS201', '高级数据库', 48, 3.00, 'T002', 'CS', '学习高级数据库系统。', 1, 1);

INSERT INTO [CoursePrerequisite] ([CNO], [PREREQUISITECNO])
VALUES
('CS201', 'CS102');

INSERT INTO [Score] ([SNO], [CNO], [GRADE], [RECORDEDAT])
VALUES
('2023001', 'CS102', 85, datetime('now'));

INSERT INTO [CourseSelection] ([SNO], [CNO], [STATUS], [SELECTEDAT])
VALUES
('2023001', 'CS101', 1, datetime('now')),
('2023002', 'CS101', 1, datetime('now')),
('2023003', 'CS101', 0, datetime('now'));

INSERT INTO [Courseware] ([CNO], [FILENAME], [FILEPATH], [UPLOADEDBY], [UPLOADEDAT])
VALUES
('CS101', '第一章课件.pdf', '/uploads/coursewares/CS101/chapter1.pdf', 'T001', datetime('now'));

INSERT INTO [Message] ([FROMUSERID], [TOUSERID], [CONTENT], [CREATEDAT], [ISREAD])
VALUES
('U_T001', 'U_2023001', '请按时查看课程公告。', datetime('now'), 0);
