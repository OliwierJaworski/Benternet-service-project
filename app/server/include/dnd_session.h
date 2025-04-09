#pragma once

#include <iostream>
#include <cstdlib>
#include <zmq.hpp>
#include <nlohmann/json.hpp>
#include <mutex>
#ifdef __linux__
#elif _WIN32 
#else
    std::cerr << "unsupported platform. no rich allowed, Exiting please" <<std::endl;
    exit(1); //not made for apple normies 
#endif

using namespace nlohmann; 
using namespace std;

class dnd_session;
struct CategorySocket;


struct CategoryTopic{
    void attach_socket(CategorySocket* socket) { this->session_ = socket; }; 
    const string to_string() const ;

    CategoryTopic(json topic ) : topic_{ topic }{};
private:
    CategorySocket* session_;
    json topic_;
}; 

struct CategorySocket{
    void connect(string endpoint){socket->connect(endpoint);};
    zmq::recv_result_t recv(zmq::recv_flags flags) { return socket->recv(socket_buffer,flags); }
    void send(string push_message, zmq::send_flags flags) { socket->send(zmq::buffer(push_message),flags); }
    zmq::message_t* GetBuffer() { return &socket_buffer; } 
    string ReadBuffer() { return socket_buffer.to_string(); }

    CategorySocket(dnd_session& session, json topic, zmq::socket_type type);
private:
    const unique_ptr<zmq::socket_t> socket { nullptr };
    const dnd_session& session_;
    const CategoryTopic* topic_;
    zmq::message_t socket_buffer;
    //maybe socket type also as info? zmq::socket_type type;
}; 

struct CategoryMessageSystem{
   void PollingInit();
   void PollEvents();
   void PollingAddEvent(void* socket, short events =ZMQ_POLLIN );
   void PollingRemoveEvent(){};//functionality will be added later maybe

   CategoryMessageSystem(dnd_session& session) : session_{ session }{} 
private:
    const dnd_session& session_;
    vector<zmq::pollitem_t> items;
};

struct CategoryContext{
    zmq::context_t& get_context(){return context;};

    CategoryContext(dnd_session& session) : session_{ session }{};
private:
    const dnd_session& session_;
    zmq::context_t context{1};
}; 


class dnd_session //miss ooit renamen naar gamemaster
{ 
public:
    const json topic_template = R"(
        {
            "topic"  : "dnd_playthrough",
            "session": "start?",
            "message": "",
            "delim"  : ">"
        })"_json;
    
    std::mutex            topics_lock; 
    vector<CategoryTopic> topics;
    CategoryContext       context { *this };
    CategoryMessageSystem MessageSystem{ *this };
    vector<std::unique_ptr<CategorySocket>> sockets; 

    dnd_session();
    ~dnd_session();
private:
    
};
using socket_type = zmq::socket_type;
