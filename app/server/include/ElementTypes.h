#pragma once

#include <iostream>
#include <zmq.hpp>

class EFactory;

enum ElemOPT{
    ENDPOINT,
    SOCKCREATE, 
    SOCKOPT,
    NOOPT
};

enum RSLT{
    OK,
    WARN,
    NOK
};

#ifdef ZMQ_CPP11
enum class Element_type : int
{
    req = ZMQ_REQ,
    rep = ZMQ_REP,
    dealer = ZMQ_DEALER,
    router = ZMQ_ROUTER,
    pub = ZMQ_PUB,
    sub = ZMQ_SUB,
    xpub = ZMQ_XPUB,
    xsub = ZMQ_XSUB,
    push = ZMQ_PUSH,
    pull = ZMQ_PULL,
#if defined(ZMQ_BUILD_DRAFT_API) && ZMQ_VERSION >= ZMQ_MAKE_VERSION(4, 2, 0)
    server = ZMQ_SERVER,
    client = ZMQ_CLIENT,
    radio = ZMQ_RADIO,
    dish = ZMQ_DISH,
    gather = ZMQ_GATHER,
    scatter = ZMQ_SCATTER,
    dgram = ZMQ_DGRAM,
#endif
#if defined(ZMQ_BUILD_DRAFT_API) && ZMQ_VERSION >= ZMQ_MAKE_VERSION(4, 3, 3)
    peer = ZMQ_PEER,
    channel = ZMQ_CHANNEL,
#endif
#if ZMQ_VERSION_MAJOR >= 4
    stream = ZMQ_STREAM,
#endif
    pair = ZMQ_PAIR,
    filter = 200,
    qeue
};
#endif

/**
 * @class Interconnect Element
 * @brief Struct for managing interconnection between element-type objects
 *
 * @details - data transfer between objects 
 * @details - data can be probed to reveal whats inside
 */
struct ICElement{ 
public:
    void SetBuffer(zmq::message_t newvalue){buffer = std::move(newvalue);}
    zmq::message_t* GetBuffer(){return &buffer;};

    ICElement() = default;
    ~ICElement() = default;
private:

    zmq::message_t buffer;
    std::shared_ptr<ICElement> sink;
    std::shared_ptr<ICElement> source;
};

/**
 * @class Element-Type template
 * @brief Template for derived Element implementations
 */
class Element_T{
    friend EFactory;
public:
    virtual void process() = 0; //main activity == send/receive/filter 
    
protected:
    void* (*cb_)(){nullptr};

    zmq::context_t& context;
    std::shared_ptr<ICElement> sink {nullptr};
    std::shared_ptr<ICElement> source {nullptr};
    std::unique_ptr<zmq::socket_t> socket {nullptr};

    Element_T(zmq::context_t& ctx): context{ctx}{};
    virtual ~Element_T() = default;
private:

};

class EFactory{
public:
    std::unique_ptr<Element_T> build();
    template<typename argT>
    RSLT opt(ElemOPT opt, argT  arg, 
             std::optional<const void*> optval_= std::nullopt , std::optional<size_t> size = std::nullopt);

    EFactory(zmq::context_t& ctx): context{ctx} {type = static_cast<zmq::socket_type>(-1);}
    ~EFactory() = default;
private:
    void reset() {type = static_cast<zmq::socket_type>(-1);}
    void CreateElement(Element_type type);
    zmq::context_t& context;
    zmq::socket_type type;
    std::unique_ptr<Element_T> element{nullptr};
};

class Sub_Element : public Element_T {
public:
    Sub_Element(zmq::context_t& ctx) : Element_T(ctx) {} 
    ~Sub_Element() override {} 
    
    void process() override;
};

class Push_Element : public Element_T {
public:
    Push_Element(zmq::context_t& ctx) : Element_T(ctx) {}  
    ~Push_Element() override {} 
    
    void process() override;
};

class Filter_Element : public Element_T {
public:
    Filter_Element(zmq::context_t& ctx) : Element_T(ctx) {} 
    ~Filter_Element() override {} 
    
    void process() override;
};