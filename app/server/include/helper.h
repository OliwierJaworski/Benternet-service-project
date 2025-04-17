#pragma once

#include <BManager.h>

struct PItems{
    std::vector<std::unique_ptr<Element_T>> elements;
    void push_back(std::unique_ptr<Element_T> elem);
};


void 
PItems::push_back(std::unique_ptr<Element_T> elem){
}