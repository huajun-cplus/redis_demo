# CMakeList.txt: redis_demo 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 2.8.7)

file(GLOB_RECURSE REDIS_DEMO_SRCS ${PROJECT_SOURCE_DIR}/redis_demo/*.cpp)

set(CMAKE_CXX_STANDARD 17)

IF(CMAKE_CL_64)
    message("platform info: x64")
ELSE()
    message("platform info: x86")
ENDIF(CMAKE_CL_64)

if(WIN32)
    message("platform info: win32")

    # 设置运行库
    set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} /MT")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")

    # lib
    link_libraries(${PROJECT_SOURCE_DIR}/common/3rd/cpp_redis/lib/cpp_redis_x64_mt.lib)
    link_libraries(${PROJECT_SOURCE_DIR}/common/3rd/tacopie/lib/tacopie_x64_mt.lib)
elseif(UNIX)
    message(platform: unix)
    # a
    link_libraries(${PROJECT_SOURCE_DIR}/common/3rd/cpp_redis/a/libcpp_redis_x64.a)
    link_libraries(${PROJECT_SOURCE_DIR}/common/3rd/tacopie/a/libtacopie_x64.a)
endif()

# include
include_directories(${PROJECT_SOURCE_DIR}/common)
include_directories(${PROJECT_SOURCE_DIR}/redis_demo/)

include_directories(${PROJECT_SOURCE_DIR}/common/3rd/cpp_redis/includes)
include_directories(${PROJECT_SOURCE_DIR}/common/3rd/tacopie/includes)

# 将源代码添加到此项目的可执行文件。
add_executable (redis_demo  ${REDIS_DEMO_SRCS})
