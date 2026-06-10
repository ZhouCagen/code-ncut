using System;
using System.Data;
using System.Windows;
using System.Windows.Controls; // ⚠️必须引用这个，不然 DataRowView 会报错

namespace CourseSelectionSystem_UI
{
    public partial class MainWindow : Window
    {
        private readonly string _userId;
        private readonly bool _isStudent;

        public MainWindow()
        {
            InitializeComponent();
            _userId = "TEST_USER";
            _isStudent = true;
            TxtWelcome.Text = "调试模式";
            LoadCourses();
        }

        public MainWindow(string userId, string realName, bool isStudent)
        {
            InitializeComponent();

            _userId = userId;
            _isStudent = isStudent;

            TxtWelcome.Text = $"{realName} ({(isStudent ? "学生" : "教师")})";

            LoadCourses();
            if (_isStudent) LoadScores();
        }

        private void BtnRefreshCourse_Click(object sender, RoutedEventArgs e)
        {
            LoadCourses();
        }

        private async void BtnSelectCourse_Click(object sender, RoutedEventArgs e)
        {
            if (GridCourses.SelectedItem == null)
            {
                MessageBox.Show("请先在表格里点击选中一门课程！");
                return;
            }

            try
            {
                var selectedRow = GridCourses.SelectedItem as DataRowView;

                if (selectedRow == null) return;

                string courseId = selectedRow["CNO"].ToString()!;
                string courseName = selectedRow["CNAME"].ToString()!;

                var result = MessageBox.Show($"确定要选择【{courseName}】吗？", "选课确认",
                                             MessageBoxButton.YesNo, MessageBoxImage.Question);

                if (result != MessageBoxResult.Yes) return;

                var scoreRepo = new ScoreRepository(App.AppDatabase);
                bool isSuccess = await scoreRepo.SelectCourseAsync(_userId, courseId);

                if (isSuccess)
                {
                    MessageBox.Show($"选课成功！\n你已加入 {courseName}", "成功");
                    LoadCourses();
                    LoadScores();
                }
                else
                {
                    MessageBox.Show("选课失败！\n可能原因：你已经选过这门课了。", "提示");
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("系统错误：" + ex.Message);
            }
        }
        private async void BtnWithdraw_Click(object sender, RoutedEventArgs e)
        {
            if (GridScores.SelectedItem == null)
            {
                MessageBox.Show("请先在表格里选中一门要退选的课程！", "提示");
                return;
            }

            try
            {
                var row = GridScores.SelectedItem as DataRowView;
                if (row == null) return;

                string cno = row["CNO"].ToString()!;
                string cname = row["CNAME"].ToString()!;

                int grade = Convert.ToInt32(row["GRADE"]);
                if (grade > 0) 
                {
                    MessageBox.Show("这门课已经出成绩了，不能退选！"); 
                    return; 
                }

                var result = MessageBox.Show($"确定要退选【{cname}】吗？\n退选后，名额将释放。", "退课确认",
                                             MessageBoxButton.YesNo, MessageBoxImage.Warning);

                if (result != MessageBoxResult.Yes) return;

                var scoreRepo = new ScoreRepository(App.AppDatabase);

                int rowsAffected = await scoreRepo.DeleteScoreAsync(_userId, cno);

                if (rowsAffected > 0)
                {
                    MessageBox.Show("退课成功！", "提示");

                    LoadScores();  
                    LoadCourses(); 
                }
                else
                {
                    MessageBox.Show("退课失败，可能是数据库没删掉。", "错误");
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("系统错误：" + ex.Message);
            }
        }

        private void BtnRefreshScore_Click(object sender, RoutedEventArgs e)
        {
            LoadScores();
        }


        private async void LoadCourses()
        {
            try
            {
                var courseRepo = new CourseRepository(App.AppDatabase);
                DataTable dt = await courseRepo.GetCourseDataAsync();
                GridCourses.ItemsSource = dt.DefaultView;
            }
            catch (System.Exception ex)
            {
                MessageBox.Show("加载课程失败: " + ex.Message);
            }
        }

        private async void LoadScores()
        {
            if (!_isStudent) return;

            try
            {
                var scoreRepo = new ScoreRepository(App.AppDatabase);

                DataTable dt = await scoreRepo.GetMyScoresAsync(_userId);

                GridScores.ItemsSource = dt.DefaultView;
            }
            catch (System.Exception ex)
            {
                MessageBox.Show("加载成绩失败: " + ex.Message);
            }
        }
    }
}