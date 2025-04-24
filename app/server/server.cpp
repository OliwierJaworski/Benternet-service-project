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

void cb_func(Bbuffer& forwarded_data); 
void serializemethod(zmq::message_t&, std::any&);
void DeserializeMethod(zmq::message_t&, std::any&);

int main() {
    EFactory Elembuilder{*BManager::context()};
    PFactory PipelineBuilder{*BManager::context()};

    auto any_data = std::make_shared<std::any>(testtype{});

    PipelineBuilder.AddData(any_data);
    Pipeline_W pipeline = PipelineBuilder.build();

    std::string topic ="dnd_session>start?>";

    std::cout << "recv creation!\n";
    Elembuilder.opt(ElemOPT::SOCKCREATE, Element_type::sub);
    Elembuilder.opt(ElemOPT::ENDPOINT, "tcp://benternet.pxl-ea-ict.be:24042");
    Elembuilder.opt(ElemOPT::SOCKOPT, ZMQ_SUBSCRIBE, topic.c_str(), topic.size());
    Elembuilder.AddPackMethod(DeserializeMethod);
    auto RecvStart = Elembuilder.build();

    std::cout << "filter creation!\n";
    Elembuilder.opt(ElemOPT::SOCKCREATE, Element_type::filter);
    Elembuilder.opt(ElemOPT::SOCK_CB, cb_func);
    auto processreply = Elembuilder.build();

    std::cout << "send creation!\n";
    Elembuilder.opt(ElemOPT::SOCKCREATE, Element_type::push);
    //builder.opt(ElemOPT::ENDPOINT, "tcp://localhost:5554");
    Elembuilder.opt(ElemOPT::ENDPOINT, "tcp://benternet.pxl-ea-ict.be:24041");
    Elembuilder.AddPackMethod(serializemethod);
    auto sendReply = Elembuilder.build();

    pipeline.ElementLink(
        std::move(RecvStart),
        std::move(processreply),
        std::move(sendReply)
    );

   

    BManager::instance().EnableSingle(pipeline);
    BManager::instance().Run();
    //BManager::instance().stopSingle(pipeline);
    BManager::instance().shutdown();
    return 0;
}



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

void serializemethod(zmq::message_t&, std::any&){

}

void DeserializeMethod(zmq::message_t&, std::any&){

}