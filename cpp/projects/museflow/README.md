# MuseFlow

运行在 Linux 上、本地优先的 AI 场景配乐工作台。C++20 后端 + Vue 3 / TypeScript 前端。

**当前为第四阶段 v0.4.0 桌面版：Qt 6 独立窗口，支持 DeepSeek / Lyria、严格时长、版本树与 A/B 对比。** 默认 Mock 无需 API Key。真实 Provider 已通过本地契约服务测试，尚未使用真实账户调用 Google 或 DeepSeek。

## 使用 ElevenLabs Music

在“模型设置 → 音乐生成服务商”选择 **ElevenLabs Music**，填写 API Key，点击“保存并测试密钥”。测试使用 [音乐编排接口](https://elevenlabs.io/docs/api-reference/music/create-composition-plan)，不生成音频、不消耗音乐积分，不依赖 `user_read` 权限；它不保证剩余额度足够生成音频。Key 应启用 Music 权限，实际生成需要付费套餐。

点击“生成测试音乐”创建独立的 30 秒试听项目，也可以在创作台生成试听或完整歌曲。按 [官方音乐接口](https://elevenlabs.io/docs/api-reference/music/compose)使用 `POST /v1/music`、`xi-api-key` 认证和已实测的 `music_v1` 模型，发送提示词、毫秒时长及纯音乐选项。歌词和时间轴通过提示词引导，不保证逐段精确执行；提示词（含歌词与结构）最多 4100 字符，超限明确报错、不截断。

源音频为 MP3，由现有 FFmpeg 流程转换为所选 MP3/WAV；严格时长输出 WAV。生成在后台执行，失败不自动重新提交收费请求。Key 只存后端会话内存；各服务商 Key 独立，重启后重新填写，或启动前设置 `ELEVENLABS_API_KEY`。桌面渲染子进程不继承该环境变量。

2026-09-10 已用真实账户验证 3 秒 `music_v1` 音频生成成功（HTTP 200，MP3 可解码），并通过编译后的 C++ 后端验证音乐编排连接测试、5 秒真实生成与严格时长 WAV 保存。本地契约回归使用 `MUSEFLOW_ELEVENLABS_TEST_BASE`，仅接受 IPv4 回环 HTTP 地址；正常运行不要设置此变量。

## 第一阶段已实现

- 场景描述 → 模拟设计稿 → 曲风、情绪、BPM、调性、乐器、人声和歌词编辑。
- 时间轴段落添加、删除、起止时间、情绪、强度、乐器及过渡描述编辑；前后端验证时间重叠、越界、强度及 BPM 等参数。添加段落会优先使用空档，否则平分一个最长段落。
- 本地规则生成带时间描述的提示词预览；保存和生成时重新校验、刷新提示词。
- 独立 `ITextModelProvider` / `IMusicModelProvider` 接口及两个 Mock 实现。
- C++ 后台队列依次经过 `queued → preparing → generating → post_processing → completed / failed`；HTTP 线程不等待音乐生成。前端轮询进度，任务快照独立存储。
- SQLite 项目、设计稿、任务和版本持久化；每次生成保存独立音频及参数快照，不覆盖旧作品。
- 基础项目列表、打开、保存、删除确认；作品库搜索、项目筛选、查看生成参数与提示词。
- HTML5 Audio 播放、由实际 WAV 数据解码得到的波形、点击波形定位、WAV 下载和 HTTP Range。
- 重启后保留已完成任务；中断任务在重启时标为失败并给出重新生成提示。打开项目会恢复任务轮询。
- 中文桌面布局与全部导航路由，开发双服务启动、生产静态资源托管、XDG 目录、单元与集成测试。

## 第二阶段已实现

- 独立 DeepSeek Provider，兼容可配置地址与模型名称的 Chat Completions 服务；真实请求由 C++ libcurl 发出。
- 两张独立设置卡：文本服务商、地址、模型、Key、超时、连接测试与状态；音乐模型偏好及会话 Key 管理。Google 接入见第三阶段。
- `DEEPSEEK_API_KEY` / `GEMINI_API_KEY` 环境变量读取。界面 Key 仅保存在后端内存，公开接口仅返回存在标记与来源。非敏感设置原子写入 XDG 配置目录。
- 独立文本后台队列；连接测试、生成与改稿不占用 HTTP 处理线程。文本队列与音乐队列分别执行。
- 内置 `backend/schema/music-blueprint.schema.json`，校验必填字段、类型、枚举、额外字段、范围与文本长度，再检查时间轴重叠等业务约束。空输出、无效设计稿最多重试一次；网络错误、拒绝、截断不自动重试。
- 自然语言改稿入口，支持让文本模型修改歌词、人声与时间轴；失败时保留现有设计稿。
- AI 提示词与参数绑定：参数未变时原样保存，参数变化时按最新参数转换提示词，防止保存过时内容。
- 中文网络错误分类，响应大小/嵌套深度限制、TLS 校验、禁止重定向及密钥回显过滤。

## 第三阶段已实现与使用 Lyria

在“模型设置”的音乐卡选择 **Google Lyria**，输入 Gemini Key 并保存。Key 测试只查询所选模型信息；“生成测试音乐”使用本地测试设计稿创建独立项目并发起 30 秒音乐任务，会按 Google 规则计费，不额外调用文本模型。创作台也可直接生成试听或完整歌曲。默认试听优先与模型选择控制强调的生成按钮；两个显式入口始终使用各自对应的模型。

按 2026-09-09 核对的 [Google 音乐生成指南](https://ai.google.dev/gemini-api/docs/music-generation?hl=en)实现：C++ 向 `/v1beta/interactions` 发送 `model` / `input`，通过 `x-goog-api-key` 请求头认证。试听使用 `lyria-3-clip-preview`；完整歌曲使用 `lyria-3.5`。完整歌曲请求 WAV 时使用官方 `response_format: {"type":"audio"}`，试听默认返回 MP3，再由本机转换所选格式。

遍历 REST 响应 `steps` 中 `model_output.content` 的文本和音频块，保留歌词与可解析的 JSON 歌曲结构，严格解码最后一个 Base64 音频块。采用 [Interactions 参考文档](https://ai.google.dev/api/interactions-api?hl=en)中的状态及 GET 查询路径；模型元数据检测使用 [Models API](https://ai.google.dev/api/models)。所有生成和音频处理均在后台队列内执行。进度百分比表示本地阶段，Google 不提供此界面的精确生成百分比。

试听将设计稿时间轴按比例缩放到 30 秒，并保存实际发送的提示词 `generationPrompt`；原设计稿保持完整快照。普通模式只能通过提示词影响时长，不能保证模型精确执行。完整歌曲的实际长度受服务商限制与模型输出影响。

**严格时长**使用 WAV / 44.1 kHz / 双声道 / 16-bit PCM，以避免 MP3 编码填充导致容器时长偏差。即使默认格式为 MP3，开启严格时长也保存 WAV，界面会明确提示。按 [FFmpeg 滤镜文档](https://ffmpeg.org/ffmpeg-filters.html)使用淡出、补静音和裁剪；处理后用 ffprobe 检查，误差超过一个采样点则任务失败。补齐增加的是静音，不会续写音乐。裁剪不改变保存的原始音频。若本地转换失败，仍保留 `.source` 文件供恢复，界面不会将失败任务显示为作品。

每个项目保留 `<任务 UUID>.source.wav/mp3` 原始音频和独立成品。界面分别展示模型目标、原始音频与处理后实际时长。歌词和结构随任务及版本保存在 SQLite；Base64 与 Key 不写入数据库。作品库可播放、查看参数/提示词/模型返回文本并按成品格式下载。

`POST /api/providers/music/test` 在真实模式下返回与文本测试一致的 `202 + job`，可通过 `/api/text-jobs/:id` 查询这个轻量连接测试。音乐生成使用原有 `/api/generations` 与 SQLite 队列，状态包含 `queued/preparing/generating/post_processing/completed/failed`。音频接口支持 WAV/MP3、Range 与下载。

网络连接超时 10 秒，生成响应最长等待 600 秒，远端任务查询单次 60 秒，总查询等待最多 15 分钟；ffprobe/FFmpeg 分别限制为 30/120 秒。关闭程序会取消本地等待，**不代表远端已取消或未计费**；不会自动重新提交收费请求。错误不会静默改用 Mock。生成前请以界面显示的服务商为准；尚未实现可靠费用估算。

自动测试使用 `MUSEFLOW_GOOGLE_TEST_BASE` 注入仅限 `http://127.0.0.1:端口` 的本地服务；普通运行不要设置该变量。生产端点固定为 Google 官方域名，前端没有外部 API 地址或请求。

## 第四阶段已实现

- 项目搜索、重命名、任务历史、版本入口，以及设计与记录 JSON 导出。重命名项目不会重命名旧音频版本。
- 从任意已完成版本创建分支草稿；复制设计稿并保存 `draftParentVersionId`，可改稿后生成新子版本。草稿本身不伪装为音频作品。分支可继续产生子分支或同父版本的多个候选。
- 版本树展示父子路径；A/B 可同时载入两个音频、快速切换、保留绝对播放位置、循环当前版本。只有当前一侧播放；短版本会将位置限制到尾部附近。
- 比较实际发送提示词、曲风、BPM、人声、歌词、时间轴、生成模式和格式等差异；忽略随机 UUID 等无音乐意义的字段。
- 每个版本支持 0～5 分评分（0 为未评分）和备注；参数快照不可通过评价接口修改。
- 单作品删除有二次确认，并删除成品与原始音频。保留任务历史且标记音频已删除。有子分支或正在生成的子任务时拒绝删除父版本；可从末端逐层删除，或删除整个项目。
- 导出 JSON 包含项目、设计稿、任务、版本关系和评价，不包含音频、模型设置或 Key。音频请单独下载；当前没有项目导入或音频归档包。

| 新增接口 | 行为 |
| --- | --- |
| `GET /api/activity` | 未完成音乐任务与文本任务数量，供桌面退出确认 |
| `GET /api/versions/:id` | 单个版本及参数快照 |
| `POST /api/versions/:id/branch` | 创建并保存项目分支草稿，返回 `201 + project` |
| `PUT /api/versions/:id/metadata` | 只允许 `rating` / `notes` |
| `DELETE /api/versions/:id` | 删除末端作品，保留任务历史 |
| `GET /api/projects/:id/export` | 下载设计与记录 JSON |

`POST /api/generations` 接受可选 `parentVersionId` 字符串。父版本必须存在且属于同一项目。保存项目时用 `draftParentVersionId` 保存分支来源，设为 `null` 可恢复独立生成。检查和写入在仓库锁内完成，删除与生成不会留下竞态产生的悬空引用。

## 桌面应用

```bash
./scripts/build.sh
./scripts/run.sh
# 注册当前用户的应用菜单图标与命令，无需 sudo
./scripts/install-desktop.sh
```

安装后在应用菜单搜索 **MuseFlow**，或运行 `~/.local/bin/museflow`。安装脚本只创建当前用户的菜单入口、图标和启动脚本，不复制或清空项目数据，也不覆盖非 MuseFlow 管理的同名文件。

桌面程序使用 [Qt WebEngine](https://doc.qt.io/qt-6/qwebengineprofile.html) 的内存会话承载界面，使用 [Qt 下载接口](https://doc.qt.io/qt-6/qwebenginedownloadrequest.html)提供保存对话框。没有浏览器地址栏；无需安装 Chrome 才能运行桌面程序。Chrome 仅用于另行运行的浏览器回归测试。Qt 仍使用内置 Web 渲染引擎，界面不是全部重写为原生 Qt 控件。

重复启动会激活已有窗口。关闭窗口时检查未保存设计稿、下载和后台任务；需要时显示确认。退出会终止该窗口启动的后台进程，正常情况下释放 18080 端口；远端 Google 任务不保证同步取消。已有开发后端占用端口时，桌面程序会提示启动失败，不接管或关闭它。

模型环境变量只传给 C++ 后端，不传给 WebEngine 子进程。窗口页面禁止外部导航和外部请求，音频及 JSON 导出通过保存对话框写到用户选择的位置。菜单注册遵循 [Desktop Entry 规范](https://specifications.freedesktop.org/desktop-entry/latest/exec-variables.html)。

主流发行版依赖可参考 [Arch Qt WebEngine](https://archlinux.org/packages/extra/x86_64/qt6-webengine/)、[Debian Qt WebEngine](https://packages.debian.org/bookworm/qt6-webengine-dev)和 [Ubuntu 24.04 Qt 包](https://packages.ubuntu.com/source/noble/qt6-webengine)。已在当前 Linux / Qt 6.11 环境实测；其他发行版提供构建说明，尚未逐一实机验证。

仅构建服务端时可设置 `cmake -S backend -B build -DMUSEFLOW_BUILD_DESKTOP=OFF`。恢复完整构建使用 `-DMUSEFLOW_BUILD_DESKTOP=ON`；标准 `scripts/build.sh` 会明确启用桌面目标。不要以 root 身份启动图形程序，也不要为日常运行关闭 WebEngine 沙箱。

## 尚未实现

| 阶段 | 后续功能 |
| --- | --- |
| 待后续安排 | 灵感素材管理、Linux Secret Service、AppImage |

“灵感素材”页面明确显示尚未实现。Mock 文本修订仅记录修改要求，不执行智能改写；Mock 音乐原始素材固定为 8 秒，可转换格式和严格时长处理。

“加强副歌”等修改通过修改设计稿、重新生成独立版本实现，不代表能够精确编辑原音频的局部片段。

## 使用 DeepSeek

1. 启动后进入“模型设置”，创作助手服务商选择 DeepSeek / Chat Completions 兼容服务。
2. 填写 API 地址、模型名称及 Key。官方默认地址为 `https://api.deepseek.com`；支持基础地址、带 `/v1` 的地址或完整 `/chat/completions` 地址。模型名称可自由输入。
3. 点击“保存并测试连接”。这会发送一个最小 JSON 文本请求，可能产生少量费用。
4. 回到创作台生成设计稿，或在已有设计稿上填写修改要求。失败会显示中文错误，不会覆盖原有设计稿。

远程地址要求 HTTPS；本机兼容服务可使用 `http://127.0.0.1:端口`。地址不得携带用户名、密码、查询参数或片段。自定义地址应由用户确认可信；点击测试或生成会把场景及认证 Key 发往该地址。

Key 留空时保留当前 Key；“清除”在当前会话清除手输 Key，并停用对应环境变量 Key；“使用环境变量”恢复环境变量来源。重启后手输 Key 消失，环境变量重新读取。程序不自动加载 `.env`。

按 2026-09-09 核对的 [DeepSeek Chat Completions 文档](https://api-docs.deepseek.com/api/create-chat-completion/) 和 [JSON Output 文档](https://api-docs.deepseek.com/guides/json_mode/)实现：使用 `response_format: {"type":"json_object"}`，提示中包含 JSON Schema 和示例；官方地址使用文档支持的非思考模式，自定义兼容服务仅发送通用 Chat Completions 字段。错误分类参考 [官方错误码](https://api-docs.deepseek.com/quick_start/error_codes/)。

JSON Output 本身不等同于严格 Schema 保证，因此后端再次执行校验。项目的 Schema 校验器覆盖当前内置 Schema 使用的封闭关键字集合，未来加入未支持关键字会在启动时失败；它不是面向任意第三方 Schema 的通用验证器。整数参数还要求 JSON 整数字面量。

当前通过本地兼容服务验证了真实 libcurl 调用链与失败分支；**没有使用真实账户 Key 调用 DeepSeek，不宣称真实服务端到端验证已完成**。可在界面自行填写 Key 测试，不需要将 Key 发给开发者。

文本请求连接超时默认 10 秒、响应超时默认 90 秒，可在设置调整。单次输出失败最多重试一次。不会自动重试 401、403、429、网络超时或拒绝，以避免重复计费。

## 第二阶段新增接口

| 接口 | 行为 |
| --- | --- |
| `GET /api/settings` | 公开设置、Key 存在状态与来源，不返回 Key |
| `PUT /api/settings` | 部分更新 `text` / `music`；Key 只进入内存 |
| `POST /api/providers/text/test` | 测试当前已保存文本配置 |
| `POST /api/blueprints/generate` | `userInput` / `purpose` 创建设计稿 |
| `POST /api/blueprints/revise` | `blueprint` / `instruction` 改稿 |
| `GET /api/blueprints/schema` | 当前内置音乐设计稿 Schema |
| `GET /api/text-jobs/:id` | 查询文本后台任务及脱敏结果 |

Mock 请求保持立即返回。真实文本请求返回 HTTP 202 和 `{"job":{"id":"…","status":"queued"}}`，随后轮询文本任务；状态为 `queued/running/completed/failed`，成功结果在 `result`，失败为 `error/code`。文本任务暂存内存，仅保留最近 64 个完成结果；重启后需要重新发起。该队列与音乐生成的 SQLite 任务完全分开。

## Linux 依赖

要求 CMake ≥ 3.20、支持 C++20 的 GCC/Clang、Node.js ≥ 22.12（推荐使用受支持的 LTS）、npm、Python 3、libcurl、SQLite3、nlohmann/json，以及 Qt ≥ 6.4 的 Widgets / Network / WebEngineWidgets。HTTP 框架 cpp-httplib 已按固定版本保存在 `backend/third_party/`。

Arch Linux：

```bash
sudo pacman -S --needed base-devel cmake curl sqlite nlohmann-json nodejs npm python ffmpeg xdg-utils util-linux qt6-base qt6-webengine
```

Debian 12+ / Ubuntu 24.04+（Qt ≥ 6.4）：

```bash
sudo apt update
sudo apt install build-essential cmake libcurl4-openssl-dev libsqlite3-dev nlohmann-json3-dev python3 curl ffmpeg xdg-utils util-linux qt6-base-dev qt6-webengine-dev
```

另外安装符合上述版本要求的 Node.js 和 npm；部分 Debian/Ubuntu 发行版自带的 Node.js 版本较旧。用 `node --version` 检查。FFmpeg/ffprobe 是运行依赖，模拟与真实音乐均使用它们检测时长与转换格式。MP3 输出需要 FFmpeg 包含 libmp3lame 编码器。

## 开发运行

在项目根目录执行：

```bash
./scripts/dev.sh
```

首次运行构建后端并使用 `npm ci` 安装锁定依赖；随后启动：

- C++：`http://127.0.0.1:18080`
- Vue/Vite：`http://127.0.0.1:5173`

浏览器打开 Vue 地址。前端的 `/api` 由 Vite 代理到本地 C++ 服务。按 Ctrl+C 同时关闭两端。如果已有进程占用端口，脚本报错退出，不终止用户的其他进程。

操作示例：输入场景 → 点击“AI 生成音乐设计稿” → 调整参数和时间轴 → 点击“生成 30 秒试听 · 模拟” → 等待约 3.5 秒 → 播放或下载。生成前自动保存项目，完成后自动保存作品。重复生成可在作品库看到多个独立记录。分别展示目标、原始与处理后时长。

## 生产运行

```bash
./scripts/build.sh
./scripts/run.sh
```

`scripts/run.sh` 打开 **MuseFlow 独立桌面窗口**，包含应用图标与菜单，不打开 Chrome 或浏览器标签。Qt WebEngine 在窗口内承载 Vue 界面；桌面程序启动自己的 C++ 后端并验证实例标识。后端始终只绑定 `127.0.0.1:18080`，前端不直接调用外部模型。

只调试 C++ 后端时：

```bash
./build/museflow
```

`--assets DIR` / `--web DIR` 可指定资源和前端构建目录，便于移动运行目录。默认路径在 CMake 配置时确定；移动整个项目后可使用 `scripts/run.sh` 或重新构建。此启动脚本会显式指定当前项目的可执行文件与资源路径；移动项目后需重新安装菜单入口。当前使用发行版 Qt 运行库，尚未提供 AppImage 或系统级安装包。

构建默认使用 2 个并行编译任务；可通过 `MUSEFLOW_BUILD_JOBS=4 ./scripts/build.sh` 调整。

## 测试

先完成前后端构建，并停止占用 18080 端口的开发服务：

```bash
./scripts/test.sh
```

包含 C++ 核心校验、队列/数据库/配置测试、前端 Vitest 校验、真实 HTTP 集成测试、本地 Chat Completions、Google Interactions 与 ElevenLabs Music 契约测试。ElevenLabs 测试覆盖独立 Key、编排连接、毫秒时长、人声参数、MP3/WAV、严格时长、错误分类、禁止重试及 Key 不落盘；浏览器测试覆盖设置、连接、生成与下载。Lyria 测试覆盖 Base64、歌词/结构、MP3/WAV、严格裁剪/补静音/淡出、异步任务与禁止收费请求自动重试。验证创建/改稿/歌词、有限重试、网络错误、Key 不落盘/不回传、重启消失、环境变量与代理等分支。全部使用临时 XDG 目录和假 Key，不修改用户项目，不调用收费模型。

使用**已安装的 Google Chrome**运行浏览器测试，无需下载 Playwright Chromium：

```bash
./scripts/test.sh --e2e
# 或只运行浏览器测试
cd frontend
npm run test:e2e
```

Playwright 使用 `channel: 'chrome'` 和独立临时浏览器资料目录，不使用个人 Chrome 配置。Chrome 在非标准位置时，可指定 `MUSEFLOW_CHROME_PATH=/absolute/path/to/chrome npm run test:e2e`。

浏览器测试操作完整创作、设置与改稿、版本分支、A/B 切换、评分与删除保护，验证播放、下载、重载、错误提示、Key 输入清空和窄屏布局。额外使用 `127.0.0.1:18081` 和 `127.0.0.1:18082` 的本地契约服务；请确保 18080/18081/18082 端口空闲。

独立桌面测试：

```bash
./scripts/test-desktop.sh
```

此测试使用 Qt 离屏窗口和临时 XDG 数据。第一部分通过仅测试时开启的回环调试端口驱动实际 Qt WebEngine，验证 MP3 播放、分支、A/B、单实例和退出；第二部分使用 Qt Test 操作保存对话框，验证 WAV / JSON 文件落盘、活动任务和未保存修改的退出确认、端口释放。测试使用虚拟音频输出，不改变系统音频配置。

## 数据与安全边界

遵循 XDG 环境变量，未设置或值不是绝对路径时使用默认目录：

| 用途 | 默认位置 |
| --- | --- |
| 配置 | `~/.config/museflow/` |
| 数据库 | `~/.local/share/museflow/museflow.db` |
| 音频 | `~/.local/share/museflow/projects/<项目 UUID>/<任务 UUID>.wav 或 .mp3` |
| 缓存 | `~/.cache/museflow/` |

非敏感设置位于配置目录的 `settings.json`，仅包含服务商、地址、模型、超时和音乐输出偏好。两个环境变量由后端启动时读取。界面输入的 Key 不写入配置、数据库、日志或导出，也不进入浏览器持久化存储。请不要把真实 Key 写入示例文件。

后端限制请求正文大小，检查 Host/Origin 和跨站请求标记，SQLite 查询使用参数绑定，数据库只保存允许的字段，文件路径由后端生成的 UUID 构造，音频存放于静态资源目录之外。进程使用 `umask(0077)`，XDG 应用目录设置为仅当前用户可访问。无请求正文日志，无浏览器持久化存储。

数据目录使用进程锁防止重复启动影响正在生成的任务；操作系统会在进程结束时自动释放锁。

所有外部模型请求仅由 C++ libcurl 发起，支持 Linux `HTTP_PROXY`、`HTTPS_PROXY` 和 `NO_PROXY`（小写变量同样有效）。不记录请求正文、认证头或上游原始错误。文本单响应最多 2 MiB，Google 单响应最多 192 MiB，解码音频最多 128 MiB；JSON 嵌套最多 64 层；不跟随重定向，不关闭证书验证。

## 代码结构

```text
desktop/
  src/                    Qt 窗口、本机页面边界、单实例与进程管理
  resources/              应用图标
  tests/                  原生保存对话框与窗口关闭测试
backend/
  include/museflow/        模型、Provider、仓库与队列接口
  src/api/                HTTP 路由、本地访问边界、静态资源
  src/models/             Schema / 业务校验、提示词变化检测与转换
  schema/                 内置 MusicBlueprint JSON Schema
  src/providers/          DeepSeek、Lyria 与独立文本/音乐 Mock 实现
  src/repositories/       SQLite，参数绑定及事务
  src/services/           独立文本/音乐队列、FFmpeg 处理、仅内存 Key 与非敏感设置
  src/utils/              libcurl、UUID、时间与 XDG 路径
  tests/                  C++ 核心测试
  third_party/            固定版本 cpp-httplib 及许可证
frontend/src/
  api/ types/ stores/     HTTP 客户端、类型、校验、Pinia 创作状态
  components/ views/     编辑器、播放器、页面和路由
  styles/                原生 CSS
frontend/tests/           Chrome 端到端测试
assets/                   原始合成测试音频
scripts/                  构建、开发、运行、测试和音频生成
```

后续可在 `GenerationQueue` 状态更新处接入事件发布以提供 SSE/WebSocket；目前客户端按任务 ID 轮询。旧版本的 `parentVersionId` 为空仍作为独立根节点，新分支记录真实父版本 ID。现有数据库原地兼容，不需要删除数据。
