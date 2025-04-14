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
Socket_t::AddEvent(short&& eventtype, _Pollevent_cb cb_, Socket_t* callback_socket){
    if(cb_ == nullptr){
        cerr<< "No callback provided for event for socket " << GetTopic()["session"].get<string>() << ", exiting."<< endl;
        exit(1);
    }
    EventItems eventItem(session_.get_session().events.Getindex(), eventtype, cb_, callback_socket, this);
    session_.get_session().events.OnPollAddEvent(zmq::pollitem_t{.socket = socket_.handle(), .fd = 0, .events= eventtype, .revents=0}, eventItem);
}


