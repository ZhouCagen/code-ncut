using CourseSelectionSystem.Core.DTOs;
using CourseSelectionSystem.Core.Repositories;

namespace CourseSelectionSystem.Core.Services;

public class AuthService
{
    private readonly UserRepository _userRepository;

    public AuthService(UserRepository userRepository)
    {
        _userRepository = userRepository;
    }

    public async Task<LoginResult> LoginAsync(string account, string password)
    {
        if (string.IsNullOrWhiteSpace(account) || string.IsNullOrWhiteSpace(password))
        {
            return new LoginResult { Success = false, Message = "账号或密码不能为空" };
        }

        var user = await _userRepository.FindByAccountAsync(account);

        if (user == null)
        {
            return new LoginResult { Success = false, Message = "账号不存在" };
        }

        if (!PasswordHasher.VerifyPassword(password, user.PASSWORDHASH))
        {
            return new LoginResult { Success = false, Message = "密码错误" };
        }

        return new LoginResult
        {
            Success = true,
            Message = "登录成功",
            Role = user.ROLE,
            USERID = user.USERID,
            ACCOUNT = user.ACCOUNT,
            RELATEDID = user.RELATEDID,
            COLLEGEID = user.COLLEGEID,
        };
    }
}
