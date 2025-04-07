cmake_minimum_required(VERSION 3.10.0)
project(test VERSION 0.1.0 LANGUAGES C CXX)

option(CURL_STATIC_LINKING "Set to ON to build libcurl with static linking."  OFF)

if(CURL_STATIC_LINKING)
    message("-DCURL_STATICLIB [added]")
    add_definitions(-DCURL_STATICLIB)
endif()

add_definitions(-DJSON_USE_IMPLICIT_CONVERSIONS=0)

find_package(CURL REQUIRED)

include_directories(${CURL_INCLUDE_DIRS})

# Define the target (executable) first
add_executable(${PROJECT_NAME}  
    main.cpp
)

# Add the include directories after defining the target
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR} 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/external/openai-cpp/include/openai
)

# Link libraries after the target is fully defined
target_link_libraries(${PROJECT_NAME} 
        ${CURL_LIBRARIES}
)
