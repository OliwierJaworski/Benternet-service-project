cmake_minimum_required(VERSION 3.10.0)
project(server_zmq VERSION 0.1.0 LANGUAGES C CXX)

option(CURL_STATIC_LINKING "Set to ON to build libcurl with static linking."  OFF)

if(CURL_STATIC_LINKING)
    message("-DCURL_STATICLIB [added]")
    add_definitions(-DCURL_STATICLIB)
endif()

add_definitions(-DJSON_USE_IMPLICIT_CONVERSIONS=0)

find_package(CURL REQUIRED)

add_executable(${PROJECT_NAME}  
    server.cpp
    )

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CURL_INCLUDE_DIRS}
    )

target_link_libraries(${PROJECT_NAME} 
    COMMON_LIBS
    ${CURL_LIBRARIES}
    )
