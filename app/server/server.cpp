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
    std::unique_ptr <Element_T> socket_sub = builder.build();
    std::unique_ptr <Element_T> socket_sub1 = builder.build();
    std::unique_ptr <Element_T> socket_sub2 = builder.build();
    //socket_sub->process();
    //std::unique_ptr <Element_T> socket_sub2 = builder.build();
    //std::unique_ptr <Element_T> socket_sub3 = builder.build();
    pipeline->ElementLink(
        std::move(socket_sub),
        std::move(socket_sub1),
        std::move(socket_sub2)
    );
    return 0;
}

