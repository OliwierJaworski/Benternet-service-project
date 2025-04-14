#include <chat_manager.h>

using namespace Benternet;

EventItems::EventItems(int index, short eventtype, _Pollevent_cb cb_) : index_{index}, cb_{cb_}, eventtype{eventtype}{

}

EventItems::~EventItems(){

}

void 
CategoryEvents::PollEvents(){

}

void 
CategoryEvents::PollAddEvent(_Pollevent_cb cb_, short eventtype){
    Poll_Items_.push_back(EventItems{(Poll_Items_.size()-1), eventtype, cb_});
}

void 
CategoryEvents::PollRemoveEvent(){
    
}

/*
void test(std::vector<teststruct> test){
    std::vector<zmq::pollitem_t> items_;
    items_.push_back(static_cast<zmq::pollitem_t&>(test.at(0)));
}*/