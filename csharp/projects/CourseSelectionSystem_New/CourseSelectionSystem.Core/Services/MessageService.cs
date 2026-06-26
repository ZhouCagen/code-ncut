using System.Data;
using CourseSelectionSystem.Core.DTOs;
using CourseSelectionSystem.Core.Repositories;

namespace CourseSelectionSystem.Core.Services;

public class MessageService
{
    private readonly MessageRepository _messageRepository;

    public MessageService(MessageRepository messageRepository)
    {
        _messageRepository = messageRepository;
    }

    public async Task<ServiceResult> SendMessageAsync(
        string fromUserId,
        string toUserId,
        string content
    )
    {
        if (string.IsNullOrWhiteSpace(content))
        {
            return new ServiceResult { Success = false, Message = "留言内容不能为空" };
        }

        await _messageRepository.SendMessageAsync(fromUserId, toUserId, content);

        return new ServiceResult { Success = true, Message = "留言发送成功" };
    }

    public async Task<DataTable> GetMessagesForUserAsync(string userId)
    {
        return await _messageRepository.GetMessagesForUserAsync(userId);
    }

    public async Task<DataTable> GetConversationAsync(string userA, string userB)
    {
        return await _messageRepository.GetConversationAsync(userA, userB);
    }

    public async Task<ServiceResult> MarkAsReadAsync(int messageId)
    {
        await _messageRepository.MarkAsReadAsync(messageId);

        return new ServiceResult { Success = true, Message = "留言已读" };
    }
}
