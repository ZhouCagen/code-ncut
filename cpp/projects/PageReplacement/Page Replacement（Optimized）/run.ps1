# 1. 如果 bin 文件夹不存在，自动创建一个
if (!(Test-Path -Path "bin")) {
    New-Item -ItemType Directory -Path "bin" | Out-Null
}

Write-Host "Compiling..." -ForegroundColor Cyan

g++ -std=c++23 src/main.cpp -o bin/run.exe

# 3. 判断是否编译成功
if ($LASTEXITCODE -eq 0) 
{
    Write-Host "Compilation Successful! Running..." -ForegroundColor Green
    Write-Host "----------------------------------"
    # 运行生成的程序
    ./bin/run.exe
} else 
{
    Write-Host "Compile Failed! Check your code." -ForegroundColor Red
}