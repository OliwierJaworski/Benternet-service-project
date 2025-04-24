#pragma once

#include <cstdarg>
#include <iostream>
#include <zmq.hpp>
#include <nlohmann/json.hpp>
#include <ElementTypes.h>

class BManager;
class PFactory;

/**
 * @class Pipeline-Type template
 * @brief Template for derived Pipeline implementations
 */
class Pipeline_T{
   friend PFactory;
public:
    template<typename... Ptrs>
    int ElementLink(std::unique_ptr<Element_T>&& first, Ptrs&&... rest);//loop over atleast 1 item so user does not call empty function
    void pollevents();
    void Shutdown();
    void SetStatus(bool status_){status = status_;}
    bool GetStatus(){return status;}// eg. running or not 
    void SetIsContinous(bool status_){IsContinous = status_;}
    bool GetIsContinous(){return IsContinous;}// eg. running or not 
    
    ~Pipeline_T(){}
private:

    template<typename UType>
    static std::shared_ptr<Pipeline_T> create(zmq::context_t& ctx, UType& data) {std::cout<< "\n usertype2:"<< data.type().name()<< "\n"; return std::shared_ptr<Pipeline_T>(new Pipeline_T(ctx, std::move(data)));}
    template<typename UType>
    Pipeline_T(zmq::context_t& context_, UType data_): context{context_}{std::cout<< "\n usertype3:"<< data_.type().name()<< "\n"; if(!buffer) buffer = std::make_shared<Bbuffer>(); buffer->SetUdata(data_); }
    
    bool status{false}; // offline| not running
    bool IsContinous{false}; //whether its oneshot or not
    int elem_index{0};
    std::shared_ptr<Bbuffer> buffer{nullptr};
    zmq::context_t& context;
    std::vector<std::shared_ptr<Element_T>> linkedElems;
    void IClink(); //connect each Element with the next using ICElements
};



/**
 * @class Benternet Manager
 * @brief class for managing pipelines and event tracking
 *
 * @details - manages created /added pipelines
 * @details - does eventtracking for zmq sockets
 */

class BManager{
public:
    inline static BManager& instance() { static BManager BManagers; return BManagers; }
    inline static zmq::context_t* context() { static zmq::context_t context{1}; return &context;} 

    //pipeline running 
    inline void EnableSingle(std::shared_ptr<Pipeline_T>& ref) {ref->SetStatus(true); pipelines.push_back(ref);}
    inline void EnableAll() { for(auto& pipeline : pipelines) pipeline->SetStatus(true);}

    //pipeline not running 
    inline void stopSingle(std::shared_ptr<Pipeline_T>& ref) {ref->SetStatus(false); pipelines.push_back(ref);}
    inline void stopAll() { for(auto& pipeline : pipelines) pipeline->SetStatus(false);}

    //delete pipeline with elements
    inline void DropSingle(std::shared_ptr<Pipeline_T>& ref) {ref->Shutdown();}
    inline void DropAll() { for(auto& pipeline : pipelines) pipeline->Shutdown();}

    //execute traversal of pipelines
    void Run();
    //shutdown the context and sockets
    inline void shutdown() {for(auto& pipeline : pipelines) pipeline->Shutdown(); BManager::context()->close();}

private:
    BManager(){}
    ~BManager(){}
    
    std::vector<std::shared_ptr<Pipeline_T>> pipelines;
};



template<typename... Ptrs>
int 
Pipeline_T::ElementLink(std::unique_ptr<Element_T>&& first, Ptrs&&... rest){
    linkedElems.push_back(std::move(first));
    (linkedElems.push_back(std::move(rest)), ...); 
    IClink();
    return 0;
}

class Pipeline_W{
public:
    template<typename... Ptrs>
    inline int ElementLink(std::unique_ptr<Element_T>&& first, Ptrs&&... rest) { return pipeline->ElementLink(std::move(first), std::forward<Ptrs>(rest)...);}

    operator std::shared_ptr<Pipeline_T>& () {return pipeline;} 

    Pipeline_W( std::shared_ptr<Pipeline_T>& pipeline_){pipeline = pipeline_;}
    ~Pipeline_W() = default;
private:
    std::shared_ptr<Pipeline_T> pipeline;
};

class PFactory{
    public:
        Pipeline_W build();
        template <typename Utype>
        inline void UserDataType(Utype& data){buffer = std::make_any<Utype>(std::move(data)); std::cout<< "\n usertype:"<< buffer.type().name()<< "\n"; } //if user type needs default values this does not work

        PFactory(zmq::context_t& ctx): context{ctx} {}
        ~PFactory() = default;
    private:
    
        void reset();
        zmq::context_t& context;
        std::any buffer;
};

struct testtype{
    int data{22};
    std::string somestring{"hello from custom data structure!"};
};