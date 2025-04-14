#include <chat_manager.h>

using namespace Benternet;

EventItems::EventItems(int index, short eventtype, _Pollevent_cb cb_, Socket_t* callback_socket, Socket_t* current_socket) : index_{index}, cb_{cb_}, eventtype{eventtype}, callback_socket{callback_socket}, current_socket{current_socket}{

}

EventItems::~EventItems(){

}

void 
CategoryEvents::LogEvents(){
    try{
        if(zmq::poll(ZMQ_Items_, std::chrono::milliseconds(-1)) == -1){
            std::exception_ptr p = std::current_exception();
            std::clog <<(p ? p.__cxa_exception_type()->name() : "null") << std::endl;
            exit(1);
        }
    }catch(...)
    {
        std::exception_ptr p = std::current_exception();
        std::clog <<(p ? p.__cxa_exception_type()->name() : "null") << std::endl;
        exit(1);
    }
}

void
CategoryEvents::LoopEvents(){
    int index =0;

    try{
    cout << "polling items: "<< ZMQ_Items_.size() << endl;
    for(auto& item : ZMQ_Items_){
        if(item.revents & item.events){
            ExecuteEvent(Poll_Items_.at(index),item.events);
        }
        index++;
    }
    }catch(...)
    {
        std::exception_ptr p = std::current_exception();
        std::clog <<(p ? p.__cxa_exception_type()->name() : "null") << std::endl;
        exit(1);
    }
}
void
CategoryEvents::ExecuteEvent(EventItems& item, short eventtype){
   
    switch(eventtype){

        case POLL_OUT:
            if(item.current_socket->cansend){
                item.current_socket->send( item.cb_(item.current_socket->ReadBuffer(), item.current_socket->GetDataForCB(), nullptr), zmq::send_flags::none);
            }
            item.current_socket->cansend = false;
            return;
        break;

        case POLL_IN:
            item.current_socket->recv(zmq::recv_flags::none);
            if(item.callback_socket){
                string tmp = item.cb_( item.current_socket->ReadBuffer(), item.current_socket->GetDataForCB(), &item.callback_socket->cansend  );
                item.callback_socket->Set_Buffer(tmp);
            }else{
                string tmp = item.cb_( item.current_socket->ReadBuffer(), item.current_socket->GetDataForCB(), nullptr );
            }
            return;
        break;
    }

    std::exception_ptr p = std::current_exception();
    std::clog <<(p ? p.__cxa_exception_type()->name() : "null") << std::endl;
    exit(1);
}

void 
CategoryEvents::OnPollAddEvent(zmq::pollitem_t&& zmq_item, EventItems& event_item){
    ZMQ_Items_.push_back(zmq_item);
    Poll_Items_.push_back(event_item);
}

void 
CategoryEvents::OnPollRemoveEvent(){
    
}



//recv komt binnnen-> callback function -> callback socket doet send ->> send_enable! + data