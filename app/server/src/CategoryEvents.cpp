#include <chat_manager.h>

using namespace Benternet;

EventItems::EventItems(int index, short eventtype, _Pollevent_cb cb_, Socket_t* callback_socket, Socket_t* current_socket) : index_{index}, cb_{cb_}, eventtype{eventtype}, callback_socket{callback_socket}, current_socket{current_socket}{

}

EventItems::~EventItems(){

}

void 
CategoryEvents::OnPollEvents(){
    cout << "polling from event poolsize:" << ZMQ_Items_.size() << endl;
    for(auto& item : Poll_Items_){
        cout << "topic: " << item.current_socket->GetTopic().dump() << endl; 
    }
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
    
    try{
    int index =0;
    string tmp;
    cout << "polling items: "<< ZMQ_Items_.size() << endl;
    for(auto& item : ZMQ_Items_){
        if(item.revents & item.events){
            switch(item.events){
                case POLL_IN:
                    Poll_Items_.at(index).current_socket->recv(zmq::recv_flags::none);
                    tmp = Poll_Items_.at(index).cb_( Poll_Items_.at(index).current_socket->ReadBuffer(),nullptr);
                    Poll_Items_.at(index).callback_socket->Set_Buffer(tmp);
                    break;

                case POLL_OUT:
                    /*
                        if(Poll_Items_.at(index).callback_socket != nullptr){ //user wants to send data back after reading
                            if(Poll_Items_.at(index).callback_socket->CanSend){ //the recv processed all data and wants to send answer
                                Poll_Items_.at(index).callback_socket->send( Poll_Items_.at(index).cb_( Poll_Items_.at(index).callback_socket->ReadBuffer(),nullptr) ,zmq::recv_flags::none);
                                Poll_Items_.at(index).callback_socket->CanSend = false;
                            }
                        }
                    
                    */
                    break;
                default:
                    std::exception_ptr p = std::current_exception();
                    std::clog <<(p ? p.__cxa_exception_type()->name() : "null") << std::endl;
                    exit(1);
                    break;
            }
        }
        item.revents=0;
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
CategoryEvents::OnPollAddEvent(zmq::pollitem_t&& zmq_item, EventItems& event_item){
    ZMQ_Items_.push_back(zmq_item);
    Poll_Items_.push_back(event_item);
}

void 
CategoryEvents::OnPollRemoveEvent(){
    
}



//recv komt binnnen-> callback function -> callback socket doet send ->> send_enable! + data