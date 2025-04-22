#include <ElementTypes.h>

std::unique_ptr<Element_T> 
EFactory::build(){
    std::unique_ptr<Element_T> tmp = std::move(element);
    reset();
    return std::move(tmp);
}

void EFactory::CreateElement(Element_type type) {
    switch(type) {
        case Element_type::sub:
    
            element = std::make_unique<Sub_Element>(context); 
            element->socket = std::make_unique<zmq::socket_t>(context, ZMQ_SUB);
            element->eventhandle= std::make_unique<PollItem_T>(element->socket->handle(),POLL_IN,*element);
            break;

        case Element_type::push:
           
            element = std::make_unique<Push_Element>(context);
            element->socket = std::make_unique<zmq::socket_t>(context, ZMQ_PUSH);
            element->eventhandle= std::make_unique<PollItem_T>(element->socket->handle(),POLL_OUT,*element);
            break;

        case Element_type::qeue:
            
            break;

        case Element_type::filter:
            element = std::make_unique<Filter_Element>(context);
            break;

        default:
            std::cerr << "Unknown element type" << std::endl;
            exit(1);
            break;
    }
}

void
Sub_Element::process(){
    std::cout << "process of sub element\n";
    
    if(sink != nullptr){
        if (socket->recv(sink->GetBuffer(), zmq::recv_flags::none) == -1)
        {
            std::cout << zmq_strerror(errno) << std::endl;
        }

    }else if(source != nullptr){
        if (socket->recv(source->GetBuffer(), zmq::recv_flags::none) == -1)
        {
            std::cout << zmq_strerror(errno) << std::endl;
        }

    }else{
        std::cout << "somehow no buffer could be found\n";
        exit(1);
    }
}

void
Push_Element::process(){
    std::cout << "process of push element\n";
    std::string buffer; 
    if (socket->send(sink->GetBuffer(),zmq::send_flags::none) == -1)
    {
        std::cout << zmq_strerror(errno) << std::endl;
    }
}

void
Filter_Element::process(){
    std::cout << "process of Filter element\n";
    if(cb_ != nullptr){
        if(sink != nullptr){
            std::cout << "sink is not nullptr so taking its buffer\n";
            cb_(sink->GetBuffer()); //in message = topic

        }else if(source != nullptr){
            std::cout << "source is not nullptr so taking its buffer\n";
            cb_(source->GetBuffer()); //in message = topic

        }else{
            std::cout << "somehow no buffer could be found\n";
            exit(1);
        }
       
    }
}
