#include <string>
#include <iostream>
#include <random>
#include <zmq.hpp>

int main() {
    zmq::context_t context(1);

    // SUB socket listens for weather requests
    zmq::socket_t sub_socket(context, zmq::socket_type::sub);
    sub_socket.connect("tcp://benternet.pxl-ea-ict.be:24042");

    // PUSH socket sends answers
    zmq::socket_t push_socket(context, zmq::socket_type::push);
    push_socket.connect("tcp://benternet.pxl-ea-ict.be:24041");

    // Subscribe to all "theweatheris?>" requests
    std::string sub_topic = "theweatheris?>";
    sub_socket.setsockopt(ZMQ_SUBSCRIBE, sub_topic.c_str(), sub_topic.length());

    // Random weather generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<std::string> weather_conditions = {
        "sunny", "cloudy", "rainy", "stormy", "snowy", "foggy", "windy"
    };
    std::uniform_int_distribution<> distrib(0, weather_conditions.size() - 1);

    std::cout << "Weather service active on Benternet..." << std::endl;

    while (true) {
        zmq::message_t request;
        sub_socket.recv(request, zmq::recv_flags::none);

        std::string received_msg = request.to_string();
        std::cout << "Received request: " << received_msg << std::endl;

        // Extract username from message
        std::string user = received_msg.substr(sub_topic.length());

        // Pick a random weather condition
        std::string random_weather = weather_conditions[distrib(gen)];

        // Construct the response
        std::string response = "theweatheris!>" + user + ">" + random_weather;

        // Send the response via PUSH socket
        zmq::message_t msg(response.size());
        memcpy(msg.data(), response.c_str(), response.size());

        push_socket.send(msg, zmq::send_flags::none);
        std::cout << "Sent response: " << response << std::endl;
    }

    return 0;
}
