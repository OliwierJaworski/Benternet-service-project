#pragma once

#include <cstdarg>
#include <iostream>
#include <zmq.hpp>
#include <nlohmann/json.hpp>
#include <ElementTypes.h>

class BManager;


/**
 * @class Pipeline-Type template
 * @brief Template for derived Pipeline implementations
 */
class Pipeline_T{
private:
    bool status{false}; // offline| not running
    bool IsContinous{false}; //whether its oneshot or not
    int elem_index{0};
    zmq::context_t& context;
    std::vector<std::shared_ptr<Element_T>> linkedElems;
    void IClink(); //connect each Element with the next using ICElements
    void pollevents();
public:

    template<typename... Ptrs>
    int ElementLink(std::unique_ptr<Element_T>&& first, Ptrs&&... rest);//loop over atleast 1 item so user does not call empty function
    void SetStatus(bool status_){status = status_;}
    bool GetStatus(){return status;}// eg. running or not 
    void SetIsContinous(bool status_){IsContinous = status_;}
    bool GetIsContinous(){return IsContinous;}// eg. running or not 

    Pipeline_T(zmq::context_t& context_);
    ~Pipeline_T();
};

/**
 * @class Benternet Manager
 * @brief class for managing pipelines and event tracking
 *
 * @details - manages created /added pipelines
 * @details - does eventtracking for zmq sockets
 */

class BManager{
private:
    static std::vector<std::shared_ptr<Pipeline_T>> pipelines;
    
    BManager();
    ~BManager();
public:
    inline static BManager& instance() { static BManager BManagers; return BManagers; }
    inline static zmq::context_t* context() { static zmq::context_t context{1}; return &context;} 

    //pipeline running | not running
    inline static void StartSingle(std::shared_ptr<Pipeline_T>& ref);
    inline static void StartAll();
    inline static void stopSingle(std::shared_ptr<Pipeline_T>& ref);
    inline static void stopAll();
};


template<typename... Ptrs>
int 
Pipeline_T::ElementLink(std::unique_ptr<Element_T>&& first, Ptrs&&... rest){
    linkedElems.push_back(std::move(first));
    (linkedElems.push_back(std::move(rest)), ...); 
    IClink();
    return 0;
}
