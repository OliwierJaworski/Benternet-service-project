#include <string>
#include <chrono>
#include <thread>
#include <iostream>

#include <zmq.hpp>
#include <openai.hpp>
#include <dnd_session.h>

using namespace nlohmann;

void* printcontextz();
void* printcontexts();
int main() {
    auto &session = dnd_session::start(); //start of manager instance
    session.socket("start?").connect("tcp://benternet.pxl-ea-ict.be:24042");
    session.socket("start?").OnEvent(ZMQ_POLLIN, printcontextz); //op welke event? en wat moet er gebeuren?
    session.socket("start!").connect("tcp://benternet.pxl-ea-ict.be:24042");
    session.socket("start!").OnEvent(ZMQ_POLLIN, printcontexts); //op welke event? en wat moet er gebeuren?
    while(1){
        session.polling();
    }   
}

void* printcontextz(){
    printf("-- start? --> fired\n");
    return nullptr;
}
void* printcontexts(){
    printf("-- start! --> fired\n");
    return nullptr;
}