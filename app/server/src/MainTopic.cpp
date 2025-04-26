#include <Topics.h>
namespace Benternet{

json MainTopic_info = {
    { "benternet", {
        { "service", {
            { "prefix", "dnd_session>start" },
            { "topic", "dnd_session" },
            { "session", "start" },
            { "message", "" },
            { "delim", ">" },
            { "name", ""},
           // { "id", "service_001" },
            { "status", "active" },
            { "last_heartbeat", "" }
        }},
        { "commands", {
            { "help", "[ CORRECT USAGE: ], topic>session>!**command** : for commands: !commands" },
            { "time", "" },
            { "play!", "" },
            { "message", "" },
            { "delim", ">" },
            { "ActiveUsers", 0 },
            { "Sessions", json::array({
                { {"sessionID", ""}, {"Players", json::array({""})} }
            }) },
            { "id", "service_001" },
            { "status", "active" },
            { "last_heartbeat", "" }
        }},
        { "configuration", {
            { "max_connections", 100 },
            { "timeout", 5000 },
            { "auto_restart", true },
            { "logging_enabled", true },
            { "log_level", "INFO" }
        }},
        { "monitoring", {
            { "uptime", "" },
            { "error_count", 0 },
            { "request_count", 0 },
            { "health_check_interval", 60 }
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

    Ebuilder.opt(ElemOPT::SOCKCREATE, Element_type::sub);
    Ebuilder.opt(ElemOPT::ENDPOINT, "tcp://benternet.pxl-ea-ict.be:24043");
    Ebuilder.opt(ElemOPT::SOCKOPT, ZMQ_SUBSCRIBE, topic.c_str(), topic.size());
    Ebuilder.AddUnpackMethod(MainTopic::UnpackMethod);
    auto RecvStart1 = Ebuilder.build();

    //Data Filter element
    Ebuilder.opt(ElemOPT::SOCKCREATE, Element_type::filter);
    Ebuilder.opt(ElemOPT::SOCK_CB, MainTopic::filter_cb);
    auto processreply = Ebuilder.build();

    //Data Filter element
    Ebuilder.opt(ElemOPT::SOCKCREATE, Element_type::filter);
    Ebuilder.opt(ElemOPT::SOCK_CB, MainTopic::filter_cb);
    auto processreply2 = Ebuilder.build();

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
        std::move(processreply2),
        std::move(sendReply)
    );

    BManager::instance().EnableSingle(pipeline);
}

void 
MainTopic::filter_cb(Bbuffer& forwarded_data){
    
    std::cout << "hello from callback!\n";

    std::cout << forwarded_data.GetUdataV().type().name() << "\n";
    auto data_ = std::any_cast<MainTopic>(forwarded_data.GetUdataV());
    if(data_.err){
        return;
    }
        

}


void 
MainTopic::UnpackMethod(zmq::message_t & message, std::any & data){

    std::string msg_str(static_cast<char*>(message.data()), message.size());
    std::cout << "[ received ] ----[ " << msg_str <<" ]----\n";
    auto& data_ = std::any_cast<MainTopic&>(data);
    try{
        data_.SetServiceFields(msg_str);
        
        if(data_.pick_option(data_.GetFromString("MESSAGE",msg_str),data_.info) == "**INVALID**")
            throw std::runtime_error("GetFromString: Invalid key or insufficient parts");

    }catch(const std::runtime_error& err){
        data_.err=1;
        std::string err_str = err.what();
        if(err_str == "GetFromString: Invalid key or insufficient parts"){
            data_.Processed_Data = data_.GetFromString("PREFIX",msg_str)+">" + "[ Not supported content ] :: " + data_.GetHelp();
            std::cout << data_.Processed_Data << std::endl;
            return;
        }
    }
    message.size();
}

string 
MainTopic::pick_option(const string& optionstring, json& info_){
    if (optionstring.empty() || optionstring[0] != '!') {
        return "**INVALID**";
    }

    std::string command = optionstring.substr(1); // remove '!'

    for (auto& [key, val] : info_["benternet"]["service"].items()) {
        if (key == command) {
            return command;
        }
    }

    return "**INVALID**";
}

void 
MainTopic::PackMethod(zmq::message_t &message, std::any &data){
    auto& data_ = std::any_cast<MainTopic&>(data);

    message.rebuild(data_.Processed_Data.size());
    memcpy(message.data(), data_.Processed_Data.c_str(), data_.Processed_Data.size());

    std::string msg_str(static_cast<char*>(message.data()), message.size());
    std::cout << "[ Sending ] ----[ " << msg_str <<" ]----\n";
}
};