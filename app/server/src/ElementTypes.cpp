#include <ElementTypes.h>


/**
 * ------------------------------------------------------------------------- @brief Element_T 
***/

int 
Element_T::opt(ElemOPT opt, std::string arg){
    switch(opt){
        case ENDPOINT:
            break;
        case SOCKOPT:
            break;
        default:
            return -1;
            break;
    }
    return 0;
}