cmake_minimum_required(VERSION 3.16)
project(VidiQtServer)

find_package(Qt6 REQUIRED COMPONENTS Core Network)

set(CMAKE_AUTOMOC ON)

set(SERVER_SOURCES
    src/main.cpp
    src/core/Server.cpp
    src/network/ClientConnection.cpp
)

set(SERVER_HEADERS
    src/core/Server.h
    src/network/ClientConnection.h
)

add_executable(${PROJECT_NAME} ${SERVER_SOURCES} ${SERVER_HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core
    ${CMAKE_CURRENT_SOURCE_DIR}/src/network
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Network)