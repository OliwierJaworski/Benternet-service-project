#include <string>
#include <chrono>
#include <thread>
#include <iostream>

#include <zmq.hpp>
#include <openai.hpp>
#include <dnd_session.h>

using namespace nlohmann;

void* printcontext();

int main() {
    auto &session = dnd_session::start(); //start of manager instance
    session.socket("start?").connect("tcp://benternet.pxl-ea-ict.be:24042");
    session.socket("start?").OnEvent(ZMQ_POLLIN, printcontext); //op welke event? en wat moet er gebeuren?
    session.socket("start!").connect("tcp://benternet.pxl-ea-ict.be:24042");
    session.socket("start!").OnEvent(ZMQ_POLLIN, printcontext); //op welke event? en wat moet er gebeuren?
    session.polling();
}

void* printcontext(){
    printf("received message --------------------!\n");
    return nullptr;
}