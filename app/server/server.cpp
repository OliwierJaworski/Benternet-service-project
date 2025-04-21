#include <string>
#include <chrono>
#include <thread>
#include <iostream>

#include <zmq.hpp>
#include <openai.hpp>
#include <BManager.h>

int main() {
    std::shared_ptr<Pipeline_T> pipelineA;
    EFactory builder{*BManager::context()};

    std::string topic ="topictest";

    builder.opt(ElemOPT::ENDPOINT, "idk.be");
    builder.opt(ElemOPT::SOCKCREATE, Element_type::sub);
    builder.opt(ElemOPT::SOCKOPT, ZMQ_SUBSCRIBE, topic.c_str(), topic.size());
    std::unique_ptr <Element_T> socket_sub = builder.build();
    
    return 0;
}
