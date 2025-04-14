#include <chat_manager.h>

using namespace Benternet;

shared_ptr<Socket_t> 
CategorySocket::Oncreate(json topic, zmq::socket_type type){
    for(auto &&currentSocket : sockets_V){
        if( CategoryTopic::OnGet_Session( currentSocket->GetTopic() ) == topic["session"].get<string>() ){
            return currentSocket;
        }
    }

    if(static_cast<int>(type) == -1){
        //no socket found and no type provided user error;
        cerr<< "Could not find socket called: " << topic["session"].get<string>() << " and no type provided, exiting."<< endl;
        exit(1);
        //later misschien default socket behavior dat de user kan instellen bij aanmaken van instance om dit te vermijden.
    }

    auto tmpV_socket_t = make_shared<Socket_t>(topic, type, context, *this);
    sockets_V.push_back(tmpV_socket_t);
    return tmpV_socket_t;
}

void 
Socket_t::AddEvent(short&& eventtype, _Pollevent_cb cb_){
    session_.OnAddEvent(eventtype,cb_);
}

void 
CategorySocket::OnAddEvent(short& eventtype, _Pollevent_cb cb_){

}

