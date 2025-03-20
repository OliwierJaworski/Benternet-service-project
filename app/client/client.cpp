#include <string>
#include <iostream>

#include <zmq.hpp>

int main()
{
    // initialize the zmq context with a single IO thread
    zmq::context_t context{1};

    // construct a REQ (request) socket and connect to interface
    zmq::socket_t socket{context, zmq::socket_type::req};
    socket.connect("tcp://localhost:5555");

    // set up some static data to send
    std::string query="";

    while(1)
    {
        std::getline(std::cin, query);
        // send the request message
        std::cout << "sending message: \""<<query <<  "\""<< std::endl;
        socket.send(zmq::buffer(query), zmq::send_flags::none);
        
        // wait for reply from server
        zmq::message_t reply{};
        socket.recv(reply, zmq::recv_flags::none);

        std::cout << "Received " << reply.to_string(); 
        std::cout << std::endl;
    }

    return 0;
}
