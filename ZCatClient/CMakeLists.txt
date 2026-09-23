cmake_minimum_required(VERSION 4.2.1)
project(ZCatClient)

aux_source_directory(${CMAKE_SOURCE_DIR}/source/source SRC_LIST)

#启用RC语言
enable_language(RC)

#生成二进制资源文件.res(程序的图标)
set(RC_FILE ${CMAKE_SOURCE_DIR}/resource/app_ico/ZCat.rc)

add_executable(ZCatClient WIN32 ${SRC_LIST} ${RC_FILE})
set_target_properties(
    ZCatClient PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    RUNTIME_OUTPUT_Directory_DEBUG ${CMAKE_SOURCE_DIR}/bin
    RUNTIME_OUTPUT_Directory_RELEASE ${CMAKE_SOURCE_DIR}/bin
)

set(LIBS ${CMAKE_SOURCE_DIR}/lib/SDL2)
set(HEADS ${CMAKE_SOURCE_DIR}/source/include ${CMAKE_SOURCE_DIR}/include/SDL2)

target_include_directories(ZCatClient PRIVATE ${HEADS})
target_link_directories(ZCatClient PRIVATE ${LIBS})
target_link_libraries(ZCatClient PRIVATE mingw32 SDL2main SDL2 SDL2_ttf ws2_32)

#导入库和静态库同目录，链接静态库时能找到，动态库和可执行文件同级目录(windows下会搜索)
#mingw32和ws2_32是系统内置库，所以不用额外配置，mingw32链接优先级最高，其次SDL_main
#客户端源文件采用utf-8字符编码，使用gcc编译器来构建：cmake -G "MinGW Makefiles" ..