cmake_minimum_required(VERSION 3.10.0)
project(test VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CUDA_COMPILER /usr/local/cuda/bin/nvcc)
set(LLAMA_BUILD_COMMON On)
set(GGML_CUDA On)

# Add subdirectory for external dependencies
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/externals/llama.cpp")

# Define the target (executable) first
add_executable(${PROJECT_NAME}  
    main.cpp
    src/LLMInference.cpp
)

# Add the include directories after defining the target
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} 
${CMAKE_CURRENT_SOURCE_DIR}/src)

# Link libraries after the target is fully defined
set(ALL_LIBS PRIVATE
    common llama ggml
)

target_link_libraries(${PROJECT_NAME} 
    ${ALL_LIBS}
)
