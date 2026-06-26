using System.Data;
using CourseSelectionSystem.Core.Data;
using CourseSelectionSystem.Core.Models;
using Microsoft.Data.Sqlite;

namespace CourseSelectionSystem.Core.Repositories;

public class MessageRepository : BaseRepository<Message>
{
    public MessageRepository(Database database)
        : base(database) { }

    public async Task<DataTable> GetMessagesForUserAsync(string userId)
    {
        string sql = """
            SELECT *
            FROM [Message]
            WHERE [FROMUSERID] = @UserId
               OR [TOUSERID] = @UserId
            ORDER BY [CREATEDAT] DESC
            """;

        SqliteParameter[] parameters = [new("@UserId", userId)];

        return await _database.ExecuteQueryAsync(sql, parameters);
    }

    public async Task<DataTable> GetConversationAsync(string userA, string userB)
    {
        string sql = """
            SELECT *
            FROM [Message]
            WHERE
            (
                [FROMUSERID] = @UserA
                AND [TOUSERID] = @UserB
            )
            OR
            (
                [FROMUSERID] = @UserB
                AND [TOUSERID] = @UserA
            )
            ORDER BY [CREATEDAT] ASC
            """;

        SqliteParameter[] parameters = [new("@UserA", userA), new("@UserB", userB)];

        return await _database.ExecuteQueryAsync(sql, parameters);
    }

    public async Task<int> SendMessageAsync(string fromUserId, string toUserId, string content)
    {
        string sql = """
            INSERT INTO [Message]
            (
                [FROMUSERID],
                [TOUSERID],
                [CONTENT],
                [CREATEDAT],
                [ISREAD]
            )
            VALUES
            (
                @FromUserId,
                @ToUserId,
                @Content,
                @CreatedAt,
                @IsRead
            )
            """;

        SqliteParameter[] parameters =
        [
            new("@FromUserId", fromUserId),
            new("@ToUserId", toUserId),
            new("@Content", content),
            new("@CreatedAt", DateTime.Now),
            new("@IsRead", false),
        ];

        return await _database.ExecuteNonQueryAsync(sql, parameters);
    }

    public async Task<int> MarkAsReadAsync(int messageId)
    {
        string sql = """
            UPDATE [Message]
            SET [ISREAD] = 1
            WHERE [ID] = @MessageId
            """;

        SqliteParameter[] parameters = [new("@MessageId", messageId)];

        return await _database.ExecuteNonQueryAsync(sql, parameters);
    }
}
