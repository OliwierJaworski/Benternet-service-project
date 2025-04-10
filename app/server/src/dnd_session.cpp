#include "dnd_session.h"

dnd_session& dnd_session::start(){
    static dnd_session instance;
    return instance;
}

dnd_session::dnd_session(){
    auto socket = make_unique<CategorySocket>(*this, topic_template, socket_type::sub);
    sockets.push_back(std::move(socket));;
}

dnd_session::~dnd_session(){
}

const string CategoryTopic::to_string() const {
        std::string tmp="";

        if (isJson(topic_.dump())){
            string delim = topic_[0]["delim"].get<string>();

            tmp += topic_[0]["topic"].get<string>() + delim; 
            tmp += topic_[0]["session"].get<string>() + delim; 
            tmp += (topic_[0]["message"].get<string>().empty())?"":topic_[0]["message"].get<string>() + delim ; 

            cout << "to_string topic: " << tmp << std::endl; 
            return tmp; 

        }else{
            std::cerr << "Response is not a valid JSON";
            exit(1);
        }
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
        default:
            std::cerr << "no type case for current type" << std::endl;
            exit(1);
    };
    session.MessageSystem.PollingAddEvent((void*)socket.get(),ZMQ_POLLIN);
}

CategorySocket::~CategorySocket() {
    session_.MessageSystem.PollingRemoveEvent((void*)socket.get());
}

CategorySocket& dnd_session::socket(std::string SocketID){
    for(auto &&currentSocket : sockets){ //accepts both rvalue and lvalue arguments & only accepts lvalue

    }
    //socketfind()-> zoek door sockets naar socket
    //return socket;-> socket().get().set().kill()
}

void CategoryMessageSystem::PollEvents(){
    if(zmq_poll(items.data(),items.size(),250) == -1){
        std::cerr << "error when polling events" << std::endl;
        exit(1);
    }
    for(auto item : items){
        if(item.revents & ZMQ_POLLIN){

        }
    }
} 

void CategoryMessageSystem::PollingAddEvent(void* socket, short events){
    zmq::pollitem_t tmp_item {socket,0,events,0};
    items.push_back(tmp_item);
}

void CategoryMessageSystem::PollingRemoveEvent(void* socket){
    int position=0;
    for(auto item : items){
        if(item.socket == socket){
            items.erase(items.begin()+position);;
            return;
        }
        position++;
    } 
}

bool CategoryTopic::isJson(const std::string &data){
    bool rc = true;
    try {
        auto json = json::parse(data); // throws if no json 
    }
    catch (std::exception &){
        rc = false;
    }
    return(rc);
}


