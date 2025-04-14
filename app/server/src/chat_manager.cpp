#include <chat_manager.h>

using namespace Benternet;

chat_manager::chat_manager(){

}

chat_manager::~chat_manager(){

}

chat_manager& 
chat_manager::instance(){
    static chat_manager instance;
    return instance;
}

Socket_t&
chat_manager::Socket(string SockedID, string topic, zmq::socket_type type){
    json topic_tmp = CategoryTopic::OnGet_Template();
    topic_tmp["topic"] = topic;
    topic_tmp["session"] = SockedID;
    return *sockets.Oncreate(topic_tmp, type);
}










