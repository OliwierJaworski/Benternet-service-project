#include <string>
#include <chrono>
#include <thread>
#include <iostream>

#include <zmq.hpp>
#include <openai.hpp>
#include <BManager.h>

void cb_func(zmq::message_t& forwarded_data) {
    std::cout << "hello from callback!\n";
    std::cout << static_cast<char*>(forwarded_data.data()) << "\n";
}

int main() {
    std::shared_ptr<Pipeline_T> pipeline = std::make_shared<Pipeline_T>(*BManager::context());
    EFactory builder{*BManager::context()};

    std::string topic ="dnd_session>start?>";

    std::cout << "recv creation!\n";
    builder.opt(ElemOPT::SOCKCREATE, Element_type::sub);
    builder.opt(ElemOPT::ENDPOINT, "tcp://benternet.pxl-ea-ict.be:24042");
    builder.opt(ElemOPT::SOCKOPT, ZMQ_SUBSCRIBE, topic.c_str(), topic.size());
    auto RecvStart = builder.build();

    std::cout << "filter creation!\n";
    builder.opt(ElemOPT::SOCKCREATE, Element_type::filter);
    builder.opt(ElemOPT::SOCK_CB, cb_func);
    auto processreply = builder.build();

    std::cout << "send creation!\n";
    builder.opt(ElemOPT::SOCKCREATE, Element_type::push);
    builder.opt(ElemOPT::ENDPOINT, "tcp://localhost:5554");
    // builder.opt(ElemOPT::ENDPOINT, "tcp://benternet.pxl-ea-ict.be:24041");
    auto sendReply = builder.build();
    
    std::cout << " element linking!\n";
    pipeline->ElementLink(
        std::move(RecvStart),
        std::move(processreply),
        std::move(sendReply)
    );
    std::cout << "finished linking!\n";
    BManager::instance().StartSingle(pipeline);
    BManager::instance().Run();
    BManager::instance().stopSingle(pipeline);
    exit(1);
    return 0;
}

