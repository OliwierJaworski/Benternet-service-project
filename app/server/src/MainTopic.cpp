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
            { "!DontUseFirstEntry!", "" },
            { "!commands", 
                "\n\033[1;34m!about\033[0m: \033[1;32mservice info.\033[0m\n"
                "\033[1;34m!help\033[0m: \033[1;32mUsage help and syntax guide.\033[0m\n"
                "\033[1;34m!play\033[0m: \033[1;32mStarts a new DND session.\033[0m\n"
                "\033[1;34m!time\033[0m: \033[1;32mReturns the current server time.\033[0m\n"
                "\033[1;34m!delim\033[0m: \033[1;32mShows the current delimiter.\033[0m\n"
                "\033[1;34m!ActiveUsers\033[0m: \033[1;32mShows the number of currently active users.\033[0m\n"
                "\033[1;34m!Sessions\033[0m: \033[1;32mLists current sessions and players.\033[0m\n"
                "\033[1;34m!id\033[0m: \033[1;32mDisplays the service ID.\033[0m\n"
                "\033[1;34m!status\033[0m: \033[1;32mDisplays the current service status.\033[0m\n"
                "\033[1;34m!last_heartbeat\033[0m: \033[1;32mShows the last heartbeat timestamp (Function type).\033[0m\n"
            },
            { "!about", "\n\033[1;34mService for creating/joining user made lobbies. to play DND with an AI dungeon master\033[0m" },
            { "!help", "\n\\033[1;34m[ CORRECT USAGE: ], topic>session>!**command** : for commands: !commands\033[0m" },
            { "!time", "**FUNCTYPE**" },
            { "!play!", "**FUNCTYPE**" },
            { "!delim", "\n\\033[1;34m>\033[0m" }, // won't be allowed yet to change delim
            { "!ActiveUsers", 0 },
            { "!Sessions", json::array({
                { {"sessionID", ""}, {"Players", json::array({""})} }
            }) },
            { "!id", "\n\033[1;34mDND-lobby\033[0m" },
            { "!status", "\n\\033[1;34mactive\033[0m" },
            { "!last_heartbeat", "**FUNCTYPE**" }
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
MainTopic::CreateDNDSession(){
   
}

void 
MainTopic::UnpackMethod(zmq::message_t & message, std::any & data){

    std::string msg_str(static_cast<char*>(message.data()), message.size());
    std::cout << "[ received ] ----[ " << msg_str <<" ]----\n";
    auto& data_ = std::any_cast<MainTopic&>(data);
    
    try {
        std::string msg_str(static_cast<char*>(message.data()), message.size());
        std::cout << "[ received ] ----[ " << msg_str <<" ]----\n";
        
        auto& data_ = std::any_cast<MainTopic&>(data);

        try {
            data_.SetServiceFields(msg_str, '!');
            data_.Processed_Data = data_.pick_option(data_.GetFromString("MESSAGE", msg_str), data_.info);
        } 
        catch (const std::runtime_error& err) {
            data_.err = 1;
            std::string err_str = err.what();
            if (err_str == "[VARVALUE] Invalid message appended") {
                data_.Processed_Data =  data_.GetTopic() + data_.GetDelim() + 
                                        data_.GetSession() + data_.GetDelim() +
                                        data_.GetID() + data_.GetDelim() +
                                        "[ Not supported content ] :: " + data_.GetHelp();
                std::cout << data_.Processed_Data << std::endl;
                return;
            }
        }
    } catch (const std::runtime_error& err) {
        std::cout << "[FATAL ERROR] UNABLE TO RECOVER FROM FOLLOWING ERROR:\n" << std::endl;
        std::string err_str = err.what();
        exit(1);
    }
    message.size();
}

void 
MainTopic::filter_cb(Bbuffer& forwarded_data){
    
    std::cout << "hello from callback!\n";

    std::cout << forwarded_data.GetUdataV().type().name() << "\n";
    auto& data_ = std::any_cast<MainTopic&>(forwarded_data.GetUdataV());
    if(data_.err){
        return;
    }
    //!!!!!!!!try catch for: [json.exception.type_error.302] type must be string, but is null
    data_.ExecCommand(data_.Processed_Data);
}

void 
MainTopic::PackMethod(zmq::message_t &message, std::any &data){
    auto& data_ = std::any_cast<MainTopic&>(data);

    //if wrong argument was provided
    if(data_.err){
        message.rebuild(data_.Processed_Data.size());
        memcpy(message.data(), data_.Processed_Data.c_str(), data_.Processed_Data.size());
        return;
    }

    string forwarded = data_.Processed_Data;
    std::cout << "forwarded: "<<forwarded << "\n";

    data_.Processed_Data =  data_.GetTopic() + data_.GetDelim() + 
                                        data_.GetSession() + data_.GetDelim() +
                                        data_.GetID() + data_.GetDelim() +
                                        forwarded;
                                        
    message.rebuild(data_.Processed_Data.size());
    memcpy(message.data(), data_.Processed_Data.c_str(), data_.Processed_Data.size());

    std::string msg_str(static_cast<char*>(message.data()), message.size());
    std::cout << "[ Sending ] ----[ " << msg_str <<" ]----\n";
}

string 
MainTopic::pick_option(const string& optionstring, json& info_){
    std::cout << "[pick_option] Trying to find option: " << optionstring << "\n";

    if (optionstring.empty() || optionstring[0] != '!') {
        throw std::runtime_error("[VARVALUE] Invalid message appended");
    }

    for (auto& [key, val] : info_["benternet"]["commands"].items()) {
        std::cout << "[pick_option] Checking key: " << key << "\n";
        if (key == optionstring) {
            return optionstring;
        }
    }

    throw std::runtime_error("[VARVALUE] Invalid message appended"); //command not supported 
}

void
MainTopic::ExecCommand(std::string& command){
   string value =info["benternet"]["commands"][command].get<std::string>();
   
   if(value.find("**FUNCTYPE**") != string::npos){
        command_table[command](*this);
   }else{
    
        Processed_Data = value;
        std::cout << "forwarded: "<<Processed_Data << "\n";
   }
}

void 
MainTopic::CT_play(MainTopic& data){
 //big big hassle :(
}

};