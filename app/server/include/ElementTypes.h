#pragma once

#include <iostream>
#include <zmq.hpp>

enum ElemOPT{
    ENDPOINT, 
    SOCKOPT
};

/**
 * @class Interconnect Element
 * @brief Struct for managing interconnection between element-type objects
 *
 * @details - data transfer between objects 
 * @details - data can be probed to reveal whats inside
 */
struct ICElement{ 
private:
    std::shared_ptr<ICElement> sink;
    std::shared_ptr<ICElement> source;
};

/**
 * @class Element-Type template
 * @brief Template for derived Element implementations
 */
class Element_T{
private:
    std::shared_ptr<ICElement> sink {nullptr};
    std::shared_ptr<ICElement> source {nullptr};
    std::unique_ptr<zmq::socket_t> socket {nullptr};
    
public:
    int opt(ElemOPT opt, std::string arg);

    Element_T() = default;
    ~Element_T() = default;
};
