# CMakeList.txt: redis_demo 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 2.8.7)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE REDIS_DEMO_SRCS ${PROJECT_SOURCE_DIR}/redis_demo/*.cpp)

# 将源代码添加到此项目的可执行文件。
add_executable (redis_demo  ${REDIS_DEMO_SRCS})

# include
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/common)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/redis_demo/)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/common/3rd/cpp_redis/includes)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/common/3rd/tacopie/includes)

if(WIN32)
    message("platform info: win32")

    # 设置运行库
    set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} /MT")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")

    # lib
    target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/common/3rd/cpp_redis/lib/cpp_redis_x64_mt.lib)
    target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/common/3rd/tacopie/lib/tacopie_x64_mt.lib)
elseif(UNIX)
    message("platform info: unix")
    # a
    target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/common/3rd/cpp_redis/a/libcpp_redis.a)
    target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/common/3rd/tacopie/a/libtacopie.a)

    # pthread
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    target_link_libraries(${PROJECT_NAME} PUBLIC Threads::Threads)
endif()