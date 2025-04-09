#pragma once

#include <cstdlib>
#include <zmq.hpp>
#include <nlohmann/json.hpp>
#ifdef __linux__
#elif _WIN32 
#else
    std::cerr << "unsupported platform. no rich allowed, Exiting please" <<std::endl;
    exit(1); //not made for apple normies 
#endif

using namespace nlohmann; 
class dnd_session;



struct CategoryTopic{
    int start(); 
    int exit();
    CategoryTopic(dnd_session& session, json topic ) : session_{session}, topic_{topic}{};
private:
    dnd_session& session_;
    json topic_;
}; 




class dnd_session
{
private:
    zmq::context_t context{1};
public:
    const json topic_template{{"topic", "dnd_playthrough"},{"session","start?"},{"message",""},{"delimiter",">"}};
    dnd_session();
    ~dnd_session();

    std::vector<CategoryTopic> topics;
};

int CategoryTopic::start(){

} 

int CategoryTopic::exit(){

}