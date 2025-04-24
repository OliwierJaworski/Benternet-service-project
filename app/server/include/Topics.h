#include <openai.hpp>
#include <BManager.h>
#include <zmq.hpp>
#include <nlohmann/json.hpp>

namespace Benternet{

using json = nlohmann::json;
using string = std::string;

class BTopics{
public:
    inline static const json topic_template{{"topic", "dnd_session"}, {"session", "start?"}, {"message",""}, {"delim", ">"}};

    void run();

    BTopics(){}
    ~BTopics(){}
private:
    void CreateMainThread();
    EFactory Ebuilder { *BManager::context() };
    PFactory Pbuilder { *BManager::context() };
    
};

struct MainTopic{
    json info                   { BTopics::topic_template };
    inline string GetTopic()    { return info["topic"].get<string>(); }
    inline string GetSession()  { return info["session"].get<string>(); };
    inline string GetMessage()  { return info["message"].get<string>(); };
    inline string GetDelim()    { return info["delim"].get<string>(); };

    inline string recvtopic()   { return (GetTopic()+"?>"+GetSession()+">");};
    inline string sendtopic()   { return (GetTopic()+"!>"+GetSession()+">");};
    
    static void filter_cb( Bbuffer& forwarded_data );
    static void UnpackMethod( std::function<void (zmq::message_t &, std::any &)> deser );
    static void PackMethod( std::function<void (zmq::message_t &, std::any &)> deser );
};

};
