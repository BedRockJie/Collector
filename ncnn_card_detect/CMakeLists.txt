cmake_minimum_required(VERSION 3.2)


project(carddetect)


set(name carddetect)
set(SOURCE main.cpp)

add_executable(${name} ${SOURCE})


# 包含ncnn的头文件
include_directories(/home/gaojies/workspace/d1-tina-open/tools/ncnn/build-c906/install/include/ncnn /home/gaojies/workspace/d1-tina-open/tools/ncnn/src)


# 包含ncnn的链接文件
#link_directories(/home/gaojies/workspace/d1-tina-open/tools/ncnn/build-c906/install/lib)

set(ncnn_DIR "/home/gaojies/workspace/d1-tina-open/tools/ncnn/build-c906/install/lib/cmake/ncnn" CACHE PATH "/home/gaojies/workspace/d1-tina-open/tools/ncnn/build-c906/install/lib/cmake/ncnn/ncnnConfig.cmake")



find_package(ncnn REQUIRED)   # 表示不找到 ncnn 就停掉 cmake


target_link_libraries(${name} ncnn)

