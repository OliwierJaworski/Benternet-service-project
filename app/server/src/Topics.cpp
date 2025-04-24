#include "Topics.h"
using namespace Benternet;

void 
BTopics::run(){
    CreateMainThread();
}
void 
BTopics::CreateMainThread(){
    MainTopic TopicData;
    std::string topic = TopicData.recvtopic();

    //receiving element
    Ebuilder.opt(ElemOPT::SOCKCREATE, Element_type::sub);
    Ebuilder.opt(ElemOPT::ENDPOINT, "tcp://benternet.pxl-ea-ict.be:24042");
    Ebuilder.opt(ElemOPT::SOCKOPT, ZMQ_SUBSCRIBE, topic.c_str(), topic.size());
    Ebuilder.AddUnpackMethod(MainTopic::UnpackMethod);
    auto RecvStart = Ebuilder.build();

    //Data Filter element
    Ebuilder.opt(ElemOPT::SOCKCREATE, Element_type::filter);
    Ebuilder.opt(ElemOPT::SOCK_CB, MainTopic::filter_cb);
    auto RecvStart = Ebuilder.build();

    //Send element
    Ebuilder.opt(ElemOPT::SOCKCREATE, Element_type::push);
    Ebuilder.opt(ElemOPT::ENDPOINT, "tcp://benternet.pxl-ea-ict.be:24041");
    Ebuilder.AddPackMethod(MainTopic::PackMethod);
    auto RecvStart = Ebuilder.build();

    Pbuilder.UserDataType<MainTopic>(TopicData); //keep it as last because TopicData might be needed
}

void 
MainTopic::filter_cb(Bbuffer& forwarded_data){
    auto dataCB = std::any_cast<testtype>(forwarded_data.GetUdataV());
    
}

void 
MainTopic::UnpackMethod(std::function<void (zmq::message_t &, std::any &)> deser){

}

void 
MainTopic::PackMethod(std::function<void (zmq::message_t &, std::any &)> deser){

}