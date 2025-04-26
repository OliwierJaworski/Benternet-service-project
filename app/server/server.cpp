#include <string>
#include <chrono>
#include <thread>
#include <iostream>

#include "Topics.h"

int main() {
    srand(time(0));
    Benternet::BTopics::instance().run();
    return 0;
}
