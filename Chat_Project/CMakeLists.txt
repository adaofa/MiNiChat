cmake_minimum_required(VERSION 3.15.0)
project(chat)
set(CMAKE_CXX_STANDARD 17)

#配置编译选项
#set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -g)

#配置最终的可执行文件输出的路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

#配置头文件的搜索路径
include_directories(${PROJECT_SOURCE_DIR}/common/include)
include_directories(${PROJECT_SOURCE_DIR}/common/include/server)
include_directories(${PROJECT_SOURCE_DIR}/common/include/server/db)
include_directories(${PROJECT_SOURCE_DIR}/common/include/server/model)
include_directories(${PROJECT_SOURCE_DIR}/common/include/server/redis)
include_directories(${PROJECT_SOURCE_DIR}/common/thirdparty)

#加载子目录
add_subdirectory(server)
add_subdirectory(client)
add_subdirectory(common)
