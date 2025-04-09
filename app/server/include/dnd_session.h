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
    inline void attach_socket(CategorySocket* socket) {this->session_ = socket;}; 
    const string to_string() const ;

    CategoryTopic(json topic ) : topic_{topic}{};
private:
    CategorySocket* session_;
    json topic_;
}; 

struct CategorySocket{
    inline void connect(string endpoint){socket->connect(endpoint);};

    CategorySocket(dnd_session& session, json topic, zmq::socket_type type);
    const unique_ptr<zmq::socket_t> socket {nullptr};
private:
    
    const dnd_session& session_;
    const CategoryTopic* topic_;
    zmq::message_t message;
}; 

struct CategoryContext{
    inline zmq::context_t& get_context(){return context;};

    CategoryContext(dnd_session& session) : session_{session}{};
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
    
    std::mutex topics_lock; 
    vector<CategoryTopic> topics;
    vector<std::unique_ptr<CategorySocket>> sockets; 
    CategoryContext context {*this};

    dnd_session();
    ~dnd_session();
private:
    
};
using socket_type = zmq::socket_type;
