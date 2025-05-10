#ifndef PUBLIC_H
#define PUBLIC_H

/*
server和client的公共文件
*/
enum EnMsgType
{
    LOGIN_MSG = 1,              //登录消息
    LOGIN_MSG_ACK,              //登录响应消息
    LOGINOUT_MSG,               //注销消息
    REG_MSG,                    //注册消息
    REG_MSG_ACK,                //注册响应消息
    ONE_CHAT_MSG,               //聊天消息
    LOGIN_EXTRA_MSG,            //延迟加载
    TEXT_MSG,                   //文本聊天信息
    IMAGE_MSG,                  //图片聊天信息
    AUDIO_MSG,                  //音频聊天信息
    VIDEO_MSG,                  //视频聊天信息
    FILE_MSG,                   //文件聊天信息
    ADD_FRIEND_REQUEST_MSG,     //添加好友请求中转消息
    ADD_FRIEND_RESPONSE_MSG,    //好友请求回复消息
    REMOVE_FRIEND_MSG,          //删除好友消息
    UPDATE_FRIEND_MSG,          //更新好友信息

    CREATE_GROUP_MSG,           //创建群组
    REMOVE_GROUP_MSG,           //删除群组
    ADD_GROUP_REQUEST_MSG,      //加入群组请求
    ADD_GROUP_RESPONSE_MSG,     //加入群组响应
    QUIT_GROUP_MSG,             //退出群聊
    UPDATE_GROUP_MSG,           //更新客户端群组信息
    MUTE_CHAT_MSG,              //群主禁言功能
    MUTE_CHAT_ACK,              //禁言响应结果
    GROUP_CHAT_MSG,             //群聊天
};

#endif
