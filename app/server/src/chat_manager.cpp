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
chat_manager::Socket(string SockedID, zmq::socket_type type, string topic){
    json topic_tmp = CategoryTopic::OnGet_Template();
    topic_tmp["topic"] = topic;
    topic_tmp["session"] = SockedID;
    return *sockets.Oncreate(topic_tmp, type);
}

string 
CategoryTopic::to_string( json topic_){
    std::string tmp="";    
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










