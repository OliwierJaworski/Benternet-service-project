#include <string>
#include <chrono>
#include <thread>
#include <iostream>

#include <zmq.hpp>
#include <openai.hpp>

using namespace nlohmann;

int main() 
{
    using namespace std::chrono_literals;
    std::string topic_delimiter = ">";
    std::string topic_base      = "dnd_playthrough";
    std::string topic_recvStart= "start?";
    std::string topic_sendToken = "start!";
    std::vector<std::string> Vtopic;
    
    //listening to input
    zmq::socket_t subsocket{context, zmq::socket_type::sub};
    subsocket.connect("tcp://benternet.pxl-ea-ict.be:24042");
    subsocket.setsockopt(ZMQ_SUBSCRIBE, topic_base.c_str(),topic_base.length());

    //socket for sending answers
    zmq::socket_t pushsocket{context, zmq::socket_type::push};
    subsocket.connect("tcp://benternet.pxl-ea-ict.be:24042");

    std::string chatresponse="";

    // prepare some static data for responses
    const std::string data{"World"};
    //socket.recv(request, zmq::recv_flags::none);
    //socket.send(zmq::buffer(chatresponse) , zmq::send_flags::none);
    zmq::message_t request;

    for (;;) 
    {

    }

    return 0;
}
