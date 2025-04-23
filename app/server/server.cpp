#include <string>
#include <chrono>
#include <thread>
#include <iostream>

#include <zmq.hpp>
#include <openai.hpp>
#include <BManager.h>
struct testtype{
    int data{22};
    std::string somestring{"hello from custom data structure!"};
};

void cb_func(Bbuffer& forwarded_data) {
    auto data = std::any_cast<testtype>(forwarded_data.GetUdataV());
    std::cout << "value of testtype:\n data:" << data.data  << "\nsomestring:" << data.somestring << "\n";
    std::cout << "hello from callback!\n";
   /*zmq::message_t datazmq = forwarded_data.GetzmqData();
    std::string tmp = BMessage::ToAnswer(datazmq);
    tmp += "your code is:";
    forwarded_data.rebuild(tmp.size());
    memcpy(forwarded_data.data(), tmp.data(), tmp.size());*/ 
}

int main() {
    testtype data{1};
    std::shared_ptr<Pipeline_T> pipeline = std::make_shared<Pipeline_T>(*BManager::context(),nullptr,nullptr,data);
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
    //builder.opt(ElemOPT::ENDPOINT, "tcp://localhost:5554");
    builder.opt(ElemOPT::ENDPOINT, "tcp://benternet.pxl-ea-ict.be:24041");
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

