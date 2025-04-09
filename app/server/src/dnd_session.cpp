#include "dnd_session.h"

dnd_session::dnd_session(){
    auto socket = make_unique<CategorySocket>(*this, topic_template, socket_type::sub);
    sockets.push_back(std::move(socket));;
}

dnd_session::~dnd_session(){
}

void CategoryTopic::attach_socket(CategorySocket* socket){
    this->session_ = socket;
}

string CategoryTopic::to_string()const{
    string tmp="";
    string delim = topic_["delim"].get<string>();
    tmp += topic_["topic"].get<string>() + delim; 
    tmp += topic_["session"].get<string>() + delim; 
    tmp += topic_["session"].get<string>() + delim; 
    return tmp; 
}

CategorySocket::CategorySocket(dnd_session& session, json topic, zmq::socket_type type) 
: session_(session), socket(std::make_unique<zmq::socket_t>(session.context.get_context(),type)){

    std::lock_guard<std::mutex> lock(session.topics_lock);
    session.topics.push_back(CategoryTopic(topic));
    topic_ = &session.topics.back();

    string tmp = topic_->to_string();
    switch(type){
        case socket_type::sub :
            socket->setsockopt(ZMQ_SUBSCRIBE, tmp.c_str(), tmp.size());
            break;
        case socket_type::pull:
            break;
        case socket_type::push :
            break;
 
 
    }
}




