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
struct CategorySocket;

typedef string (*_Pollevent_cb)(string&& message, bool* cbsocket_cansend); //template type can be used her

struct Socket_t{
    //zmq poll doet send en recv anders kan die vastzitten
    zmq::recv_result_t recv(zmq::recv_flags flags) {return socket_.recv(socket_buffer,flags); }
    zmq::recv_result_t send(string push_message, zmq::send_flags flags) { return socket_.send(zmq::buffer(push_message),flags);} 
    json& GetTopic(){return topic_[0];} 
    string ReadBuffer(){ return socket_buffer.to_string(); } //onefficient 
    void Set_Buffer(string& message){ socket_buffer = zmq::message_t{message.c_str(),message.size()} ;}
    void AddEvent(short&& eventtype, _Pollevent_cb cb_, Socket_t* callback_socket = nullptr); //nullptr als er enkel dataverwerking moet gebeuren
    void Connect(string&& endpoint) { socket_.connect(endpoint); }

    bool CanSend;
    Socket_t(json topic, zmq::socket_type type, zmq::context_t& context,CategorySocket& session): topic_{topic},
                                                                          session_{session},
                                                                          socket_{context, type}, //--> keep in mind
                                                                          socktype_{type}{CanSend =false;} //idk how to fix this :(
    ~Socket_t(){}
private:
    
    zmq::socket_t socket_;
    zmq::socket_type socktype_;
    zmq::message_t socket_buffer;
    json topic_;
    CategorySocket& session_;
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

struct EventItems {

   int index_;
   _Pollevent_cb cb_;
   short eventtype;
   Socket_t* callback_socket; //where does data from callback go
   Socket_t* current_socket;

    EventItems(int index, short eventtype, _Pollevent_cb cb_, Socket_t* callback_socket, Socket_t* current_socket); // : index_{index}, cb_{cb_}; <- cpp
    ~EventItems();
private:
   
};

struct CategoryEvents{
    void OnPollEvents();
    void OnPollAddEvent(zmq::pollitem_t&& zmq_item, EventItems& event_item);
    void OnPollRemoveEvent();
    size_t Getindex(){ return ZMQ_Items_.size() -1; }

    
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
    void OnAddEvent(short& venttype, _Pollevent_cb cb_, CategorySocket* callback_socket);
    chat_manager& get_session(){ return session_;}

    CategorySocket(chat_manager& session) : session_{session} {}
    ~CategorySocket() {}
private:
    zmq::context_t context{1};
    chat_manager& session_;
    vector<shared_ptr< Socket_t >> sockets_V;
};

class chat_manager{ 
private:
    friend Socket_t;

    CategorySocket      sockets {*this}; 
    CategoryTopic       topics  {*this};
    CategoryEvents      events  {*this};

    chat_manager(const chat_manager&) = delete; //copy constructor deleted for singleton pattern
    chat_manager& operator= (const chat_manager&) = delete; //assignment operator deleted for singleton pattern

    chat_manager();
    ~chat_manager();

public:
    static chat_manager& instance();
    Socket_t& Socket( string SockedID, zmq::socket_type type = static_cast<zmq::socket_type>(-1), string topic = "dnd_session");
    const void list_sockets() {sockets.Onlist();}
    void PollEvents(){events.OnPollEvents();};
};

}