#include <string>
#include <chrono>
#include <thread>
#include <iostream>
#include <random>

#include <zmq.hpp>
#include <openai.hpp>
#include <Chat_API.hpp>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<uint32_t> dist(100000, 999999); // 6-digit ID

std::string recvdanswer(std::string&& message, void* data, bool* cb_socket) {
    std::string tmp = "";

    size_t first = message.find('>');
    size_t second = message.find('>', first + 1);
    size_t third = message.find('>', second + 1);

    if (third != std::string::npos) {
        message = message.substr(second + 1);
    }

    if (message.find("lets play!") != std::string::npos) {
        std::cout << "received " << message << std::endl;
        tmp = message;
        *cb_socket = true;
    }

    return tmp;
}

string playgamerecv(string&& message, void* data, bool* cb_socket){
    std::string tmp = "";

    size_t first = message.find('>');
    size_t second = message.find('>', first + 1);
    size_t third = message.find('>', second + 1);

    if (third != std::string::npos) {
        message = message.substr(second + 1);
    }

    if (message.find("lets play!") != std::string::npos) {
        std::cout << "received " << message << std::endl;
        tmp = message;
        *cb_socket = true;
    }

    return tmp;
}

string playgame(string&& message, void* data, bool* cb_socket){
    openai::start("lm-studio","",true,"http://127.0.0.1:1234/v1/");
    std::string chatresponse="";
    auto  chat = openai::chat().create(R"(
        {
            "model": "deepseek-r1-distill-qwen-7b",
            "messages":[{"role":"user", "content":"i want to play as a lilo the human"}],
            "max_tokens": 2000,
            "temperature": 0,
            "id": "chatcmpl-aye2e09u94leibtysrwob"
        }
        )"_json);
        
    chatresponse= chat["choices"][0]["message"]["content"].dump();
    return chatresponse;
}

string Sendanswer(string&& message, void* data, bool* cb_socket){
    static vector<uint32_t> ids;
    if(data != nullptr){
        //process data shared amongst other sockets//when threading is added this can be dangerous!
        cout << "data inside void* data: " << *(reinterpret_cast<int*>(data)) << endl;
    }

    string tmp ="dnd_session>start!>";
    size_t delimiter_pos = message.find('>');
    uint32_t random_id = dist(gen);

    do {
        random_id = dist(gen);
    } while (std::find(ids.begin(), ids.end(), random_id) != ids.end());

    ids.push_back(random_id);
    
    if (delimiter_pos != std::string::npos) {
        tmp += message.substr(0, delimiter_pos+1);
    }else{
        std::cerr  << "user forgot to mention the topic from the recv socket\n";
        exit(1);
    }
    cout << "send triggered :received from recv socket" << message << endl;

    string sessionIDsub = std::to_string(random_id) + "?";
    string sessionIDpush = std::to_string(random_id) + "!";
    tmp += "your session code is: " + sessionIDsub + ">";

    socket(sessionIDsub,zmq::socket_type::sub);
    socket(sessionIDpush,zmq::socket_type::push); 

    socket(sessionIDsub).Connect("tcp://benternet.pxl-ea-ict.be:24042");
    socket(sessionIDpush).Connect("tcp://benternet.pxl-ea-ict.be:24042");

    socket(sessionIDsub).AddEvent(POLL_IN, recvdanswer, socket(sessionIDpush)); 
    socket(sessionIDpush).AddEvent(POLL_OUT, playgame);

    return tmp;
}

int main() {

    //initialize socket
    socket("start?",zmq::socket_type::sub);
    socket("start!",zmq::socket_type::push); 

     //connect to endpoint
    socket("start?").Connect("tcp://benternet.pxl-ea-ict.be:24042");
    socket("start!").Connect("tcp://benternet.pxl-ea-ict.be:24041");

    //add recv event on which cb gets called and in the callback user chooses if the cb_socket
    //should answer or not by setting cb_socket to true/false
    socket("start?").AddEvent(POLL_IN, recvdanswer, socket("start!")); 

    //add send cb for the cb_socket
    socket("start!").AddEvent(POLL_OUT, Sendanswer);

    int data=1;
    //give the socket data it can process or return back to the application
    socket("start!").SetDataForCB(&data);

    //poll recv and send events to limit cpu usage 
    while(1){
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        poll();
        run();
    }
       return 0;
}

