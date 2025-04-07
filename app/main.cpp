#include <iostream>
#include "openai.hpp"
#include "external/openai-cpp/include/openai/openai.hpp"

using namespace nlohmann;
int main(int argc, char* argv[]) {
    openai::start("lm-studio","",true,"http://127.0.0.1:1234/v1/");
    std::string chatresponse="";
    auto  chat = openai::chat().create(R"(
        {
            "model": "deepseek-r1-distill-qwen-7b",
            "messages":[{"role":"user", "content":"i want to play as a oliwier the human"}],
            "max_tokens": 2000,
            "temperature": 0,
            "id": "chatcmpl-aye2e09u94leibtysrwob"
        }
        )"_json);
        
    chatresponse= chat["choices"][0]["message"]["content"].dump();
    size_t start_conv = chatresponse.rfind("</think>");
    chatresponse = chatresponse.substr(start_conv + std::string("</think>").length());
    std::cout << "response :" << chatresponse << std::endl;
    return 0;
}
