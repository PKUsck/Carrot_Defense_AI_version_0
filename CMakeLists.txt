cmake_minimum_required(VERSION 3.16)
project(CarrotDefense VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)      # 原标准中显式开启，建议补上
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC OFF)     # 如果你没有 .qrc 文件可以关闭

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------- 通用 Qt 查找（不写死路径） ----------
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    gamescene.cpp
    enemy.cpp
    tower.cpp
    projectile.cpp
    wave.cpp
)

set(PROJECT_HEADERS
    mainwindow.h
    gamescene.h
    enemy.h
    tower.h
    projectile.h
    wave.h
    node.h
    pathfinder.h
)

# ---------- 可执行文件创建（兼容 Qt6 的最终化步骤） ----------
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(CarrotDefense
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${PROJECT_HEADERS}
    )
else()
    add_executable(CarrotDefense
        ${PROJECT_SOURCES}
        ${PROJECT_HEADERS}
    )
endif()

target_link_libraries(CarrotDefense PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
)

# ---------- 部署 Qt 依赖（不写死路径） ----------
# 方法一：使用 find_program 寻找 windeployqt（适用于 Qt5/Qt6）
if(WIN32)
    find_program(WINDEPLOYQT_EXECUTABLE
        NAMES windeployqt
        HINTS ${Qt${QT_VERSION_MAJOR}_DIR}/../../../bin   # 利用 Qt 安装目录线索
              ${CMAKE_PREFIX_PATH}/bin
    )
    if(WINDEPLOYQT_EXECUTABLE)
        add_custom_command(TARGET CarrotDefense POST_BUILD
            COMMAND "${WINDEPLOYQT_EXECUTABLE}"
                --qmldir "${CMAKE_SOURCE_DIR}"
                "$<TARGET_FILE:CarrotDefense>"
            COMMENT "Running windeployqt to copy Qt DLLs..."
        )
    else()
        message(WARNING "windeployqt not found; Qt DLLs will not be copied automatically")
    endif()
endif()

# 方法二（更现代，Qt6 推荐）：使用 qt_generate_deploy_app_script
# 需要 Qt6 且设置 INSTALL 部署，这里简单展示概念，可根据需要启用
# if(QT_VERSION_MAJOR EQUAL 6)
#     include(DeployQt6)
#     qt_generate_deploy_app_script(
#         TARGET CarrotDefense
#         OUTPUT_SCRIPT deploy_script
#         NO_UNSUPPORTED_PLATFORM_ERROR
#     )
#     install(SCRIPT ${deploy_script})
# endif()

# ---------- 可选：安装规则（与第一个标准保持一致） ----------
include(GNUInstallDirs)
install(TARGETS CarrotDefense
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# ---------- Qt6 最终化（必须放在最后） ----------
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(CarrotDefense)
endif()
