#include "Topics.h"

namespace Benternet{

json DNDtopic_info = {
    { "benternet", {
        { "service", {
            { "prefix", "" },
            { "topic", "dnd_session" },
            { "session", "" },
            { "message", "" },
            { "delim", ">" },
            { "name", ""},
            { "status", "active" },
            { "last_heartbeat", "" }
        }},
        { "commands", {
            { "!DontUseFirstEntry!", "due to bug the first command will not send" },
            { "!about", "" },
            { "!help", "[ CORRECT USAGE: ], topic>session>!**command** : for commands: !commands" },
            { "!time", "**FUNCTYPE**"},
            { "!play!", "**FUNCTYPE**" },
            { "!ActiveUsers", 0 },
        }},
        { "dependencies", {
            { "**NOVALUE**", {
                { "topic", "" },
                { "session", "" },
                { "argument", "" },
                { "Vartype", "" }
            }}
        }}
    }}
};   

void 
DndTopic::Process( Bbuffer& forwarded_data ){
    std::string chatresponse="";
    auto& data_ = std::any_cast<DndTopic&>(forwarded_data.GetUdataV());
    ;
    json uquery = R"(
        {
            "model": "llama-3.2-1b-instruct",
            "messages":[{"role":"user", "content":" "}],
            "max_tokens": 2000,
            "temperature": 0,
            "session_id": ""
        }
        )"_json;
    uquery["messages"].push_back({{"role","user"},{"content",data_.Processed_Data}});
    uquery["session_id"] = data_.GetID();
    auto chat = openai::chat().create(uquery);
    data_.Processed_Data = chat["choices"][0]["message"]["content"].dump();
}

void 
DndTopic::UnpackMethod(zmq::message_t &message, std::any &data){
    std::string msg_str(static_cast<char*>(message.data()), message.size());
    auto& data_ = std::any_cast<DndTopic&>(data);
    
    data_.Processed_Data = data_.GetFromString("MESSAGE",msg_str);
}

void 
DndTopic::PackMethod(zmq::message_t &message, std::any &data){
    auto& data_ = std::any_cast<DndTopic&>(data);
    string send = data_.GetTopic() + ">" + data_.GetSession() + "!>" + data_.GetID() + ">" + data_.Processed_Data;
    message.rebuild(send.size());
    memcpy(message.data(), send.c_str(), send.size());
}

void 
Dnd_MP::Process( Bbuffer& forwarded_data ){
    std::vector<string> Pnames;
    std::vector<Element_T> recv_ch;
    
    std::regex u_template(R"(\[([^\]]+)\])");
    std::smatch match;

    auto& data_ = std::any_cast<DndTopic&>(forwarded_data.GetUdataV());
    std::string topic = data_.recvtopic()+ data_.GetID() +">";
    std::string ns{data_.Processed_Data};

    auto it = ns.cbegin();
	while (std::regex_search(it, ns.cend(), match, u_template)) {
        Pnames.push_back(match[1]); 
        it = match.suffix().first;
    }

    if(Pnames.size() == 0) {  throw std::runtime_error("[VARVALUE] provided 0 user names"); }
    
    //EFactory Ebuilder { *BManager::context() };
    //PFactory Pbuilder { *BManager::context() };

    for(auto id: Pnames){
     /*   Ebuilder.opt(ElemOPT::SOCKCREATE, Element_type::sub);
        //Ebuilder.opt(ElemOPT::ENDPOINT, "tcp://benternet.pxl-ea-ict.be:24042");
        Ebuilder.opt(ElemOPT::ENDPOINT, "tcp://127.0.0.1:24042");
        Ebuilder.opt(ElemOPT::SOCKOPT, ZMQ_SUBSCRIBE, topic.c_str(), topic.size());
        Ebuilder.AddUnpackMethod(Dnd_MP::UnpackMethod_Setup);
        auto RecvStart = Ebuilder.build();*/
    }
}

void 
Dnd_MP::UnpackMethod(zmq::message_t &message, std::any &data){
   

    std::string msg_str(static_cast<char*>(message.data()), message.size());
    auto& data_ = std::any_cast<Dnd_MP&>(data);
    
    data_.Processed_Data = data_.GetFromString("MESSAGE",msg_str);
}

void 
Dnd_MP::PackMethod(zmq::message_t &message, std::any &data){
   
}

};