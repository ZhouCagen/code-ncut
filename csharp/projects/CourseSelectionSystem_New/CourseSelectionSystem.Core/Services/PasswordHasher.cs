using System.Security.Cryptography;
using System.Text;

namespace CourseSelectionSystem.Core.Services;

public static class PasswordHasher
{
    public static string HashPassword(string password)
    {
        byte[] bytes = Encoding.UTF8.GetBytes(password);
        byte[] hashBytes = SHA256.HashData(bytes);

        return Convert.ToHexString(hashBytes).ToLower();
    }

    public static bool VerifyPassword(string password, string passwordHash)
    {
        if (password == passwordHash)
        {
            return true;
        }

        string currentHash = HashPassword(password);

        return string.Equals(currentHash, passwordHash, StringComparison.OrdinalIgnoreCase);
    }
}
