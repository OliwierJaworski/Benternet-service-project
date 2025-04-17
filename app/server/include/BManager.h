#pragma once

#include <cstdarg>
#include <iostream>
#include <zmq.hpp>
#include <nlohmann/json.hpp>
#include <ElementTypes.h>

class BManager;

typedef std::string (*Pollevent_cbF)(std::string&& message, void* data, bool* cb_socket);

/**
 * @class templated PollItem_T
 * @brief structure for event logging and callbacks
 *
 * @details - variable type of Uarg user can provide own datatype for specific data manipulation
 * @example - provide datatype to store image, text or audio data which will be used in the pipeline
 */
struct PollItem_T{
    zmq::pollitem_t item;
    Pollevent_cbF cbF;

    operator zmq::pollitem_t*() {return &item;}; 
    PollItem_T(void* socket_, Pollevent_cbF cbF_, short eventtype_) : item{ .socket= socket_, .fd=0, .events = eventtype_, .revents=0},
                                                                      cbF{cbF_}{}
private:
};

/**
 * @class Pipeline-Type template
 * @brief Template for derived Pipeline implementations
 */
class Pipeline_T{
private:
    zmq::context_t& context;
    std::vector< std::unique_ptr<Element_T> > Elements;
    std::vector<PollItem_T> items;

public:

    int ElementLink(va_arg, ...);
    int addLinked(va_arg ...);

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
    zmq::context_t context{1};
    static std::vector< std::shared_ptr<Pipeline_T> > pipelines;

    BManager();
    ~BManager();
public:
    inline static BManager& instance() { static BManager BManagers; return BManagers; }
};






