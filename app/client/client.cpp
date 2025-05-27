#include <string>
#include <iostream>
#include <zmq.hpp>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Gebruik: client.exe <topic> <gebruikersnaam>\n";
        return 1;
    }

    std::string topic = argv[1];
    std::string naam = argv[2];

    zmq::context_t context{1};

    zmq::socket_t push_socket{context, zmq::socket_type::push};
    push_socket.connect("tcp://benternet.pxl-ea-ict.be:24041");

    zmq::socket_t sub_socket{context, zmq::socket_type::sub};
    sub_socket.connect("tcp://benternet.pxl-ea-ict.be:24042");

    std::string subscribe_topic = "dnd_session>" + topic + "!>";
    sub_socket.setsockopt(ZMQ_SUBSCRIBE, subscribe_topic.c_str(), subscribe_topic.length());

    while (true) {
        std::string message;
        std::cout << "Message: ";
        std::getline(std::cin, message); 

        std::string push_message = "dnd_session>" + topic + "?>" + naam + ">" + message;
        std::cout << "Verzonden aanvraag: " << push_message << std::endl;

        push_socket.send(zmq::buffer(push_message), zmq::send_flags::none);

        zmq::message_t reply;
        sub_socket.recv(reply, zmq::recv_flags::none);
        std::string antwoord = reply.to_string();

        std::cout << "Ontvangen antwoord: " << antwoord << std::endl;
    }

    return 0;
}
