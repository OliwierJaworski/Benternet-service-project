#include <string>
#include <chrono>
#include <thread>
#include <iostream>

#include <zmq.hpp>
#include <openai.hpp>
#include <BManager.h>
#include <helper.h>

int main() {
    std::cout << "hello world!" << std::endl;
    std::shared_ptr<Pipeline_T> pipelineA;
    Sub_Socket sockin; //-->Sub_Socket::Element_T
    Push_Socket sockout;
    sockout.opt(ENDPOINT,"ssdewqe.com");
    sockout.opt(SOCKOPT,"die speciale socket opties");
    ElementLink(Sub_Socket, MessageFilter, Push_Socket);
    pipelineA.add(Sub_Socket, MessageFilter, Push_Socket);
    pipelineA.setopt(ENDPOINT,"ssdewqe.com");
    //
    //? endpoint voor pipeline?
    //? wat houdt de pipeline in?
    //? welke dataaccess?
    BManager::instance();
    
    return 0;
}
