cmake_minimum_required(VERSION 3.20)
project(DesignToolsPro VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC_PATH_PREFIX OFF)

# 禁用响应文件，直接在命令行传递 include 路径
set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_INCLUDES OFF)
set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_OBJECTS OFF)
set(CMAKE_C_USE_RESPONSE_FILE_FOR_INCLUDES OFF)
set(CMAKE_C_USE_RESPONSE_FILE_FOR_OBJECTS OFF)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(SOURCES
    src/main.cpp
    src/pages/ColorPickerPage.cpp
    src/pages/RgbColorTablePage.cpp
)

set(HEADERS
    src/pages/ColorPickerPage.h
    src/pages/RgbColorTablePage.h
)

add_executable(DesignToolsPro WIN32 ${SOURCES} ${HEADERS} resources/resources.qrc resources/app.rc)

target_include_directories(DesignToolsPro PRIVATE src)
target_link_libraries(DesignToolsPro PRIVATE Qt6::Widgets)

if(WIN32)
    target_compile_definitions(DesignToolsPro PRIVATE UNICODE _UNICODE)
    target_compile_options(DesignToolsPro PRIVATE
        -finput-charset=UTF-8 -fexec-charset=UTF-8
        -fvisibility=hidden -fvisibility-inlines-hidden
        -O2 -DNDEBUG
    )
    target_link_options(DesignToolsPro PRIVATE
        -s -Wl,--gc-sections -Wl,--as-needed
        -Wl,--nxcompat -Wl,--dynamicbase
        -Wl,--high-entropy-va
    )
endif()

if(WIN32)
    set(MINGW_BIN "D:/Qt/Tools/mingw1310_64/bin")
    set(QT_PLATFORMS "${Qt6_DIR}/../../../plugins/platforms")
    add_custom_command(TARGET DesignToolsPro POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${Qt6_DIR}/../../../bin/Qt6Core.dll"
            "${Qt6_DIR}/../../../bin/Qt6Gui.dll"
            "${Qt6_DIR}/../../../bin/Qt6Widgets.dll"
            "${MINGW_BIN}/libgcc_s_seh-1.dll"
            "${MINGW_BIN}/libstdc++-6.dll"
            "${MINGW_BIN}/libwinpthread-1.dll"
            $<TARGET_FILE_DIR:DesignToolsPro>
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:DesignToolsPro>/platforms"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${QT_PLATFORMS}/qwindows.dll"
            "$<TARGET_FILE_DIR:DesignToolsPro>/platforms/qwindows.dll"
    )
endif()
