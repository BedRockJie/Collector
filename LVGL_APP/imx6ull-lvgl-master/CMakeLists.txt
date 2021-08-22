cmake_minimum_required (VERSION 2.8)  
#//cmake最低使用版本

project(card_detect)
#//工程名

#//设置编译器，此处为ARM平台

#link_libraries("/home/kknight/Linux/tslib-1.4/tmp/lib/libts.so")
#//链接使用的库，相当于gcc -L/xxx/xxx


# 函数：递归添加子目录到全局头文件，root_dir 需要是完整路径  !!!!!! 头文件
function(include_sub_directories_recursively root_dir)
    # 当前路径是一个目录吗，是的话就加入到包含目录
    if (IS_DIRECTORY ${root_dir})
        message("include dir: " ${root_dir})
        include_directories(${root_dir})
    endif()

    # 获得当前目录下的所有文件，让如ALL_SUB列表中
    file(GLOB ALL_SUB RELATIVE ${root_dir} ${root_dir}/*)
    foreach(sub ${ALL_SUB})
        if (IS_DIRECTORY ${root_dir}/${sub})
            # 对子目录递归调用，包含
            include_sub_directories_recursively(${root_dir}/${sub})
        endif()
    endforeach()
endfunction()

# 全局包含目录，这里包含程序目录
#message("include dir: " ${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR})

include_sub_directories_recursively(${PROJECT_SOURCE_DIR}/lvgl)
include_sub_directories_recursively(${PROJECT_SOURCE_DIR}/lv_drivers)
include_sub_directories_recursively(${PROJECT_SOURCE_DIR}/lv_100ask)
include_sub_directories_recursively(${PROJECT_SOURCE_DIR}/lv_lib_png)


# 查找满足规则的文件名放到 SOURCES 变量
file(GLOB_RECURSE LVGL_SOURCES lvgl/src/*.c)
file(GLOB LV_DRV_SOURCES
	lv_drivers/*.c
	lv_drivers/indev/*.c
	lv_drivers/gtkdrv/*.c
	lv_drivers/display/*.c
)
file(GLOB LV_PNG_SOURCES  lv_lib_png/*c)
#file(GLOB_RECURSE LV_EXP_SOURCES lv_100ask/src/*.c)
file(GLOB_RECURSE LV_EXP_SOURCES lv_100ask/**/*.c)
file(GLOB_RECURSE LV_EXP_SOURCES lv_100ask/**/*/*.c)

# message (${LV_EXP_SOURCES})

# file(GLOB_RECURSE LV_EXP_SOURCES mytest/*.c)
# 生成可执行文件
add_executable(card_detect main.c ${LVGL_SOURCES} ${LV_DRV_SOURCES} ${LV_PNG_SOURCES} ${LV_EXP_SOURCES})

# export RISCV_ROOT_PATH=/home/nihui/osd/riscv64-linux-x86_64-20210512
# cmake -DCMAKE_TOOLCHAIN_FILE=../toolchains/c906.toolchain.cmake -DCMAKE_BUILD_TYPE=relwithdebinfo -DNCNN_BUILD_TESTS=ON -DNCNN_OPENMP=OFF -DNCNN_THREADS=OFF -DNCNN_RUNTIME_CPU=OFF -DNCNN_RVV=ON -DNCNN_SIMPLEOCV=ON -DNCNN_BUILD_EXAMPLES=ON ..
