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
            break;

        case Element_type::push:
           
            element = std::make_unique<Push_Element>(context);
            element->socket = std::make_unique<zmq::socket_t>(context, ZMQ_PUSH);
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
    (void)socket->recv(*sink->GetBuffer(), zmq::recv_flags::none);
}

void
Push_Element::process(){
    std::cout << "process of push element\n";
    std::string buffer; 
    (void)socket->send(*source->GetBuffer(),zmq::send_flags::none);
}

void
Filter_Element::process(){
    std::cout << "process of Filter element\n";
    if(cb_ != nullptr){
        cb_();
    }
}