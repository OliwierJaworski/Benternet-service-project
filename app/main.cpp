#include <iostream>
#include "openai.hpp"
#include "external/openai-cpp/include/openai/openai.hpp"

int main(int argc, char* argv[]) {
    openai::start("mykey","myorg");
    std::cout << "hello from main.cpp";
 
    return 0;
}
