#include <string>
#include <chrono>
#include <thread>
#include <iostream>

#include <zmq.hpp>
#include <openai.hpp>
#include <dnd_session.h>

using namespace nlohmann;

int main() {
    auto &session = dnd_session::start(); //start of manager instance
    session.socket("start?").connect("tcp://benternet.pxl-ea-ict.be:24042");
    session.socket("start?").OnEvent(ZMQ_POLLIN, printcontext); //op welke event? en wat moet er gebeuren?
    //session.socket("start?").recv(zmq::recv_flags::none); recv wordt door onevent gedaan
    std::string received_msg = session.socket("start?").ReadBuffer();
    std::cout << "----socket received: ---" << received_msg << endl; 

   
    //session.socket("start?").addevent(incoming/outgoing);
    //polling happens in background -> controlled by manager
}

void* printcontext(){
    printf("received message --------------------!\n");
}