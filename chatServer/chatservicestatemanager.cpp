// #include "chatservicestatemanager.h"
// #include "mysqlconnpool.h"

// ChatServiceStateManager::ChatServiceStateManager()
// {

// }

// void ChatServiceStateManager::setUserState(int userid, const std::string& state)
// {
//     std::string key = "state:" + std::to_string(userid);
//     //redis_.set(key, state);
// }

// std::string ChatServiceStateManager::getUserState(int userid)
// {
//     std::string key = "state:" + std::to_string(userid);
//     std::string state = redis_.get(key);
//     if (state.empty())
//     {
//         auto mysqlConn = MysqlConnPool::getInstance()->getConnection();
//         User user = usermodel_.query(userid);
//         redis_.set(key, user.getState(), 300);  // 缓存5分钟
//         return user.getState();
//     }
//     return state;
// }

// void ChatServiceStateManager::syncRedisStateToMySQL()
// {
//     std::vector<std::string> keys = redis_.getKeysWithPrefix("state:");
//     for (const auto& key : keys)
//     {
//         std::string idstr = key.substr(strlen("state:"));
//         int userid = std::stoi(idstr);
//         std::string state = redis_.get(key);
//         User user(userid, "", "", state);
//         usermodel_.updateState(user);
//     }
// }
