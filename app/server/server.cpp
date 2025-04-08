#include <string>
#include <chrono>
#include <thread>
#include <iostream>

#include <zmq.hpp>
#include <openai.hpp>

using namespace nlohmann;

int main() 
{
    using namespace std::chrono_literals;

    // initialize the zmq context with a single IO thread
    zmq::context_t context{1};

    openai::start("lm-studio","",true,"http://127.0.0.1:1234/v1/");
    std::string chatresponse="";

    json uquery = R"(
        {
            "model": "deepseek-r1-distill-qwen-7b",
            "messages":[{"role":"user", "content":" "}],
            "max_tokens": 2000,
            "temperature": 0
        }
        )"_json;

    // construct a REP (reply) socket and bind to interface
    zmq::socket_t socket{context, zmq::socket_type::rep};

    socket.bind("tcp://*:5555");
    



    // prepare some static data for responses
    const std::string data{"World"};

    for (;;) 
    {
        zmq::message_t request;

        // receive a request from client
        socket.recv(request, zmq::recv_flags::none);
        std::cout << "Received " << request.to_string() << std::endl;

        uquery["messages"].push_back({{"role","user"},{"content",request.to_string()}});

        std::cout << "Received " << request.to_string() << std::endl;

        auto chat = openai::chat().create(uquery);

        chatresponse= chat["choices"][0]["message"]["content"].dump();
        size_t start_conv = chatresponse.rfind("</think>");
        chatresponse = chatresponse.substr(start_conv + std::string("</think>").length());

        socket.send(zmq::buffer(chatresponse) , zmq::send_flags::none);
    }

    return 0;
}
