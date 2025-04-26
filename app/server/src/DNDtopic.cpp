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
            "model": "mn-violet-lotus-12b",
            "messages":[{"role":"user", "content":" "}],
            "max_tokens": 2000,
            "temperature": 0
        }
        )"_json;
    uquery["messages"].push_back({{"role","user"},{"content",data_.Processed_Data}});
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
};