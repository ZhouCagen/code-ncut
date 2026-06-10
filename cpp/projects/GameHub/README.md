# GameHub 游戏大厅

GameHub 是一个跨平台的游戏大厅系统，支持网页、移动端 App 和小程序。  
提供多人游戏房间、匹配、聊天，以及整合多款游戏的统一界面。  

GameHub is a cross-platform game lobby system that supports web, mobile apps, and mini-programs.  
It provides multiplayer game rooms, matchmaking, chat, and an integrated UI for multiple games.

---

## 🚀 功能 Features
- **跨平台前端 Cross-platform Frontend**
  - 网页 Web (React/Vue)
  - 移动 App (React Native / Flutter)
  - 小程序 Mini-program
- **高性能后端 High-performance Backend**
  - Go 编写（主服务器）Written in Go (main server)
  - 匹配、聊天、游戏房间管理 Matchmaking, chat, room management
- **游戏模块 Game Modules**
  - C++ 开发 Developed in C++
  - 模块化设计，方便扩展 Modular design for easy extension
- **数据库 Database**
  - MySQL 持久化存储 Persistent storage
  - 实时数据优化查询 Optimized for real-time queries
- **部署 Deployment**
  - Docker 支持，易于扩展 Docker-ready for scaling

---

## 📂 项目结构 Project Structure

```plaintext
GameHub/
├── backend/         # Go 后端服务器 Go backend server
├── frontend/        # Web/App/小程序 Web/App/Mini-program
├── game/            # C++ 游戏逻辑及资源 C++ game logic & assets
├── database/        # SQL 建表 & 测试数据 SQL schema & seed data
├── scripts/         # 构建和部署脚本 Build & deployment scripts
├── docs/            # 文档 Documentation
└── README.md
```

## 🛠 技术栈 Tech Stack

- 后端 Backend: Go (Gin / Fiber)  
- 前端 Frontend: React / Vue / React Native / Flutter  
- 游戏引擎 Game Engine: C++ (Custom)  
- 数据库 Database: MySQL  
- 开发工具 Dev Tools: VS Code, Git, Docker


## 📦 快速开始 Setup & Run

### 1. 克隆仓库 Clone the Repository
```bash
git clone https://github.com/yourusername/GameHub.git
cd GameHub
```

### 2. 启动后端 Start the Backend Server
```bash
cd backend
go mod init gamehub
go run main.go
```

### 3. 启动前端（以 Web 为例） Run the Frontend (Example: Web)
```bash
cd frontend/web
npm install
npm run dev
```

---

## 📄 许可证 License

MIT License. 详见 LICENSE 文件。
MIT License. See LICENSE for details.
