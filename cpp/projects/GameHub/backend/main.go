package main

import (
	"net/http"
	"time"

	"github.com/gin-gonic/gin"
	"github.com/golang-jwt/jwt/v5"
	"golang.org/x/crypto/bcrypt"
)

// JWT密钥--生产环境请用配置或环境变量管理
var jwtKey = []byte("my_secret_key")

// 用户存储（演示用，生产请用数据库）
var users = map[string]string{}

// User 结构体--接收请求数据
type User struct {
	Username string `json:"username"`
	Password string `json:"password"`
}

// JWT Claims 结构体--包含用户名和注册的JWT标准声明
type Claims struct {
	Username string `json:"username"`
	jwt.RegisteredClaims
}

// 接口函数--注册处理
func RegisterHandler(c *gin.Context) {
	var request User

	// 绑定请求的 JSON 到 User 结构体
	err := c.ShouldBindJSON(&request)
	if err != nil {
		// 请求格式错误--返回400
		c.JSON(http.StatusBadRequest, gin.H{"error": "参数错误"})
		return
	}

	// 检查用户名是否已存在
	_, exists := users[request.Username]
	if exists {
		c.JSON(http.StatusBadRequest, gin.H{"error": "用户已存在"})
		return
	}

	// 对密码进行哈希加密--防止明文存储
	hashedPassword, err := bcrypt.GenerateFromPassword([]byte(request.Password), bcrypt.DefaultCost)
	if err != nil {
		// 加密失败，返回500
		c.JSON(http.StatusInternalServerError, gin.H{"error": "密码加密失败"})
		return
	}
	// 存储用户名和加密后的密码
	users[request.Username] = string(hashedPassword)

	// 返回注册成功
	c.JSON(http.StatusOK, gin.H{"message": "注册成功"})

}

// 接口函数--登陆处理

func LoginHandler(c *gin.Context) {

	var request User

	// 绑定请求的 JSON 到 User 结构体
	err := c.ShouldBindJSON(&request)

	if err != nil {
		// 请求格式错误--返回502
		c.JSON(http.StatusBadGateway, gin.H{"error": "参数错误"})
		return
	}

	// 从存储中取出用户哈希密码
	storedHash, exists := users[request.Username]

	// 校验用户是否存在且密码匹配
	if !exists || bcrypt.CompareHashAndPassword([]byte(storedHash), []byte(request.Password)) != nil {
		// 用户名或密码错误--返回401
		c.JSON(http.StatusUnauthorized, gin.H{"error": "用户名或密码错误"})
		return
	}

	// 设置JWT过期时间（24小时）
	expirationTime := time.Now().Add(24 * time.Hour)

	// 设置JWT注册声明，包含过期时间
	registeredClaims := jwt.RegisteredClaims{
		ExpiresAt: jwt.NewNumericDate(expirationTime),
	}

	// 自定义Claims，包含用户名和注册声明
	claims := &Claims{
		Username:         request.Username,
		RegisteredClaims: registeredClaims,
	}

	// 创建带Claims的JWT令牌--使用HS256签名算法
	token := jwt.NewWithClaims(jwt.SigningMethodHS256, claims)

	// 用密钥签名生成token字符串
	tokenString, err := token.SignedString(jwtKey)
	if err != nil {
		// 签名失败--返回500
		c.JSON(http.StatusInternalServerError, gin.H{"error": "生成令牌失效"})
		return
	}

	// 返回JWT令牌给客户端
	c.JSON(http.StatusOK, gin.H{"token": tokenString})
}

func main() {
	// 创建Gin默认引擎，包含Logger和Recovery中间件
	r := gin.Default()

	// 绑定注册和登录路由，指向对应处理函数
	r.POST("api/auth/register", RegisterHandler)
	r.POST("api/auth/login", LoginHandler)

	// 启动服务器，监听8080端口
	r.Run(":8080")
}

/*
主要分为注册和登录两个接口。
注册时：接收用户名密码，判断是否存在，密码加密，保存用户。
登录时：验证用户名密码，生成带过期时间的JWT令牌返回。
JWT密钥用来签名token，保证安全性。
用Gin框架来快速搭建HTTP服务器和路由。
*/
