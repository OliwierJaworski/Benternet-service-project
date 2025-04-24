#include <openai.hpp>
#include <BManager.h>
#include <zmq.hpp>
#include <nlohmann/json.hpp>

namespace Benternet{

using json = nlohmann::json;
using string = std::string;
template<typename T>
using vector = std::vector<T>;

class BTopics{
public:
    inline static const json topic_template{{"topic", "dnd_session"}, {"session", "start"}, {"message",""}, {"delim", ">"}};

    void run();

    BTopics(){}
    ~BTopics(){}
private:
    void CreateMainThread();
    EFactory Ebuilder { *BManager::context() };
    PFactory Pbuilder { *BManager::context() };
    vector<Pipeline_W> pipelines;
};

struct MainTopic{
    json info;
    inline string GetTopic()    { return info["topic"].get<string>(); }
    inline string GetSession()  { return info["session"].get<string>(); };
    inline string GetMessage()  { return info["message"].get<string>(); };
    inline string GetDelim()    { return info["delim"].get<string>(); };

    inline string recvtopic()   { return (GetTopic()+">"+GetSession()+"?>");};
    inline string sendtopic()   { return (GetTopic()+">"+GetSession()+"!>");};
    
    static void filter_cb( Bbuffer& forwarded_data );
    static void UnpackMethod(zmq::message_t &, std::any &);
    static void PackMethod(zmq::message_t &, std::any &);

    MainTopic(){info = BTopics::topic_template;}
};

};
