#include <BManager.h>


/**
 * ------------------------------------------------------------------------- @brief BManager 
***/

BManager::BManager(){

}

BManager::~BManager(){

}


/**
 * ------------------------------------------------------------------------- @brief Pipeline_T 
***/

Pipeline_T::Pipeline_T(zmq::context_t& context_): context{context_}{
}

Pipeline_T::~Pipeline_T(){
}

void
Pipeline_T::IClink(){
    for (auto cr = linkedElems.begin(); cr + 1 != linkedElems.end(); ++cr) {
        auto& current = *cr;
        auto& next = *(cr + 1);

        current->source = std::make_shared<ICElement>(current,next);
        next->sink = current->source;
    }
}
void 
Pipeline_T::pollevents(){
    elem_index= elem_index % linkedElems.size(); //keep rotating around the object list
    auto& pollitem = linkedElems[elem_index];  

    if(pollitem->eventhandle!=nullptr){ //if its filter /qeue it does not have a socket so nothing to poll
        zmq::pollitem_t* item = *pollitem.get()->eventhandle;
        try{
            if(zmq::poll(item, 1,std::chrono::milliseconds(100)) == -1){
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
        if(item->revents){
            pollitem->process(); //if recv/push etc...
            elem_index++;
        }
    }else{
        pollitem->process(); //if filter // qeue etc...
        elem_index++;
    }
    
}

