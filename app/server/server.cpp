#include <string>
#include <chrono>
#include <thread>
#include <iostream>

#include <zmq.hpp>
#include <openai.hpp>
#include <chat_manager.h>

using namespace Benternet;

string recvdanswer(string&& message, void* data){
    cout << "received" << message << endl;
    string tmp ="";
    return tmp;
}

int main() {
    chat_manager::instance().Socket("start?",zmq::socket_type::sub); //create socket instance
    chat_manager::instance().Socket("start?",zmq::socket_type::sub); //see if socket is found
    chat_manager::instance().Socket("start?").Connect("tcp://benternet.pxl-ea-ict.be:24042"); //connect to server
    chat_manager::instance().Socket("start?").AddEvent(ZMQ_POLLIN, recvdanswer);

    chat_manager::instance().Socket("start!",zmq::socket_type::sub); //create 2nd socket instance
    chat_manager::instance().Socket("start!",zmq::socket_type::sub); //see if socket is found
    chat_manager::instance().Socket("start!").Connect("tcp://benternet.pxl-ea-ict.be:24042"); //connect to server
    chat_manager::instance().Socket("start!").AddEvent(ZMQ_POLLIN, recvdanswer);

    chat_manager::instance().Socket("start*",zmq::socket_type::sub); //create 2nd socket instance
    chat_manager::instance().Socket("start*",zmq::socket_type::sub); //see if socket is found
    chat_manager::instance().Socket("start*").Connect("tcp://localhost:5555"); //connect to server
    while(1){
        chat_manager::instance().PollEvents();
        chat_manager::instance().executeEvents();
    }
}

