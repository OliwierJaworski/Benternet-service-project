cmake_minimum_required(VERSION 3.10.0)
project(test VERSION 0.1.0 LANGUAGES C CXX)

find_package(cppzmq)

set(CMAKE_CUDA_COMPILER /usr/local/cuda/bin/nvcc)
set(LLAMA_BUILD_COMMON On)
set(GGML_CUDA On)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/externals/llama.cpp")

add_executable(${PROJECT_NAME}  
    main.cpp
    src/LLMInference.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} 
${CMAKE_CURRENT_SOURCE_DIR}/src)


set(LLM_LIBS PRIVATE
    common llama ggml
)
set(CONNECTION_LIBS PRIVATE
    cppzmq
)


target_link_libraries(${PROJECT_NAME} 
    ${LLM_LIBS}
    ${CONNECTION_LIBS}
)
