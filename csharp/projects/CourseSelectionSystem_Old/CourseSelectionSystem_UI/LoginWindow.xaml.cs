using System.Windows;

namespace CourseSelectionSystem_UI
{
    public partial class LoginWindow : Window
    {
        public LoginWindow()
        {
            InitializeComponent();
        }

        private void BtnDevMode_Click(object sender, RoutedEventArgs e)
        {
            var hwWindow = new HomeworkWindow();
            hwWindow.Show();
        }
        private async void BtnLogin_Click(object sender, RoutedEventArgs e)
        {
            string userId = TxtUserId.Text.Trim();
            if (string.IsNullOrEmpty(userId))
            {
                MessageBox.Show("请输入学号或工号！", "提示");
                return;
            }

            BtnLogin.IsEnabled = false;
            BtnLogin.Content = "正在登陆...";

            try
            {
                bool loginSuccess = false;
                string userName = "";

                if (RbStudent.IsChecked == true)
                {
                    var studentRepo = new StudentRepository(App.AppDatabase);
                    var student = await studentRepo.GetByIdAsync(userId);

                    if (student != null)
                    {
                        loginSuccess = true;
                        userName = student.SNAME; 
                    }
                }
                else
                {
                    var teacherRepo = new TeacherRepository(App.AppDatabase);
                    var teacher = await teacherRepo.GetByIdAsync(userId);

                    if (teacher != null)
                    {
                        loginSuccess = true;
                        userName = teacher.TNAME;
                    }
                }

                if (loginSuccess)
                {
                    string currentId = TxtUserId.Text.Trim();
                    bool isStudent = RbStudent.IsChecked == true;


                    if (isStudent)
                    {
                        var mainWindow = new MainWindow(currentId, userName, true);
                        mainWindow.Show();
                    }
                    else
                    {
                        var teacherWindow = new TeacherWindow(currentId, userName);
                        teacherWindow.Show();
                    }

                    this.Close();
                }
                else
                {
                    MessageBox.Show("用户不存在，请检查ID是否正确。", "登录失败");
                }
            }
            catch (System.Exception ex)
            {
                MessageBox.Show($"数据库连接出错: {ex.Message}", "错误");
            }
            finally
            {
                BtnLogin.IsEnabled = true;
                BtnLogin.Content = "登 录";
            }

        }
    }
}
