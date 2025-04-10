#include <string>
#include <chrono>
#include <thread>
#include <iostream>

#include <zmq.hpp>
#include <openai.hpp>
#include <dnd_session.h>

using namespace nlohmann;

int main() {
    dnd_session::start(); //start of manager instance
    //dnd_session::sockets(socketbysessionvalue).add();
    //dnd_session::sockets("topic").send();
}
