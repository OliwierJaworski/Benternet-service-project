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

typedef void*(*_Pollevent_cb)();

struct Socket_t{
    zmq::recv_result_t recv() {}
    zmq::recv_result_t send() {} 
    json& GetTopic(){return topic_;}
    void OnEvent(short eventtype, _Pollevent_cb cb_);
    void OnConnect(string endpoint) { socket_.connect(endpoint); }

    Socket_t(json topic, zmq::socket_type type, zmq::context_t& context): topic_{topic}, 
                                                                          socket_{context, type}, 
                                                                          socktype_{type}{}
    ~Socket_t(){}
private:
    zmq::socket_t socket_;
    zmq::socket_type socktype_;
    json topic_;
};

struct CategoryTopic{
    static string OnGet_Topic(json& object)   { return object["topic"].get<string>(); }
    static string OnGet_Session(json& object) { return object["session"].get<string>(); }
    static string OnGet_Message(json& object) { return object["message"].get<string>(); }
    static string OnGet_Delim(json& object)   { return object["delim"].get<string>(); }
    static json OnGet_Template()      { return topic_template; }

    CategoryTopic(chat_manager& session) : session_{session}  {}
    ~CategoryTopic(){}
private:
    chat_manager& session_;
    inline static const json topic_template{{"topic", "dnd_session"}, {"session", "start?"}, {"message",""}, {"delim", ">"}};
};

struct EventItems : zmq::pollitem_t {

    EventItems(int index, short eventtype, _Pollevent_cb cb_); // : index_{index}, cb_{cb_}; <- cpp
    ~EventItems();
private:
   int index_;
   _Pollevent_cb cb_;
   short eventtype;
};

struct CategoryEvents{
    void OnPollEvents();
    void OnPollAddEvent(_Pollevent_cb cb_, short eventtype);
    void OnPollRemoveEvent();

    CategoryEvents(chat_manager& session) : session_{session} {}
    ~CategoryEvents(){}
private:
    chat_manager& session_;
    std::vector<zmq::pollitem_t > ZMQ_Items_;
    std::vector<EventItems>Poll_Items_;
};

struct CategorySocket{
    void Onlist(string target=""); //list all sockets and their state active /offline etc.. if target is empty all otherways targeted socket
    shared_ptr<Socket_t> Oncreate(json topic, zmq::socket_type type);

    CategorySocket(chat_manager& session) : session_{session} {}
    ~CategorySocket() {}
private:
    zmq::context_t context{1};
    chat_manager& session_;
    vector<shared_ptr< Socket_t >> sockets_V;
};

class chat_manager{ 
private:
    CategorySocket      sockets {*this}; 
    CategoryTopic       topics  {*this};
    CategoryEvents      events  {*this};

    chat_manager(const chat_manager&) = delete; //copy constructor deleted for singleton pattern
    chat_manager& operator= (const chat_manager&) = delete; //assignment operator deleted for singleton pattern

    chat_manager();
    ~chat_manager();

public:
    static chat_manager& instance();
    Socket_t& Socket( string SockedID, string topic = "dnd_session", zmq::socket_type type = static_cast<zmq::socket_type>(-1));
    const void list_sockets() {sockets.Onlist();}


};

}