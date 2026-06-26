var builder = WebApplication.CreateBuilder(args);

// ==========================================
// ⬇️ 这里是你插入的代码 ⬇️
// ==========================================
builder.Services.AddSingleton<Database>(sp =>
{
    // 你刚才提供的连接字符串
    string connString = "Server=.;Database=DatabasePrinciplesXSCJ;Trusted_Connection=True;TrustServerCertificate=True;";
    return new Database(connString);
});
// ==========================================

// Add services to the container.
builder.Services.AddControllersWithViews();

var app = builder.Build(); // 🚧 分界线，必须写在这一行之前！

// Configure the HTTP request pipeline.
if (!app.Environment.IsDevelopment())
{
    app.UseExceptionHandler("/Home/Error");
    // The default HSTS value is 30 days. You may want to change this for production scenarios, see https://aka.ms/aspnetcore-hsts.
    app.UseHsts();
}

app.UseHttpsRedirection();
app.UseRouting();

app.UseAuthorization();

app.MapStaticAssets();

app.MapControllerRoute(
    name: "default",
    pattern: "{controller=Home}/{action=Login}/{id?}")
    .WithStaticAssets();

app.Run();