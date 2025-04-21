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

int 
Pipeline_T::ElementLink(std::initializer_list<std::unique_ptr<Element_T>> ElemList){
    for(auto& elem : ElemList){
        linkedElems.push_back(std::move(const_cast<std::unique_ptr<Element_T>&>(elem))); // removes the const-ness from references and pointers
    }
    return 0;
}

