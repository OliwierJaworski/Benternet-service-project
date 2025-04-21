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

