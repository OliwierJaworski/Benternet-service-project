#include <string>
#include <chrono>
#include <thread>
#include <iostream>

#include <zmq.hpp>
#include <openai.hpp>
#include <dnd_session.h>

using namespace nlohmann;

int main() 
{
    dnd_session session;
    session.sockets.at(0)->connect("tcp://benternet.pxl-ea-ict.be:24041");
    std::string message = "theweatheris?>ss>";
    zmq::message_t msg(message.c_str(), message.size());
    
    session.sockets.at(0)->socket->send(msg, zmq::send_flags::none);
    return 0;
}
