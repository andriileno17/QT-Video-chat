cmake_minimum_required(VERSION 3.16)
project(VidiQtClient)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network Multimedia)

set(CMAKE_AUTOMOC ON)

set(CLIENT_SOURCES
    src/main.cpp
    src/ui/MainWindow.cpp
    src/network/NetworkManager.cpp
    src/media/CameraCapture.cpp
)

set(CLIENT_HEADERS
    src/ui/MainWindow.h
    src/network/NetworkManager.h
    src/media/CameraCapture.h
)

add_executable(${PROJECT_NAME} ${CLIENT_SOURCES} ${CLIENT_HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui
    ${CMAKE_CURRENT_SOURCE_DIR}/src/network
    ${CMAKE_CURRENT_SOURCE_DIR}/src/media
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    Qt6::Core 
    Qt6::Gui 
    Qt6::Widgets 
    Qt6::Network 
    Qt6::Multimedia
)