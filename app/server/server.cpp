#include <string>
#include <chrono>
#include <thread>
#include <iostream>

#include "Topics.h"

int main() {
    srand(time(0));
    openai::start("lm-studio","",true,"http://127.0.0.1:1234/v1/");
    Benternet::BTopics::instance().run();
    return 0;
}
