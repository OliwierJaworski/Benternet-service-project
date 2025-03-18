cmake_minimum_required(VERSION 3.10.0)
project(client_zmq VERSION 0.1.0 LANGUAGES C CXX)

add_executable(${PROJECT_NAME}  
    client.cpp
)

target_link_libraries(${PROJECT_NAME} 
    COMMON_LIBS
)
