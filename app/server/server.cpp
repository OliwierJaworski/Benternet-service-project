#include <string>
#include <chrono>
#include <thread>
#include <iostream>

#include "Topics.h"

int main() {
   
    Benternet::BTopics zmq_services;
    zmq_services.run();
    return 0;
}
