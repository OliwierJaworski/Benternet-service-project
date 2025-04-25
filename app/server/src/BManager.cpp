#include <BManager.h>


/**
 * ------------------------------------------------------------------------- @brief BManager 
***/
void 
BManager::Run(){
    while (true) {
        bool any_running = false;

        for (auto& pipeline : pipelines) {
            if (pipeline->GetStatus()) {
                pipeline->pollevents();
                any_running = true;
            }
            std::cout << "traversed pipeline\n";
        }

        if (!any_running) {
            std::cout << "No pipelines running, exiting loop.\n";
            break;
        }
    }
}

/**
 * ------------------------------------------------------------------------- @brief Pipeline_T 
***/

void 
Pipeline_T::Shutdown(){
    SetStatus(false);
    for (auto& elem : linkedElems) {
        if (elem->socket && elem->socket->handle() != nullptr) {
            elem->socket->close();
        }
    }
    linkedElems.clear();
}

void
Pipeline_T::IClink(){
    if(linkedElems.size() < 2){
        HandleInvalid("must be atleast 2 elements to create a working pipeline");
    }
    for (auto cr = linkedElems.begin(); cr + 1 != linkedElems.end(); cr++) {
        auto& current = *cr;
        auto& next = *(cr + 1);

        current->source = std::make_shared<ICElement>(current, next, buffer);
        next->sink = current->source;
    }
}

void 
Pipeline_T::pollevents(){
    
    if(status){
        
        elem_index= elem_index % linkedElems.size(); //keep rotating around the object list

        auto& pollitem = linkedElems[elem_index];  
        if(pollitem->eventhandle!=nullptr){ //if its filter /qeue it does not have a socket so nothing to poll
          //  std::cout << "found socket checking for event\n";
            zmq::pollitem_t* item = pollitem->eventhandle->Item();
            
            if(zmq::poll(item, 1,std::chrono::milliseconds(-1)) == -1){
                std::exception_ptr p = std::current_exception();
                std::clog <<(p ? p.__cxa_exception_type()->name() : "null") << std::endl;
                exit(1);
            }

            if(item->revents == item->events){
                pollitem->process(); //if recv/push etc...
                elem_index++;
                item->revents =0;
            }
        }else{
            pollitem->process(); //if filter // qeue etc...
            elem_index++;
        }

    }
    
    if(!IsContinous && elem_index == linkedElems.size()){
        status =false;  //if oneshot and the pipeline has been traversed
    }
}

Pipeline_W
PFactory::build(){
    assert(buffer.has_value() && "Could not build Pipeline due to missing objects");
    std::shared_ptr<Pipeline_T> tmpP = Pipeline_T::create(context, buffer);
    Pipeline_W wrapper(tmpP);
    reset();
    return std::move(wrapper);
}

void
PFactory::reset(){
    buffer.reset();
}
