<script setup>
import { computed, nextTick, ref } from "vue";

const page = ref("connect");
// connect | auth | chat

const serverUrl = ref("ws://127.0.0.1:2006");
const socket = ref(null);
const connected = ref(false);

const authMode = ref("login");
// login | register

const account = ref("");
const password = ref("");
const confirmPassword = ref("");

const nickname = ref("");
const phone = ref("");
const birthday = ref("");
const gender = ref("");
const email = ref("");

const rememberMe = ref(false);

const currentUser = ref(null);

const authError = ref("");
const authInfo = ref("");
const connectionError = ref("");

const onlineUsers = ref([]);
const messages = ref([]);

const privateTarget = ref("");
const privateContent = ref("");

const groupName = ref("");
const groupId = ref("");
const groupContent = ref("");

const activeChatMode = ref("group");
// private | group

const composerText = ref("");

const canChat = computed(() => connected.value && currentUser.value !== null);

const chatTitle = computed(() =>
{
    if (activeChatMode.value === "private" && privateTarget.value)
        return `与 ${privateTarget.value} 私聊`;

    if (activeChatMode.value === "group" && groupId.value)
        return `群聊 ${groupId.value}`;

    return "聊天室";
});

const chatSubtitle = computed(() =>
{
    if (activeChatMode.value === "private")
        return "私聊消息";

    return "群聊消息";
});

function nowTime()
{
    return new Date().toLocaleTimeString();
}

async function scrollMessagesToBottom()
{
    await nextTick();

    const panel = document.querySelector(".chat-messages");
    if (panel)
        panel.scrollTop = panel.scrollHeight;
}

function addMessage(message)
{
    messages.value.push({
        time: nowTime(),
        ...message,
    });

    scrollMessagesToBottom();
}

function sendJson(payload)
{
    if (!socket.value || socket.value.readyState !== WebSocket.OPEN)
    {
        addMessage({
            category: "error",
            title: "系统",
            content: "WebSocket 未连接",
            self: false,
        });

        return false;
    }

    socket.value.send(JSON.stringify(payload));
    return true;
}

function connectServer()
{
    connectionError.value = "";

    if (!serverUrl.value.trim())
    {
        connectionError.value = "服务器地址不能为空";
        return;
    }

    if (socket.value && socket.value.readyState === WebSocket.OPEN)
        socket.value.close();

    const ws = new WebSocket(serverUrl.value.trim());
    socket.value = ws;

    ws.addEventListener("open", () =>
    {
        connected.value = true;
        page.value = "auth";
    });

    ws.addEventListener("message", (event) =>
    {
        let data;

        try
        {
            data = JSON.parse(event.data);
        }
        catch
        {
            addMessage({
                category: "system",
                title: "服务器",
                content: event.data,
                self: false,
            });
            return;
        }

        handleServerMessage(data);
    });

    ws.addEventListener("close", () =>
    {
        connected.value = false;
        socket.value = null;
        currentUser.value = null;

        if (page.value !== "connect")
        {
            page.value = "connect";
            connectionError.value = "连接已断开";
        }
    });

    ws.addEventListener("error", () =>
    {
        connectionError.value = "连接失败，请确认后端服务器已经启动";
    });
}

function disconnectServer()
{
    if (socket.value)
        socket.value.close();

    socket.value = null;
    connected.value = false;
    currentUser.value = null;
    page.value = "connect";
}

function switchAuthMode(mode)
{
    authMode.value = mode;
    authError.value = "";
    authInfo.value = "";
}

function registerUser()
{
    authError.value = "";
    authInfo.value = "";

    if (!account.value.trim() || !nickname.value.trim() || !password.value ||
        !phone.value.trim() || !birthday.value)
    {
        authError.value = "账号、昵称、密码、手机号、出生日期不能为空";
        return;
    }

    if (password.value.length < 6)
    {
        authError.value = "密码至少 6 位";
        return;
    }

    if (password.value !== confirmPassword.value)
    {
        authError.value = "两次输入的密码不一致";
        return;
    }

    sendJson({
        type: "register",
        account: account.value.trim(),
        nickname: nickname.value.trim(),
        password: password.value,
        phone: phone.value.trim(),
        birthday: birthday.value,
        gender: gender.value,
        email: email.value.trim(),
    });
}

function loginUser()
{
    authError.value = "";
    authInfo.value = "";

    if (!account.value.trim() || !password.value)
    {
        authError.value = "账号和密码不能为空";
        return;
    }

    sendJson({
        type: "login",
        account: account.value.trim(),
        password: password.value,
    });
}

function handleServerMessage(data)
{
    const type = data.type;

    if (type === "system")
    {
        if (page.value === "auth")
            authInfo.value = data.message ?? "";

        addMessage({
            category: "system",
            title: "系统",
            content: data.message ?? "",
            self: false,
        });
        return;
    }

    if (type === "error")
    {
        if (page.value === "auth")
            authError.value = data.message ?? "操作失败";
        else
            addMessage({
                category: "error",
                title: "错误",
                content: data.message ?? "操作失败",
                self: false,
            });

        return;
    }

    if (type === "register_result")
    {
        if (data.success)
        {
            authInfo.value = "注册成功，现在可以登录";
            authError.value = "";
            authMode.value = "login";
            password.value = "";
            confirmPassword.value = "";
        }
        else
        {
            authError.value = data.message ?? "注册失败";
            authInfo.value = "";
        }

        return;
    }

    if (type === "login_result")
    {
        if (!data.success)
        {
            authError.value = data.message ?? "登录失败";
            authInfo.value = "";
            return;
        }

        currentUser.value = {
            userId: data.user_id,
            account: data.account,
            nickname: data.nickname,
        };

        authError.value = "";
        authInfo.value = "";
        page.value = "chat";

        addMessage({
            category: "system",
            title: "系统",
            content: `${data.nickname}，欢迎回来`,
            self: false,
        });

        requestOnlineUsers();
        return;
    }

    if (type === "who_result")
    {
        onlineUsers.value = data.users ?? [];
        return;
    }

    if (type === "private_message")
    {
        addMessage({
            category: "private",
            title: data.from,
            content: data.content ?? "",
            self: false,
        });
        return;
    }

    if (type === "private_message_result")
    {
        addMessage({
            category: data.success ? "private" : "error",
            title: data.success ? "我" : "私聊失败",
            content: data.success ? data.content || "发送成功" : data.message,
            self: data.success,
        });
        return;
    }

    if (type === "create_group_result")
    {
        if (data.success && data.group_id)
        {
            groupId.value = String(data.group_id);
            activeChatMode.value = "group";
        }

        addMessage({
            category: data.success ? "system" : "error",
            title: "创建群聊",
            content: `${data.message ?? ""} group_id=${data.group_id ?? ""}`,
            self: false,
        });
        return;
    }

    if (type === "join_group_result")
    {
        addMessage({
            category: data.success ? "system" : "error",
            title: "加入群聊",
            content: `${data.message ?? ""} group_id=${data.group_id ?? ""}`,
            self: false,
        });
        return;
    }

    if (type === "group_message")
    {
        const isSelf = data.from_account === currentUser.value?.account ||
            data.from === currentUser.value?.nickname;

        addMessage({
            category: "group",
            title: data.from,
            content: data.content ?? "",
            self: isSelf,
        });
        return;
    }

    addMessage({
        category: "system",
        title: "未知消息",
        content: JSON.stringify(data),
        self: false,
    });
}

function requestOnlineUsers()
{
    sendJson({
        type: "who",
    });
}

function selectPrivateUser(user)
{
    if (user.account === currentUser.value?.account)
        return;

    activeChatMode.value = "private";
    privateTarget.value = user.account;
}

function selectGroupChat()
{
    activeChatMode.value = "group";
}

function createGroup()
{
    if (!canChat.value)
        return;

    if (!groupName.value.trim())
    {
        addMessage({
            category: "error",
            title: "建群失败",
            content: "群名不能为空",
            self: false,
        });
        return;
    }

    sendJson({
        type: "create_group",
        group_name: groupName.value.trim(),
    });
}

function joinGroup()
{
    if (!canChat.value)
        return;

    const id = Number(groupId.value);

    if (!Number.isInteger(id) || id <= 0)
    {
        addMessage({
            category: "error",
            title: "加群失败",
            content: "group_id 必须是正整数",
            self: false,
        });
        return;
    }

    sendJson({
        type: "join_group",
        group_id: id,
    });

    activeChatMode.value = "group";
}

function sendComposerMessage()
{
    if (!canChat.value)
        return;

    const text = composerText.value.trim();

    if (!text)
        return;

    if (activeChatMode.value === "private")
    {
        if (!privateTarget.value.trim())
        {
            addMessage({
                category: "error",
                title: "私聊失败",
                content: "请先在左侧选择私聊用户",
                self: false,
            });
            return;
        }

        privateContent.value = text;

        sendJson({
            type: "private_message",
            to: privateTarget.value.trim(),
            content: text,
        });

        composerText.value = "";
        return;
    }

    const id = Number(groupId.value);

    if (!Number.isInteger(id) || id <= 0)
    {
        addMessage({
            category: "error",
            title: "群聊失败",
            content: "请先填写 group_id",
            self: false,
        });
        return;
    }

    groupContent.value = text;

    sendJson({
        type: "group_message",
        group_id: id,
        content: text,
    });

    composerText.value = "";
}

function logout()
{
    currentUser.value = null;
    messages.value = [];
    onlineUsers.value = [];
    page.value = "auth";
}
</script>

<template>
    <div class="app">
        <section v-if="page === 'connect'" class="screen center-screen">
            <div class="connect-card">
                <div class="brand">
                    <div class="logo">CS</div>
                    <div>
                        <h1>Cs Chat Room</h1>
                        <p>C++ epoll WebSocket 聊天室</p>
                    </div>
                </div>

                <div class="form-block">
                    <label>服务器地址</label>
                    <input
                        v-model="serverUrl"
                        placeholder="ws://127.0.0.1:2006"
                        @keydown.enter="connectServer"
                    />

                    <button class="primary full" @click="connectServer">
                        连接服务器
                    </button>

                    <p v-if="connectionError" class="error-text">
                        {{ connectionError }}
                    </p>
                </div>
            </div>
        </section>

        <section v-else-if="page === 'auth'" class="auth-page">
            <div class="auth-box" :class="{ register: authMode === 'register' }">
                <button class="auth-close" @click="disconnectServer">×</button>

                <div class="auth-left-panel">
                    <div class="stack-logo">◆</div>

                    <div class="left-center">
                        <template v-if="authMode === 'login'">
                            <h2>创建账号</h2>
                            <button class="outline-button" @click="switchAuthMode('register')">
                                注册
                            </button>
                        </template>

                        <template v-else>
                            <h2>已有账号？</h2>
                            <button class="outline-button" @click="switchAuthMode('login')">
                                返回登录
                            </button>
                        </template>
                    </div>
                </div>

                <div class="auth-right-panel">
                    <div class="auth-form-wrap">
                        <template v-if="authMode === 'login'">
                            <h1>登录</h1>

                            <input
                                v-model="account"
                                placeholder="账号"
                                @keydown.enter="loginUser"
                            />

                            <input
                                v-model="password"
                                type="password"
                                placeholder="密码"
                                @keydown.enter="loginUser"
                            />

                            <label class="remember-row">
                                <input v-model="rememberMe" type="checkbox" />
                                <span>记住我</span>
                            </label>

                            <button class="auth-main-button" @click="loginUser">
                                登录
                            </button>

                            <div class="auth-links">
                                <button @click="switchAuthMode('register')">
                                    注册
                                </button>
                                <span>|</span>
                                <button>忘记密码？</button>
                            </div>
                        </template>

                        <template v-else>
                            <h1>创建账号</h1>

                            <div class="register-scroll">
                                <input v-model="account" placeholder="账号" />
                                <input v-model="nickname" placeholder="昵称" />
                                <input v-model="phone" placeholder="手机号" />

                                <input
                                    v-model="birthday"
                                    type="date"
                                    placeholder="出生日期"
                                />

                                <select v-model="gender">
                                    <option value="">选择性别</option>
                                    <option value="男">男</option>
                                    <option value="女">女</option>
                                    <option value="保密">保密</option>
                                </select>

                                <input v-model="email" placeholder="邮箱（可选）" />

                                <input
                                    v-model="password"
                                    type="password"
                                    placeholder="密码"
                                />

                                <input
                                    v-model="confirmPassword"
                                    type="password"
                                    placeholder="确认密码"
                                    @keydown.enter="registerUser"
                                />
                            </div>

                            <button class="auth-main-button" @click="registerUser">
                                注册
                            </button>

                            <div class="auth-links">
                                <button @click="switchAuthMode('login')">
                                    返回登录
                                </button>
                            </div>
                        </template>

                        <p v-if="authError" class="auth-message error">
                            {{ authError }}
                        </p>

                        <p v-if="authInfo" class="auth-message success">
                            {{ authInfo }}
                        </p>
                    </div>
                </div>
            </div>
        </section>

        <section v-else class="chat-shell">
            <aside class="icon-rail">
                <div class="rail-logo">C</div>

                <button class="rail-button active">💬</button>
                <button class="rail-button">👥</button>
                <button class="rail-button">📁</button>
                <button class="rail-button">📊</button>
                <button class="rail-button">⚙</button>

                <div class="rail-bottom">
                    <button class="rail-button" @click="logout">↩</button>
                    <button class="rail-button danger-icon" @click="disconnectServer">⏻</button>
                </div>
            </aside>

            <aside class="conversation-panel">
                <div class="inbox-header">
                    <h2>消息列表</h2>
                    <p>当前会话</p>
                </div>

                <div class="search-box">
                    <span>🔍</span>
                    <input placeholder="搜索联系人" @focus="requestOnlineUsers" />
                </div>

                <div
                    class="conversation-item group-entry"
                    :class="{ active: activeChatMode === 'group' }"
                    @click="selectGroupChat"
                >
                    <div class="avatar group-avatar">G</div>
                    <div class="conversation-info">
                        <div class="conversation-top">
                            <strong>群聊</strong>
                            <span>现在</span>
                        </div>
                        <p>群号：{{ groupId || "未选择" }}</p>
                    </div>
                </div>

                <div class="group-tools">
                    <input v-model="groupName" placeholder="新群名" />
                    <button class="tiny primary" @click="createGroup">创建</button>

                    <input v-model="groupId" placeholder="group_id" />
                    <button class="tiny secondary" @click="joinGroup">加入</button>
                </div>

                <div class="list-title">
                    <span>在线用户</span>
                    <button @click="requestOnlineUsers">刷新</button>
                </div>

                <div class="conversation-list">
                    <div
                        v-for="user in onlineUsers"
                        :key="user.user_id"
                        class="conversation-item"
                        :class="{
                            active:
                                activeChatMode === 'private' &&
                                privateTarget === user.account,
                            self: user.account === currentUser?.account,
                        }"
                        @click="selectPrivateUser(user)"
                    >
                        <div class="avatar">
                            {{ user.nickname?.slice(0, 1).toUpperCase() }}
                        </div>

                        <div class="conversation-info">
                            <div class="conversation-top">
                                <strong>{{ user.nickname }}</strong>
                                <span>在线</span>
                            </div>
                            <p>
                                {{
                                    user.account === currentUser?.account
                                        ? "这是你"
                                        : `账号：${user.account}`
                                }}
                            </p>
                        </div>
                    </div>

                    <p v-if="onlineUsers.length === 0" class="empty-list">
                        暂无在线用户
                    </p>
                </div>
            </aside>

            <main class="chat-main">
                <header class="chat-topbar">
                    <div class="chat-user">
                        <div class="avatar big">
                            {{
                                activeChatMode === "private" && privateTarget
                                    ? privateTarget.slice(0, 1).toUpperCase()
                                    : "G"
                            }}
                        </div>

                        <div>
                            <h1>{{ chatTitle }}</h1>
                            <p>{{ chatSubtitle }} · 通过 WebSocket</p>
                        </div>
                    </div>

                    <div class="me-info">
                        <span>{{ currentUser?.nickname }}</span>
                        <div class="avatar small">
                            {{ currentUser?.nickname?.slice(0, 1).toUpperCase() }}
                        </div>
                    </div>
                </header>

                <section class="chat-messages">
                    <div class="session-chip">
                        Session: {{ new Date().toLocaleDateString() }}
                    </div>

                    <div
                        v-for="(message, index) in messages"
                        :key="index"
                        class="message-row"
                        :class="{ mine: message.self }"
                    >
                        <div v-if="!message.self" class="avatar mini">
                            {{ message.title?.slice(0, 1).toUpperCase() || "系" }}
                        </div>

                        <div class="message-block">
                            <div class="message-meta">
                                <strong>{{ message.title }}</strong>
                                <span>{{ message.time }}</span>
                            </div>

                            <div class="bubble" :class="message.category">
                                {{ message.content }}
                            </div>
                        </div>

                        <div v-if="message.self" class="avatar mini me-mini">
                            {{ currentUser?.nickname?.slice(0, 1).toUpperCase() }}
                        </div>
                    </div>

                    <div v-if="messages.length === 0" class="empty-chat">
                        还没有消息，先选择联系人或加入群聊。
                    </div>
                </section>

                <footer class="composer">
                    <div class="composer-title">
                        <span>
                            {{
                                activeChatMode === "private"
                                    ? `正在私聊：${privateTarget || "未选择"}`
                                    : `当前群聊：${groupId || "未选择"}`
                            }}
                        </span>

                        <div class="composer-icons">
                            <button>⚡</button>
                            <button>😊</button>
                            <button>📎</button>
                        </div>
                    </div>

                    <div class="composer-input">
                        <textarea
                            v-model="composerText"
                            placeholder="输入消息，Enter 发送，Shift+Enter 换行"
                            @keydown.enter.exact.prevent="sendComposerMessage"
                        ></textarea>

                        <button class="send-button" @click="sendComposerMessage">
                            发送 ✈
                        </button>
                    </div>
                </footer>
            </main>
        </section>
    </div>
</template>
