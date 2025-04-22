#include <BManager.h>


/**
 * ------------------------------------------------------------------------- @brief BManager 
***/
void StartAll(){
    std::cerr << "not implemented yet\n";
    exit(1);
}
void 
BManager::stopSingle(std::shared_ptr<Pipeline_T>& ref){
    for(auto& pipeline : pipelines){
        if(pipeline == ref){
            pipeline->Shutdown();
        }
    }
}
void 
BManager::stopAll(){
    for(auto& pipeline : pipelines){
            pipeline->Shutdown();
    }
}

void 
BManager::Run(){
    while (true) {
        bool any_running = false;

        for (auto& pipeline : pipelines) {
            std::cout << "traversed pipeline\n";
            if (pipeline->GetStatus()) {
                pipeline->pollevents();
                any_running = true;
            }
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
        std::cerr << "must be atleast 2 elements to create a working pipeline!\n";
        exit(1);
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

        std::cout << "entered eventhandler\n";
        elem_index= elem_index % linkedElems.size(); //keep rotating around the object list
        auto& pollitem = linkedElems[elem_index];  
        std::cout << "event:" << elem_index << "currently being checked\n";
        if(pollitem->eventhandle!=nullptr){ //if its filter /qeue it does not have a socket so nothing to poll
            std::cout << "found socket checking for event\n";
            zmq::pollitem_t* item = *pollitem.get()->eventhandle;
            try{
                if(zmq::poll(item, 1,std::chrono::milliseconds(-1)) == -1){
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
    
    if(!IsContinous && elem_index == linkedElems.size()){
        //if oneshot and the pipeline has been traversed
        status =false;
    }
}

/**
 * ------------------------------------------------------------------------- @brief BMessage 
***/

std::string 
BMessage::ToTopic(zmq::message_t& forwarded_data){
    std::string result = forwarded_data.to_string();
    size_t index=0;
    for(int f=0;f<3; f++){ // topic>session>name>
        index = result.find(">",index+1);
    }
    result= result.substr(0,index+1);
    std::cout << "topic>session>name> ->" << result << "\n";
    return result;
}  

std::string 
BMessage::ToAnswer(zmq::message_t& forwarded_data){
    std::string result = forwarded_data.to_string();
    size_t index=0;
    for(int f=0;f<3; f++){ // topic>session>name>
        index = result.find(">",index+1);
    }
    result= result.substr(0,index+1);
    index = result.find("?");
    result[index] = '!';

    std::cout << "answers to:" << result << "\n";
    return result;
}  