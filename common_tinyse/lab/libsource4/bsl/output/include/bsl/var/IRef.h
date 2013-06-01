/***************************************************************************
 * 
 * 
 **************************************************************************/
 
 
 
/**
 * @file IRef.h
 * @brief 
 *  
 **/
#ifndef  __BSL_VAR__IREF_H_
#define  __BSL_VAR__IREF_H_
#include <bsl/var/IVar.h>
namespace bsl{ namespace var {
    class IRef: public IVar{
    public:
        typedef IVar::string_type   string_type;
        typedef IVar::field_type    field_type;
        virtual IVar& ref() const = 0;
        using IVar::operator=;
    };  //end of class 
}}//end of namespace


#endif  //__BSL_VAR__IREF_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
