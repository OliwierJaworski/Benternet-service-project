#include <dnd_session.h>

using namespace Benternet;

chat_manager::chat_manager(){

}

chat_manager::~chat_manager(){

}

Socket_t&
chat_manager::Socket(string endpoint, zmq::socket_type type, string session, string topic){
    json topic_tmp = CategoryTopic::Get_Template();
    topic_tmp["topic"] = topic;
    topic_tmp["session"] = session;
    return *sockets.create(topic_tmp, type);
}

chat_manager& 
chat_manager::instance(){
    static chat_manager instance;
    return instance;
}

shared_ptr<Socket_t> 
CategorySocket::create(json topic, zmq::socket_type type){
    for(auto &&currentSocket : sockets_V){
        if( CategoryTopic::Get_Session( currentSocket->GetTopic() ) == topic["session"].get<string>() ){
            return currentSocket;
        }
    }

    auto tmpV_socket_t = make_shared<Socket_t>(topic, type);
    sockets_V.push_back(tmpV_socket_t);
    return tmpV_socket_t;
}

