#include <iostream>
#include <string>
#include <chrono>
#include <thread>

#include <zmq.hpp>
#include "LLMInference.h"

int main() 
{
    using namespace std::chrono_literals;

    std::string modelPath = "../../gemma-3-1b-it-Q2_K.gguf";
    float temperature = 1.0f;
    float minP = 0.05f;
    std::unique_ptr<LLMInference> llmInference = std::make_unique<LLMInference>();
    llmInference->loadModel(modelPath, minP, temperature);

    llmInference->addChatMessage("You are a helpful assistant", "system");

    // initialize the zmq context with a single IO thread
    zmq::context_t context{1};

    // construct a REP (reply) socket and bind to interface
    zmq::socket_t socket{context, zmq::socket_type::rep};
    socket.bind("tcp://*:5555");

    std::string predicted_message ="";
    std::cout << "initialized socket etc..." << std::endl;
    for (;;) 
    {
        zmq::message_t request;

        // receive a request from client
        socket.recv(request, zmq::recv_flags::none);
        std::cout << "Received " << request.to_string() << std::endl;
       
        llmInference->startCompletion(request.to_string());

        std::string predictedToken;
        while ((predictedToken = llmInference->completionLoop()) != "[EOG]") {
            predicted_message +=predictedToken;
        }
        // send the reply to the client
        socket.send(zmq::buffer(predicted_message), zmq::send_flags::none);
        predicted_message.clear();
    }

    return 0;
}

