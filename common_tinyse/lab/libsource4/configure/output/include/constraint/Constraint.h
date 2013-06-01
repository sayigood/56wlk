/***************************************************************************
 * 
 * 
 **************************************************************************/
 
 
 
/**
 * @file Constraint.h
 * @brief 
 *  
 **/


#ifndef  __CONSTRAINT_H_
#define  __CONSTRAINT_H_

#include "Configure.h"
#include "ConstraintFunction.h"

namespace comcfg{
	class Constraint{
		public:
			/**
			 * @brief 检查约束信息
			 *
			 * @param [in] __idl   : confIDL::idl_t* 约束信息
			 * @param [in/out] __conf   : Configure* 配置信息
			 * @return  int 
			 * @retval   
			 * @see 
			**/
			int run(confIDL::idl_t * __idl, Configure * __conf);
		protected:
			int printError();
			int runGroup(confIDL::var_t* var, ConfigGroup * father, ConfigUnit * self = NULL);
			int runKey(confIDL::var_t* var, ConfigGroup * father, ConfigUnit * self = NULL);
			int runVar(confIDL::var_t* var, ConfigGroup * father, ConfigUnit * self = NULL);
			int runType(const str_t& type, const ConfigUnit & unit);
			Configure * conf;
			confIDL::idl_t * idl;
	};
}
















#endif  //__CONSTRAINT_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
