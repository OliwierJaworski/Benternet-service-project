#include <string>
#include <chrono>
#include <thread>
#include <iostream>

#include <zmq.hpp>
#include <openai.hpp>
#include <BManager.h>

std::string cb_func() {
    return "hello from callback!";
}

int main() {
    std::shared_ptr<Pipeline_T> pipeline;
    EFactory builder{*BManager::context()};

    std::string topic ="topictest";
    builder.opt(ElemOPT::SOCKCREATE, Element_type::sub);
    builder.opt(ElemOPT::ENDPOINT, "tcp://localhost:5555");
    builder.opt(ElemOPT::SOCKOPT, ZMQ_SUBSCRIBE, topic.c_str(), topic.size());
    builder.opt(ElemOPT::SOCK_CB, cb_func);
    auto socket_sub = builder.build();
    auto socket_sub1 = builder.build();
    auto socket_sub2 = builder.build();
    
    pipeline->ElementLink(
        std::move(socket_sub),
        std::move(socket_sub1),
        std::move(socket_sub2)
    );
    return 0;
}

