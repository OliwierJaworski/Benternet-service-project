#include <iostream>
#include "openai.hpp"
#include "external/openai-cpp/include/openai/openai.hpp"

int main(int argc, char* argv[]) {
    openai::start("lm-studio","",true,"http://127.0.0.1:1234/v1/");
    auto chat = openai::chat().create(R"(
        {
            "model": "deepseek-r1-distill-qwen-7b",
            "messages":[{"role":"user", "content":"play game"}],
            "max_tokens": 1000,
            "temperature": 0
        }
        )"_json);
        std::cout << "Response is:\n" << chat.dump(2) << '\n'; 
    std::cout << "hello from main.cpp";
 
    return 0;
}
