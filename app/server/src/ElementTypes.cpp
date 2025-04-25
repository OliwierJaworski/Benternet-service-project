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
            HandleInvalid("Unknown element type");
            break;
    }
}

void
Sub_Element::process(){
    
    assert(sink || source || Deserialize); 

    if(sink != nullptr){
        if (socket->recv(sink->GetICEBuffer()->GetzmqData(), zmq::recv_flags::dontwait) == -1)
        {
            std::cout << zmq_strerror(errno) << std::endl;
        }
        Deserialize(sink->GetICEBuffer()->GetzmqData(),sink->GetICEBuffer()->GetUdataV());
    }else{
        if (socket->recv(source->GetICEBuffer()->GetzmqData(), zmq::recv_flags::dontwait) == -1)
        {
            std::cout << zmq_strerror(errno) << std::endl;
        }
        Deserialize(source->GetICEBuffer()->GetzmqData(),source->GetICEBuffer()->GetUdataV());
    }
}

void
Push_Element::process(){

    assert(sink || source || Serialize); 

    std::string buffer; 

    if(sink != nullptr){
        Serialize(sink->GetICEBuffer()->GetzmqData(),sink->GetICEBuffer()->GetUdataV());
        if (socket->send(sink->GetICEBuffer()->GetzmqData(),zmq::send_flags::none) == -1)
        {
            std::cout << zmq_strerror(errno) << std::endl;
        }
        
    }else{
        Serialize(source->GetICEBuffer()->GetzmqData(),source->GetICEBuffer()->GetUdataV());
        if (socket->send(source->GetICEBuffer()->GetzmqData(),zmq::send_flags::none) == -1)
        {
            std::cout << zmq_strerror(errno) << std::endl; 
        }
    }
}

void
Filter_Element::process(){

    assert(sink || source); 

    if(cb_ != nullptr){
        if(sink != nullptr){
            std::cout << "sink is not nullptr so taking its buffer\n";
            cb_(*sink->GetICEBuffer()); //in message = topic

        }else{
            std::cout << "source is not nullptr so taking its buffer\n";
            cb_(*source->GetICEBuffer()); //in message = topic

        }
    }
}


[[noreturn]] void HandleInvalid(std::string&& msg){
    std::cerr << msg <<"\n";
    assert(false);
    std::exit(1);
}