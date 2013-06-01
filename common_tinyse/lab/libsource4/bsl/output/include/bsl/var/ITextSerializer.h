/***************************************************************************
 * 
 * 
 **************************************************************************/
 
 
 
/**
 * @file ITextSerializer.h
 * @brief 
 *  
 **/
#ifndef  __BSL_VAR_ITEXT_SERIALIZER_H__
#define  __BSL_VAR_ITEXT_SERIALIZER_H__

#include "bsl/var/IVar.h"
#include "bsl/var/Ref.h"
#include "bsl/exception/bsl_exception.h"
#include "bsl/exception/bsl_auto_buffer.h"
namespace bsl{
namespace var{

class ITextSerializer{
    public:
        virtual ~ITextSerializer(){
        }
        virtual void serialize(const IVar& var, bsl::AutoBuffer& buf) = 0;
    };

}}   //namespace bsl::var

#endif  //__BSL_VAR_ITEXT_SERIALIZER_H__

/* vim: set ts=4 sw=4 sts=4 tw=100 */
