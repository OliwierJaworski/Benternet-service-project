#pragma once

#include <iostream>
#include <cstdlib>
#include <zmq.hpp>
#include <nlohmann/json.hpp>
#include <mutex>
#include <thread>
#include <cstdlib>


#ifdef __linux__
#elif _WIN32 
#else
    std::cerr << "unsupported platform. no rich allowed, Exiting please" <<std::endl;
    exit(1); //not made for apple normies 
#endif

using namespace std;
using namespace nlohmann;

namespace Benternet{

class chat_manager;


struct Socket_t{
    zmq::recv_result_t recv() {}
    zmq::recv_result_t send() {} 
    json& GetTopic(){return topic_;}

    Socket_t(json topic, zmq::socket_type type): topic_{topic}, socktype_{type} {}
    ~Socket_t(){}
private:
    zmq::socket_t socket_;
    zmq::socket_type socktype_;
    json topic_;
};

struct CategoryTopic{
    static string Get_Topic(json& object)   { return object["topic"].get<string>(); }
    static string Get_Session(json& object) { return object["session"].get<string>(); }
    static string Get_Message(json& object) { return object["message"].get<string>(); }
    static string Get_Delim(json& object)   { return object["delim"].get<string>(); }
    static json Get_Template()      { return topic_template; }

    CategoryTopic(chat_manager& session) : session_{session}  {}
    ~CategoryTopic(){}
private:
    chat_manager& session_;
    inline static const json topic_template{{"topic", "dnd_session"}, {"session", "start?"}, {"message",""}, {"delim", ">"}};
};

struct CategoryEvents{
    void PollEvents();
    void PollAddEvent();
    void PollRemoveEvent();

    CategoryEvents(chat_manager& session) : session_{session} {}
    ~CategoryEvents(){}
private:
    chat_manager& session_;

};

struct CategorySocket{
    void list(); //list all sockets and their state active /offline etc..
    shared_ptr<Socket_t> create(json topic, zmq::socket_type type);

    CategorySocket(chat_manager& session) : session_{session} {}
    ~CategorySocket() {}
private:
    chat_manager& session_;
    vector<shared_ptr< Socket_t >> sockets_V;
};

struct CategoryContext{
    zmq::context_t& get_context(){return context;};

    CategoryContext(chat_manager& session) : session_{ session }{};
private:
    const chat_manager& session_;
    zmq::context_t context{1};
};


class chat_manager{ 

    CategorySocket      sockets {*this}; 
    CategoryTopic       topics  {*this};
    CategoryEvents      events  {*this};
    CategoryContext     context {*this};

    chat_manager(const chat_manager&) = delete; //copy constructor deleted for singleton pattern
    chat_manager& operator= (const chat_manager&) = delete; //assignment operator deleted for singleton pattern

    chat_manager();
    ~chat_manager();
public:

   static chat_manager& instance();
   Socket_t& Socket(string endpoint, zmq::socket_type type, string session, string topic = "dnd_session");
   const void list_sockets() {sockets.list();}
};

}