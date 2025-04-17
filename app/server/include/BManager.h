#pragma once

#include <iostream>
#include <zmq.hpp>
#include <nlohmann/json.hpp>


/**
 * @class Interconnect Element
 * @brief Struct for managing interconnection between element-type objects
 *
 * @details - data transfer between objects 
 */
struct ICElement{ 

private:

};

/**
 * @class Element-Type template
 * @brief Template for derived Element implementations
 */
class Element_T
{
private:

public:
    Element_T();
    ~Element_T();
};

/**
 * @class Pipeline-Type template
 * @brief Template for derived Pipeline implementations
 */
class Pipeline_T
{
private:

public:
    Pipeline_T();
    ~Pipeline_T();
};

/**
 * @class Benternet Manager
 * @brief class for managing pipelines and event tracking
 *
 * @details - manages created /added pipelines
 * @details - does eventtracking for zmq sockets
 */
class BManager
{

private:

public:
    BManager(/* args */);
    ~BManager();

};






