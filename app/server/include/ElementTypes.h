#pragma once

#include <iostream>
#include <zmq.hpp>

class EFactory;
class Pipeline_T;
class Element_T;

typedef void (*Pollevent_cbF)(zmq::message_t& forwarded_data);

enum ElemOPT{
    ENDPOINT,
    SOCKCREATE, 
    SOCKOPT,
    SOCK_CB,
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
 * @class templated PollItem_T
 * @brief structure for event logging and callbacks
 *
 * @details - variable type of Uarg user can provide own datatype for specific data manipulation
 * @example - provide datatype to store image, text or audio data which will be used in the pipeline
 */
struct PollItem_T{
    operator zmq::pollitem_t*() {return &item;}; 
    PollItem_T(void* socket_, short eventtype_, Element_T& elem) : item{ .socket= socket_, .fd=-1, .events = eventtype_, .revents=0}, element{elem}{}
private:
    zmq::pollitem_t item;
    Element_T& element;
};

/**
 * @class Interconnect Element
 * @brief Struct for managing interconnection between element-type objects
 *
 * @details - data transfer between objects 
 * @details - data can be probed to reveal whats inside
 */
struct ICElement{
    friend Pipeline_T;
public:
    void SetBuffer(zmq::message_t newvalue){*buffer = std::move(newvalue);}
    zmq::message_t& GetBuffer(){return *buffer;};

    ICElement(std::shared_ptr<Element_T>& sink_, std::shared_ptr<Element_T>& source_): sink{sink_}, source{source_}{}
    ~ICElement() = default;
private:
    std::shared_ptr<zmq::message_t> buffer;
    std::shared_ptr<Element_T> sink {nullptr};
    std::shared_ptr<Element_T> source {nullptr};
};

/**
 * @class Element-Type template
 * @brief Template for derived Element implementations
 */
class Element_T{
    friend EFactory;
    friend Pipeline_T;
public:
    virtual void process() = 0; //main activity == send/receive/filter 
    void SetBuffer(zmq::message_t newvalue){sink->GetBuffer() = std::move(newvalue);}
    zmq::message_t& GetBuffer(){return sink->GetBuffer();};

    virtual ~Element_T() = default;
protected:
    std::unique_ptr<PollItem_T> eventhandle{nullptr};
    Pollevent_cbF cb_{nullptr};
    std::vector<std::string> caps;
    zmq::context_t& context;
    std::shared_ptr<ICElement> sink {nullptr};
    std::shared_ptr<ICElement> source {nullptr};
    std::unique_ptr<zmq::socket_t> socket {nullptr};

    Element_T(zmq::context_t& ctx): context{ctx}{};
    
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

//cheat to fix template nonsense
template<typename argT>
RSLT 
EFactory::opt(ElemOPT opt, argT arg, std::optional<const void*> optval_, std::optional<size_t> size){
    switch(opt){
        case ENDPOINT:
            if constexpr (std::is_convertible_v<argT, std::string>) {
                if (element) {
                    element->socket->connect(arg);
                    return RSLT::OK;
                } else {
                    std::cerr << "Element not created before ENDPOINT set.\n";
                    return RSLT::NOK;
                }
            } else {
                std::cerr << "Invalid argument type for ENDPOINT (should be string-like).\n";
                return RSLT::NOK;
            }

        case SOCKCREATE:
            if constexpr (std::is_same_v<argT, Element_type>) {
                CreateElement(arg);
                return RSLT::OK;
            } else {
                std::cerr << "Invalid argument type for SOCKCREATE (should be Element_type).\n";
                return RSLT::NOK;
            }

        case SOCKOPT:
            if constexpr (std::is_same_v<argT, int>) {
                if (element && optval_.has_value() && size.has_value()) {
                    element->socket->setsockopt(arg, optval_.value(), size.value());
                    return RSLT::OK;
                } else {
                    std::cerr << "Missing socket or optional parameters for SOCKOPT.\n";
                    return RSLT::NOK;
                }
            } else {
                std::cerr << "Invalid argument type for SOCKOPT (should be int).\n";
                return RSLT::NOK;
            }
        case SOCK_CB:
            if constexpr (std::is_same_v<argT, Pollevent_cbF>) {
               element->cb_ = arg;
               return RSLT::OK;
            } else {
                std::cerr << "Invalid argument type for SOCK_CB (should be Pollevent_cbF).\n";
                return RSLT::NOK;
            }
        default:
            std::cerr << "Unsupported ElemOPT value.\n";
            return RSLT::NOK;
    }
}
