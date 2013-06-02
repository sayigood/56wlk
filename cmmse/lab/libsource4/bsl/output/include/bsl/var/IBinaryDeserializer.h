/***************************************************************************
 * 
 * 
 **************************************************************************/
 
 
 
/**
 * @file IBinaryDeserializer.h
 * @brief mcpack -> IVar
 *  
 **/


#ifndef  __BSL_VAR_IBINARY_DESERIALIZER_H__
#define  __BSL_VAR_IBINARY_DESERIALIZER_H__

#include "bsl/var/IVar.h"
namespace bsl{
namespace var{

class IBinaryDeserializer{
public:
    virtual ~IBinaryDeserializer(){
    }
    virtual IVar& deserialize(const void* buf, size_t max_size) = 0;
};
}}   //namespace bsl::var


#endif  //__BSL_VAR_IBINARY_DESERIALIZER_H__

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
