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
    session.socket("start?",socket_type::sub);
    session.socket("start?",socket_type::sub);
    session.socket("idkrandom",socket_type::sub);
    cout << "after idkrandom socket creation"  <<endl;
    session.socket("random2",socket_type::sub);
    cout << "after random2 socket creation"  <<endl;
    session.socket("idkrandom",socket_type::sub);
    //dnd_session::sockets(socketbysessionvalue).add();
    //dnd_session::sockets("topic").send();

    //dnd_session::socket("blabla").send("i like turtles"); err can only be performed on static ...
}
