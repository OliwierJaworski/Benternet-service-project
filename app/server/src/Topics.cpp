#include "Topics.h"
using namespace Benternet;

void 
BTopics::run(){
    CreateMainThread();
    BManager::instance().Run();
    BManager::instance().shutdown();
}

void 
BTopics::CreateMainThread(){
    MainTopic TopicData;
    std::string topic = TopicData.recvtopic();
    std::cout << "\n"<<topic<< "\n";
    //receiving element
    Ebuilder.opt(ElemOPT::SOCKCREATE, Element_type::sub);
    Ebuilder.opt(ElemOPT::ENDPOINT, "tcp://benternet.pxl-ea-ict.be:24042");
    Ebuilder.opt(ElemOPT::SOCKOPT, ZMQ_SUBSCRIBE, topic.c_str(), topic.size());
    Ebuilder.AddUnpackMethod(MainTopic::UnpackMethod);
    auto RecvStart = Ebuilder.build();

    //Data Filter element
    Ebuilder.opt(ElemOPT::SOCKCREATE, Element_type::filter);
    Ebuilder.opt(ElemOPT::SOCK_CB, MainTopic::filter_cb);
    auto processreply = Ebuilder.build();

    //Send element
    Ebuilder.opt(ElemOPT::SOCKCREATE, Element_type::push);
    Ebuilder.opt(ElemOPT::ENDPOINT, "tcp://benternet.pxl-ea-ict.be:24041");
    Ebuilder.AddPackMethod(MainTopic::PackMethod);
    auto sendReply = Ebuilder.build();

    Pbuilder.UserDataType<MainTopic>(TopicData); //keep it as last because TopicData might be needed
    Pipeline_W pipeline = Pbuilder.build(); 

    pipeline.SetIsContinous(true);  //runtime not buildtime variable
    pipelines.push_back( std::move(pipeline) );
    
    pipeline.ElementLink(
        std::move(RecvStart),
        std::move(processreply),
        std::move(sendReply)
    );

    BManager::instance().EnableSingle(pipeline);
}

void 
MainTopic::filter_cb(Bbuffer& forwarded_data){
    std::cout << "hello from callback!\n";

    std::cout << forwarded_data.GetUdataV().type().name() << "\n";
    auto data = std::any_cast<MainTopic>(forwarded_data.GetUdataV());

   /*zmq::message_t datazmq = forwarded_data.GetzmqData();
    std::string tmp = BMessage::ToAnswer(datazmq);
    tmp += "your code is:";
    forwarded_data.rebuild(tmp.size());
    memcpy(forwarded_data.data(), tmp.data(), tmp.size());*/ 

}

void 
MainTopic::UnpackMethod(zmq::message_t & message, std::any & data){
    std::cout << data.type().name() << "\n";
    auto& data_ = std::any_cast<MainTopic&>(data);
    
    message.size();

}

void 
MainTopic::PackMethod(zmq::message_t &message, std::any &data){
    std::cout << data.type().name() << "\n";
    auto& data_ = std::any_cast<MainTopic&>(data);
    message.size();
}