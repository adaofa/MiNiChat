// #ifndef CHATSERVICESTATEMANAGER_H
// #define CHATSERVICESTATEMANAGER_H
// #pragma once
// #include "redis.hpp"
// #include "usermodel.hpp"
// #include <vector>
// #include <string>

// class ChatServiceStateManager {
// public:
//     ChatServiceStateManager();

//     void setUserState(int userid, const std::string& state);
//     std::string getUserState(int userid);

//     // 定期将 Redis 中的在线状态同步回 MySQL
//     void syncRedisStateToMySQL();

// private:
//     Redis redis_;
//     UserModel usermodel_;
// };


// #endif // CHATSERVICESTATEMANAGER_H
