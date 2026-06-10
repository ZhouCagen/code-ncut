using System;
using System.Data;
using System.Windows;

namespace CourseSelectionSystem_UI
{
    public partial class HomeworkWindow : Window
    {
        private readonly Database _db = App.AppDatabase;

        public HomeworkWindow()
        {
            InitializeComponent();
        }

        private void ShowData(DataTable dt)
        {
            if (dt == null || dt.Rows.Count == 0)
            {
                MessageBox.Show("查询成功，但结果为空。");
                GridResults.ItemsSource = null;
            }
            else
            {
                GridResults.ItemsSource = dt.DefaultView;
            }
        }

        private (int p1, int p2) GetParams()
        {
            int.TryParse(TxtParam1.Text, out int p1);
            int.TryParse(TxtParam2.Text, out int p2);
            return (p1, p2);
        }

        private async void BtnTask1_Click(object sender, RoutedEventArgs e)
        {
            var repo = new StudentRepository(_db);
            ShowData(await repo.GetStudentBirthYearAsync());
        }

        private async void BtnTask2_Click(object sender, RoutedEventArgs e)
        {
            var repo = new StudentRepository(_db);
            ShowData(await repo.GetDeptCountAsync());
        }

        private async void BtnTask3_Click(object sender, RoutedEventArgs e)
        {
            var repo = new ScoreRepository(_db);
            ShowData(await repo.GetSpecificCourseScoreAsync());
        }

        private async void BtnTask4_Click(object sender, RoutedEventArgs e)
        {
            var repo = new TeacherRepository(_db);
            ShowData(await repo.GetPopularTeachersAsync());
        }

        private async void BtnTask5_Click(object sender, RoutedEventArgs e)
        {
            var repo = new StudentRepository(_db);
            ShowData(await repo.GetStudentsNoCourseAsync());
        }

        private async void BtnTask6_Click(object sender, RoutedEventArgs e)
        {
            var repo = new ScoreRepository(_db);
            ShowData(await repo.GetMaxGradePerStudentAsync());
        }

        private async void BtnTask7_Click(object sender, RoutedEventArgs e)
        {
            var repo = new TeacherRepository(_db);
            ShowData(await repo.GetTeachersByCourseAsync("数据结构"));
        }

        private async void BtnTask8_Click(object sender, RoutedEventArgs e)
        {
            var repo = new StudentRepository(_db);
            ShowData(await repo.GetStudentsByTeacherNameAsync("郑大力"));
        }


        private async void BtnTask9_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                var repo = new StudentRepository(_db);
                await repo.CreateS1TableAsync();
                MessageBox.Show("成功！表 S1 已创建，并插入了计算机系学生。\n请去数据库检查，或者写个查询来看看。", "操作成功");
            }
            catch (Exception ex) { MessageBox.Show("Error: " + ex.Message); }
        }

        private async void BtnTask10_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                var repo = new ScoreRepository(_db);
                await repo.UpdateScoreViaViewAsync();
                MessageBox.Show("成功！视图 STUDENT_CS 已建立，且 CS101 课程成绩已 +5 分。", "操作成功");
            }
            catch (Exception ex) { MessageBox.Show("Error: " + ex.Message); }
        }

        private async void BtnTask11_Click(object sender, RoutedEventArgs e)
        {
            var (low, high) = GetParams();
            try
            {
                var repo = new ScoreRepository(_db);
                ShowData(await repo.ProcedureGetScoreAsync(low, high));
            }
            catch (Exception ex) { MessageBox.Show("存储过程执行失败: " + ex.Message); }
        }

        private async void BtnTask12_Click(object sender, RoutedEventArgs e)
        {
            var (low, high) = GetParams();
            var result = MessageBox.Show($"警告：这将真正删除 {low}-{high} 分之间的数据结构成绩！\n确定要继续吗？", "删除确认", MessageBoxButton.YesNo, MessageBoxImage.Warning);

            if (result == MessageBoxResult.Yes)
            {
                try
                {
                    var repo = new ScoreRepository(_db);
                    ShowData(await repo.ProcedureDeleteScoreAsync(low, high));
                }
                catch (Exception ex) { MessageBox.Show("删除失败: " + ex.Message); }
            }
        }

        private async void BtnTask13_Click(object sender, RoutedEventArgs e)
        {
            var (minCredit, _) = GetParams();
            try
            {
                var repo = new CourseRepository(_db);
                ShowData(await repo.GetCoursesByCreditAsync(minCredit));
            }
            catch (Exception ex) { MessageBox.Show("查询失败: " + ex.Message); }
        }
    }
}