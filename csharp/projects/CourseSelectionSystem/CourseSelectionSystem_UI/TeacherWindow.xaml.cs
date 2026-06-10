using System;
using System.Data;
using System.Windows;
using System.Windows.Controls;
using Microsoft.Data.SqlClient;

namespace CourseSelectionSystem_UI
{
    public partial class TeacherWindow : Window
    {
        private readonly string _teacherId;
        private string _myCourseId = ""; 

        public TeacherWindow(string teacherId, string teacherName)
        {
            InitializeComponent();
            _teacherId = teacherId;
            TxtTeacherInfo.Text = $"{teacherName} (教师)";

            LoadMyClass();
        }

        private async void LoadMyClass()
        {
            try
            {
                string sqlTeacher = "SELECT CNO FROM Teacher WHERE TNO = @Tno";
                var database = App.AppDatabase;

                DataTable dataTeacher = await database.ExecuteQueryAsync(sqlTeacher, new[] { new SqlParameter("@Tno", _teacherId) });

                if (dataTeacher.Rows.Count == 0 || dataTeacher.Rows[0]["CNO"] == DBNull.Value)
                {
                    MessageBox.Show("系统检测到您当前没有负责任何课程。");
                    return;
                }

                _myCourseId = dataTeacher.Rows[0]["CNO"].ToString()!;

                await RefreshStudentList();
            }
            catch (Exception ex)
            {
                MessageBox.Show("加载失败：" + ex.Message);
            }
        }

        private async Task RefreshStudentList()
        {
            string sqlStudents = @"
                    SELECT Student.SNO, Student.SNAME, Score.GRADE 
                    FROM Score
                    JOIN Student ON Score.SNO = Student.SNO
                    WHERE Score.CNO = @Cno";

            var database = App.AppDatabase;
            DataTable dtStudents = await database.ExecuteQueryAsync(sqlStudents, new[] { new SqlParameter("@Cno", _myCourseId) });

            GridStudents.ItemsSource = dtStudents.DefaultView;
        }

        private void GridStudents_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (GridStudents.SelectedItem is DataRowView row)
            {
                string sname = row["SNAME"].ToString()!;
                string grade = row["GRADE"].ToString()!;

                TxtSelectedStudent.Text = sname;
                TxtScoreInput.Text = grade; 
            }
        }
        private async void BtnSaveScore_Click(object sender, RoutedEventArgs e)
        {
            if (GridStudents.SelectedItem == null)
            {
                MessageBox.Show("请先在左侧选择一个学生！");
                return;
            }

            if (!int.TryParse(TxtScoreInput.Text, out int newScore))
            {
                MessageBox.Show("请输入正确的数字成绩！");
                return;
            }

            if (newScore < 0 || newScore > 100)
            {
                MessageBox.Show("成绩必须在 0 - 100 之间！");
                return;
            }

            try
            {
                var row = GridStudents.SelectedItem as DataRowView;
                string studentId = row!["SNO"].ToString()!;

                var scoreRepository = new ScoreRepository(App.AppDatabase);

                var scoreToUpdate = new Score
                {
                    SNO = studentId,
                    CNO = _myCourseId,
                    GRADE = newScore
                };

                await scoreRepository.UpdateScoreAsync(scoreToUpdate);

                MessageBox.Show("成绩保存成功！");

                await RefreshStudentList();
            }
            catch (Exception ex)
            {
                MessageBox.Show("保存失败：" + ex.Message);
            }
        }
    }
}