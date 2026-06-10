using System.Windows;
using Microsoft.Data.SqlClient;

namespace CourseSelectionSystem_UI;
public partial class App : Application
{
    public static Database AppDatabase { get; private set; } = null!;

    protected override void OnStartup(StartupEventArgs e)
    {
        base.OnStartup(e);

        string connectionString = "Server=.;Database=DatabasePrinciplesXSCJ;Trusted_Connection=True;TrustServerCertificate=True;";

        AppDatabase = new Database(connectionString);
    }
}

