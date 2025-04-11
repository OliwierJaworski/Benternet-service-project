#include "dnd_session.h"

dnd_session& dnd_session::start(){
    static dnd_session instance;
    return instance;
}

dnd_session::dnd_session(){
    create_socket(socket_type::sub, topic_template); //initial socket which will be listening for requests
}

dnd_session::~dnd_session(){
}

CategorySocket* dnd_session::create_socket(zmq::socket_type socktype, json topic){
    auto socket = make_unique<CategorySocket>(*this, topic, socktype);
    sockets.push_back(std::move(socket));
    return sockets.back().get();
}

CategorySocket& dnd_session::socket(std::string SocketID, socket_type type){
    for(auto &&currentSocket : sockets){ //accepts both rvalue and lvalue arguments & only accepts lvalue
        cout << "checking for socket session"<< currentSocket.get()->get_session() << endl;
        if(currentSocket.get()->get_session() == SocketID){ //--> error is here 
            cout <<"socket called : |" << SocketID << "| found!" << endl;
            return *currentSocket;
        }
    }
    
    cout <<"no socket found called: |" << SocketID << "|, creating new socket!" << std::endl;
    json tmp = topic_template;
    tmp["session"]= SocketID;
    return *create_socket(type,tmp);
}
    
CategorySocket::CategorySocket(dnd_session& session, json topic, zmq::socket_type type) 
: session_(session), socket(std::make_unique<zmq::socket_t>(session.context.get_context(),type)){
     
    std::lock_guard<std::mutex> lock(session.topics_lock);
    topic_ =  make_unique<CategoryTopic>(topic); //fixed misalignment by making shared ptr
    session.topics.push_back(topic_);
   

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

string CategoryTopic::to_string(){
    std::string tmp="";
    topic_= topic_[0]; // weird behavior where json object is changed to arr of objects this is workaround :/
    
    if (topic_.is_object()){
        string delim = topic_["delim"].get<string>();

        tmp += topic_["topic"].get<string>() + delim; 
        tmp += topic_["session"].get<string>() + delim; 
        tmp += (topic_["message"].get<string>().empty())?"":topic_["message"].get<string>() + delim ; 

        cout << "to_string topic: " << tmp << std::endl; 
        return tmp; 

    }else{
        std::cerr << "where " << "| CategoryTopic::to_string() |" <<" Response is not a valid object";
        exit(1);
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


