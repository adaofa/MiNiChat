cmake_minimum_required(VERSION 3.16)

project(chatServer LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_executable(chatServer
  chatserver.hpp
  chatserver.cpp
  chatservice.hpp
  chatservice.cpp
  public.hpp
  json.hpp
  user.hpp
  usermodel.hpp
  usermodel.cpp
  friendmodel.hpp
  friendmodel.cpp
  group.hpp
  groupuser.hpp
  groupmodel.hpp
  groupmodel.cpp
  redis.hpp
  redis.cpp
  db.h
  db.cpp
  offlinemsgmodel.hpp
  offlinemsgmodel.cpp
  main.cpp

  chatservicestatemanager.h chatservicestatemanager.cpp
  mysqlconnpool.h mysqlconnpool.cpp
  redisconnpool.h redisconnpool.cpp

)
target_link_libraries(chatServer Qt${QT_VERSION_MAJOR}::Core
                    muduo_net muduo_base mysqlclient hiredis pthread)

include(GNUInstallDirs)
install(TARGETS chatServer
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
