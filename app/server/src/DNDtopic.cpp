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

}
void 
DndTopic::UnpackMethod(zmq::message_t &message, std::any &data){

}
void 
DndTopic::PackMethod(zmq::message_t &message, std::any &data){
    auto& data_ = std::any_cast<DndTopic&>(data);
    string send = data_.GetTopic() + ">" + data_.GetSession()  + ">" + data_.GetID() + ">" + "welcome to your dungeon! respond with !play to start";
    message.rebuild(data_.Processed_Data.size());
    memcpy(message.data(), data_.Processed_Data.c_str(), data_.Processed_Data.size());
}
};