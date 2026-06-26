using CourseSelectionSystem.Core.Data;
using CourseSelectionSystem.Core.Repositories;
using CourseSelectionSystem.Core.Services;

var builder = WebApplication.CreateBuilder(args);

builder.Services.AddControllersWithViews();

builder.Services.AddDistributedMemoryCache();

builder.Services.AddSession(options =>
{
    options.IdleTimeout = TimeSpan.FromHours(2);
    options.Cookie.HttpOnly = true;
    options.Cookie.IsEssential = true;
});

string databasePath = Path.GetFullPath(
    Path.Combine(builder.Environment.ContentRootPath, "..", "course_selection.db")
);

builder.Services.AddSingleton(new Database($"Data Source={databasePath}"));

// Repositories
builder.Services.AddScoped<UserRepository>();
builder.Services.AddScoped<StudentRepository>();
builder.Services.AddScoped<TeacherRepository>();
builder.Services.AddScoped<CourseRepository>();
builder.Services.AddScoped<CoursePrerequisiteRepository>();
builder.Services.AddScoped<CourseSelectionRepository>();
builder.Services.AddScoped<ScoreRepository>();
builder.Services.AddScoped<CoursewareRepository>();
builder.Services.AddScoped<MessageRepository>();

// Services
builder.Services.AddScoped<AuthService>();
builder.Services.AddScoped<StudentCourseService>();
builder.Services.AddScoped<TeacherCourseService>();
builder.Services.AddScoped<LotteryService>();
builder.Services.AddScoped<ScoreService>();
builder.Services.AddScoped<CoursewareService>();
builder.Services.AddScoped<MessageService>();

var app = builder.Build();

Console.WriteLine($"SQLite database: {databasePath}");

if (!app.Environment.IsDevelopment())
{
    app.UseExceptionHandler("/Home/Error");
}

app.UseStaticFiles();

app.UseRouting();

app.UseSession();

app.MapControllerRoute(name: "default", pattern: "{controller=Account}/{action=Login}/{id?}");

app.Run();
