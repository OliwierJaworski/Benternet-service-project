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
    static bool isJson(const std::string &data);

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
    //CategorySocket::kill()

    CategorySocket(dnd_session& session, json topic, zmq::socket_type type);
    ~CategorySocket();
private:
    unique_ptr<zmq::socket_t>const socket { nullptr };
    dnd_session& session_;
    const CategoryTopic* topic_;
    zmq::message_t socket_buffer;
    //maybe socket type also as info? zmq::socket_type type;
}; 

struct CategoryMessageSystem{
   void PollEvents();
   void PollingAddEvent(void* socket, short events =ZMQ_POLLIN );
   void PollingRemoveEvent(void* socket);

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


class dnd_session //miss ooit renamen naar gamemaster /manager
{ 
public:
    const json topic_template = {
        {"topic", "dnd_playthrough"},
        {"session", "start?"},
        {"message", ""},
        {"delim", ">"}
    };   
    

    static dnd_session& start();
    dnd_session& instance() { return start(); }
    CategorySocket& socket(std::string SocketID);


    dnd_session(const dnd_session&) = delete; //remove copy constructor
    dnd_session& operator= (const dnd_session&) = delete; //remove assignment operator 
private:
    dnd_session();
    ~dnd_session();


    std::mutex            topics_lock; 
    vector<CategoryTopic> topics;
    CategoryContext       context { *this };
    CategoryMessageSystem MessageSystem{ *this };
    vector<std::unique_ptr<CategorySocket>> sockets; 

    friend struct CategoryMessageSystem;
    friend struct CategoryContext;
    friend struct CategorySocket;
};
using socket_type = zmq::socket_type;
