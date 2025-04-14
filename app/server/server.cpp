#include <string>
#include <chrono>
#include <thread>
#include <iostream>

#include <zmq.hpp>
#include <openai.hpp>
#include <chat_manager.h>

using namespace Benternet;

string recvdanswer(string&& message, void* data, bool* cb_socket){
    cout << "received" << message << endl;
    string tmp ="this should be inside the send socket";
    *cb_socket = true;
    return tmp;
}

string Sendanswer(string&& message, void* data, bool* cb_socket){
    cout << "send triggered :received" << message << endl;
    string tmp ="dnd_session>start!>d>hello!";
    return tmp;
}

int main() {
    chat_manager::instance().Socket("start!",zmq::socket_type::push); //create 2nd socket instance
    chat_manager::instance().Socket("start!",zmq::socket_type::push); //see if socket is found
    chat_manager::instance().Socket("start!").Connect("tcp://benternet.pxl-ea-ict.be:24041"); //connect to server
    chat_manager::instance().Socket("start!").AddEvent(ZMQ_POLLOUT, Sendanswer);

    chat_manager::instance().Socket("start?",zmq::socket_type::sub); //create socket instance
    chat_manager::instance().Socket("start?",zmq::socket_type::sub); //see if socket is found
    chat_manager::instance().Socket("start?").Connect("tcp://benternet.pxl-ea-ict.be:24042"); //connect to server
    chat_manager::instance().Socket("start?").AddEvent(ZMQ_POLLIN, recvdanswer, chat_manager::instance().Socket("start!"));



    chat_manager::instance().Socket("start*",zmq::socket_type::sub); //create 2nd socket instance
    chat_manager::instance().Socket("start*",zmq::socket_type::sub); //see if socket is found
    chat_manager::instance().Socket("start*").Connect("tcp://localhost:5555"); //connect to server

    while(1){
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        chat_manager::instance().PollEvents();
        chat_manager::instance().executeEvents();
    }
}

