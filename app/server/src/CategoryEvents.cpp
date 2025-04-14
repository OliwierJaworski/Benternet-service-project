#include <chat_manager.h>

using namespace Benternet;

EventItems::EventItems(int index, short eventtype, _Pollevent_cb cb_, CategorySocket& callback_socket ) : index_{index}, cb_{cb_}, eventtype{eventtype}, callback_socket{callback_socket}{

}

EventItems::~EventItems(){

}

void 
CategoryEvents::OnPollEvents(){

}

void 
CategoryEvents::OnPollAddEvent(void* socket, _Pollevent_cb cb_, short eventtype, CategorySocket* callback_socket){
    Poll_Items_.push_back(EventItems{(Poll_Items_.size()-1), eventtype, cb_});
    ZMQ_Items_.push_back(zmq::pollitem_t{socket, 0, eventtype, 0});
}

void 
CategoryEvents::OnPollRemoveEvent(){
    
}

/*
void test(std::vector<teststruct> test){
    std::vector<zmq::pollitem_t> items_;
    items_.push_back(static_cast<zmq::pollitem_t&>(test.at(0)));
}*/