#include <string>
#include <iostream>
#include <zmq.hpp>

int main(int argc, char* argv[]) {
    // Controleer argumenten
    if (argc < 3) {
        std::cerr << "Gebruik: client.exe <topic> <gebruikersnaam>\n";
        return 1;
    }

    std::string topic = argv[1];
    std::string naam = argv[2];

    // Initialiseer ZMQ context
    zmq::context_t context{1};

    // PUSH socket om aanvragen te versturen
    zmq::socket_t push_socket{context, zmq::socket_type::push};
    push_socket.connect("tcp://benternet.pxl-ea-ict.be:24041");

    // SUB socket om antwoorden te ontvangen
    zmq::socket_t sub_socket{context, zmq::socket_type::sub};
    sub_socket.connect("tcp://benternet.pxl-ea-ict.be:24042");

    // Subscribe op antwoorden specifiek voor deze gebruiker en topic
    std::string sub_topic = topic + "!>" + naam + ">";
    sub_socket.setsockopt(ZMQ_SUBSCRIBE, sub_topic.c_str(), sub_topic.length());

    // Verzend aanvraag via PUSH
    std::string push_message = topic + "?>" + naam + ">";
    std::cout << "Verzonden aanvraag: " << push_message << std::endl;
    push_socket.send(zmq::buffer(push_message), zmq::send_flags::none);

    // Luister naar antwoorden
    zmq::message_t reply;
    sub_socket.recv(reply, zmq::recv_flags::none);
    std::string antwoord = reply.to_string();

    std::cout << "Ontvangen antwoord: " << antwoord << std::endl;

    // Houd venster open zodat antwoord leesbaar blijft
    std::cout << "\nDruk ENTER om te sluiten...";
    std::cin.get();

    return 0;
}
