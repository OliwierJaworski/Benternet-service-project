#pragma once

#include <openai.hpp>
#include <BManager.h>
#include <zmq.hpp>
#include <nlohmann/json.hpp>
#include <chrono>

namespace Benternet{

using json = nlohmann::json;
using string = std::string;
template<typename T>
using vector = std::vector<T>;

extern json MainTopic_info;

struct Topic_template{
    int err{0};

    json info;
    string Processed_Data{""};
    void PrintJson() { std::cout << info.dump(4) << std::endl;}
    string GetCurrentTimestamp();
    string GetFromString(const std::string& key, const std::string& haystack);
    void SetServiceFields(std::string input);

    inline string GetTopic()    { return info["benternet"]["service"]["topic"].get<string>(); }
    inline string GetSession()  { return info["benternet"]["service"]["session"].get<string>(); };
    inline string GetMessage()  { return info["benternet"]["service"]["message"].get<string>(); };
    inline string GetDelim()    { return info["benternet"]["service"]["delim"].get<string>(); };
    inline string GetServiceStatus() { return info["benternet"]["service"]["status"].get<string>(); }
    inline string GetLastHeartbeat() { return info["benternet"]["service"]["last_heartbeat"].get<string>(); }
    inline int GetMaxConnections() { return info["benternet"]["configuration"]["max_connections"].get<int>(); }
    inline string GetHelp() { return info["benternet"]["commands"]["help"].get<string>(); }

    inline void SetTopic(const string& topic) { info["benternet"]["service"]["topic"] = topic; }
    inline void SetSession(const string& session) { info["benternet"]["service"]["session"] = session; }
    inline void SetMessage(const string& message) { info["benternet"]["service"]["message"] = message; }
    inline void SetDelim(const string& delim) { info["benternet"]["service"]["delim"] = delim;}
    inline void SetServiceStatus(const string& status) { info["benternet"]["service"]["status"] = status; }
    inline void SetLastHeartbeat() { info["benternet"]["service"]["last_heartbeat"] = GetCurrentTimestamp(); }
    
    inline string recvtopic()   { return (GetTopic()+">"+GetSession()+"?>");};
    inline string sendtopic()   { return (GetTopic()+">"+GetSession()+"!>");};
    

    Topic_template(json& info_){info = info_;}
};

class BTopics{
public:
    void run();

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

    string pick_option(const string& optionstring, json& info_);

    MainTopic() : Topic_template(MainTopic_info){} 
};

};