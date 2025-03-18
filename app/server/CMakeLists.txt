cmake_minimum_required(VERSION 3.10.0)
project(server_zmq VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CUDA_COMPILER /usr/local/cuda/bin/nvcc)
set(LLAMA_BUILD_COMMON On)
set(GGML_CUDA On)

add_subdirectory(
  "${CMAKE_CURRENT_SOURCE_DIR}/../external/llamacpp"  # Source directory (relative to current directory)
  "${CMAKE_CURRENT_BINARY_DIR}/llamacpp"               # Binary directory (relative to current build directory)
)

add_executable(${PROJECT_NAME}  
    server.cpp
    src/LLMInference.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include 
${CMAKE_CURRENT_SOURCE_DIR}/src)

set(LLM_LIBS PRIVATE
    common llama ggml
)

target_link_libraries(${PROJECT_NAME} 
    ${LLM_LIBS}
    COMMON_LIBS
)
