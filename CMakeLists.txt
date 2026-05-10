cmake_minimum_required(VERSION 3.16)
project(CarrotDefense VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

# Qt6 path
set(CMAKE_PREFIX_PATH "D:/Qt/6.11.0/msvc2022_64")

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

set(SOURCES
    main.cpp
    mainwindow.cpp
    gamescene.cpp
    enemy.cpp
    tower.cpp
    projectile.cpp
    wave.cpp
)

set(HEADERS
    mainwindow.h
    gamescene.h
    enemy.h
    tower.h
    projectile.h
    wave.h
    node.h
    pathfinder.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

# Copy Qt DLLs after build
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND "D:/Qt/6.11.0/msvc2022_64/bin/windeployqt.exe"
        --qmldir "${CMAKE_SOURCE_DIR}"
        "$<TARGET_FILE:${PROJECT_NAME}>"
    COMMENT "Running windeployqt to bundle Qt dependencies..."
)
