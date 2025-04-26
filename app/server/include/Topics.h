#pragma once

#include <openai.hpp>
#include <BManager.h>
#include <zmq.hpp>
#include <nlohmann/json.hpp>
#include <chrono>

namespace Benternet{

#define MaxArgs 3

using json = nlohmann::json;
using string = std::string;
template<typename T>
using vector = std::vector<T>;

class DndTopic;

extern json MainTopic_info;
extern json DNDtopic_info;

struct Topic_template{
    enum MPART{
        TOPIC,
        SESSION,
        NAME,
        MESSAGE
    };

    int err{0};
    
    json info;
    string Processed_Data{""};
    void PrintJson() { std::cout << info.dump(4) << std::endl; }

    string GetCurrentTimestamp();
    string GetFromString(const std::string& key, const std::string& haystack);
    inline void Set_suffix(string& prefix_structure, const char& suffix);
    void SetServiceFields(std::string& input, const char& suffix);
    
    inline string GetTopic()noexcept { return info["benternet"]["service"]["topic"].get<string>(); }
    inline string GetSession()noexcept  { return info["benternet"]["service"]["session"].get<string>(); };
    inline string GetMessage()noexcept  { return info["benternet"]["service"]["message"].get<string>(); };
    inline string GetDelim()noexcept    { return info["benternet"]["service"]["delim"].get<string>(); };
    inline string GetID()noexcept    { return info["benternet"]["service"]["name"].get<string>(); };
    inline string GetServiceStatus()noexcept { return info["benternet"]["service"]["status"].get<string>(); }
    inline string GetLastHeartbeat()noexcept { return info["benternet"]["service"]["last_heartbeat"].get<string>(); }
    inline int GetMaxConnections()noexcept { return info["benternet"]["configuration"]["max_connections"].get<int>(); }
    inline string GetHelp()noexcept { return info["benternet"]["commands"]["!help"].get<string>(); }

    inline void SetTopic(const string topic) { info["benternet"]["service"]["topic"] = topic; }
    inline void SetSession(const string session) { info["benternet"]["service"]["session"] = session; }
    inline void SetMessage(const string message) { info["benternet"]["service"]["message"] = message; }
    inline void SetDelim(const string delim) { info["benternet"]["service"]["delim"] = delim; }
    inline void SetID(const string ID) { info["benternet"]["service"]["name"] = ID; };
    inline void SetServiceStatus(const string status) { info["benternet"]["service"]["status"] = status; }
    inline void SetLastHeartbeat() { info["benternet"]["service"]["last_heartbeat"] = GetCurrentTimestamp(); }
    inline void SetSenderName(const std::string name) { info["benternet"]["service"]["name"] = name;} 

    inline string recvtopic()   { return (GetTopic()+">"+GetSession()+"?>");};
    inline string sendtopic()   { return (GetTopic()+">"+GetSession()+"!>"+GetID()+">");};
    

    Topic_template(json& info_){info = info_;}
};

class BTopics{
public:
    void run();

    static BTopics& instance(){ static BTopics btopic; return btopic; }
    void CreateDNDThread(DndTopic data);

    BTopics(){}
    ~BTopics(){}
private:

    void CreateMainThread();

    EFactory Ebuilder { *BManager::context() };
    PFactory Pbuilder { *BManager::context() };
    vector<Pipeline_W> pipelines;
};

struct MainTopic : Topic_template{
    static void filter_cb( Bbuffer& forwarded_data );
    static void UnpackMethod(zmq::message_t &message, std::any &data);
    static void PackMethod(zmq::message_t &message, std::any &data);
    
    static void CT_time(MainTopic& data){ data.Processed_Data = data.GetCurrentTimestamp(); }
    static void CT_play(MainTopic& data);
    static void CT_last_heartbeat(MainTopic& data){ data.Processed_Data = data.GetLastHeartbeat(); }

    void ExecCommand(std::string& command);
    

    string pick_option(const string& optionstring, json& info_);

    MainTopic() : Topic_template(MainTopic_info){} 

    std::unordered_map<std::string, std::function<void(MainTopic& data)>> command_table{
        {"!time", CT_time},
        {"!play!", CT_play},
        {"!last_heartbeat", CT_last_heartbeat}
    };
};

struct DndTopic : Topic_template{
    static void Process( Bbuffer& forwarded_data );
    static void UnpackMethod(zmq::message_t &message, std::any &data);
    static void PackMethod(zmq::message_t &message, std::any &data);
    
    DndTopic() : Topic_template(DNDtopic_info){} 
};

using MPART = Topic_template::MPART;

};

