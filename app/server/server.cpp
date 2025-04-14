#include <string>
#include <chrono>
#include <thread>
#include <iostream>

#include <zmq.hpp>
#include <openai.hpp>
#include <chat_manager.h>


using namespace Benternet;

int main() {
    chat_manager::instance();
}

