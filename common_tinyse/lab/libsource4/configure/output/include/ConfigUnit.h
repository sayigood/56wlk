/***************************************************************************
 * 
 * 
 **************************************************************************/



/**
 * @file ConfigUnit.h
 * @brief 
 *  
 **/

 
#ifndef  __CONFIGUNIT_H_
#define  __CONFIGUNIT_H_

#include "utils/cc_utils.h"
#include "bsl/var/IVar.h"
#include "bsl/var/Ref.h"
#include "bsl/ResourcePool.h"
#include <vector>

namespace comcfg{
	enum{
		CONFIG_UNIT_TYPE = 0,
		CONFIG_GROUP_TYPE,
		CONFIG_ARRAY_TYPE,
		CONFIG_ERROR_TYPE
	};
	class Reader;
	class ConfigGroup;
	class ConfigUnit{
		public:
			/**
			 * @brief 方括号重载（三个函数），支持以方括号获取相应字段
			 * 			支持字符串、bsl::string和int下标。int下标Array专用。
			 * 			如果出错，返回一个ConfigError类型的常量
			 * 			可以用 conf[""].selfType() != CONFIG_ERROR_TYPE来判断
			 *
			 * @return  const ConfigUnit& [] 
			 * @retval   
			 * @see 
			**/
			virtual const ConfigUnit & operator[] (const char *) const;
			virtual const ConfigUnit & operator[] (const str_t &) const;
			virtual const ConfigUnit & operator[] (int) const;

			virtual  ConfigUnit & operator[] (const char *) ;
			virtual  ConfigUnit & operator[] (const str_t &) ;
			virtual  ConfigUnit & operator[] (int) ;
			virtual  ConfigUnit & operator= (ConfigUnit & unit) ;
			
			/**
			 * @brief 将一个ErrCode转化为一个对应的字符串信息
			 * 			ErrCode内部就是一个int, 0为OK，非0为出错信息
			 *
			 * @return  const char* 
			 * @retval   
			 * @see 
			**/
			virtual const char * seeError(const ErrCode &) const;
			virtual ~ConfigUnit();

			/**
			 * @brief 从一个字段中获取特定类型的数据
			 * 			XXX to_XXX() 表示以XXX类型获取该配置项
			 *
			 * @param [out] errCode   : ErrCode* 出错信息
			 * 				errCode不为NULL时，错误号将会写入*errCode
			 * 				当此处errCode为默认值NULL时，出错将会以异常的形式抛出
			 * 				抛出的异常为：ConfigException或其子类
			 * @return  char 
			 * @retval   
			 * @see 
			**/
			virtual char to_char(ErrCode * errCode = NULL)const;
			virtual unsigned char to_uchar(ErrCode * errCode = NULL)const;
			virtual int16_t to_int16(ErrCode * errCode = NULL)const;
			virtual u_int16_t to_uint16(ErrCode * errCode = NULL)const;
			virtual int to_int32(ErrCode * errCode = NULL)const;
			virtual u_int32_t to_uint32(ErrCode * errCode = NULL)const;
			virtual long long to_int64(ErrCode * errCode = NULL)const;
			virtual unsigned long long to_uint64(ErrCode * errCode = NULL)const;
			virtual float to_float(ErrCode * errCode = NULL)const;
			virtual double to_double(ErrCode * errCode = NULL)const;
			virtual bsl_string to_bsl_string(ErrCode * errCode = NULL)const;
			//to_std_string : 返回值为std::string，如果是双引号开头的串，这里的字符串是经过转义的，\'\x41\' -> 'A'
			//virtual std_string to_std_string(ErrCode * errCode = NULL)const;
			//to_raw_string : 未经过转义处理的字符串
			virtual str_t to_raw_string(ErrCode * errCode = NULL)const;
			//获取C风格的字符串（常量）
			virtual const char * to_cstr(ErrCode * errCode = NULL)const;

			/**
			 * @brief 带有默认值的to_XXX系列函数
			 *        返回值与to_xxx系列完全相同。
			 *        唯一的不同是：当errCode不为0（即发生错误）, 返回def默认值
			 *
			 * @param [out] errCode   : ErrCode* 出错信息
			 * @param [in] def   : const &  默认值
			 * @return  
			**/
			virtual char to_char(ErrCode * errCode, const char & def)const;
			virtual unsigned char to_uchar(ErrCode * errCode, const unsigned char & def)const;
			virtual int16_t to_int16(ErrCode * errCode, const int16_t & def)const;
			virtual u_int16_t to_uint16(ErrCode * errCode, const u_int16_t & def)const;
			virtual int to_int32(ErrCode * errCode, const int & def)const;
			virtual u_int32_t to_uint32(ErrCode * errCode, const u_int32_t & def)const;
			virtual long long to_int64(ErrCode * errCode, const long long & def)const;
			virtual unsigned long long to_uint64(ErrCode * errCode, const unsigned long long & def)const;
			virtual float to_float(ErrCode * errCode, const float & def)const;
			virtual double to_double(ErrCode * errCode, const double & def)const;
			virtual bsl_string to_bsl_string(ErrCode * errCode, const bsl_string & def)const;
			virtual str_t to_raw_string(ErrCode * errCode, const str_t & def)const;
			//获取C风格的字符串（常量）
			virtual const char * to_cstr(ErrCode * errCode, const char * def)const;



			/**
			 * @brief 与to_XXX功能相同的函数。不过get_XXX系列为C风格
			 * 			传入为用于放值的buffer，返回值是错误号
			 *
			 * @param [out] valueBuf   : char*
			 * @return  ErrCode 
			 * @retval   
			 * @see 
			**/
			virtual ErrCode get_char(char * valueBuf)const;
			virtual ErrCode get_uchar(unsigned char * valueBuf)const;
			virtual ErrCode get_int16(int16_t * valueBuf)const;
			virtual ErrCode get_uint16(u_int16_t * valueBuf)const;
			virtual ErrCode get_int32(int * valueBuf)const;
			virtual ErrCode get_uint32(u_int32_t * valueBuf)const;
			virtual ErrCode get_int64(long long * valueBuf)const;
			virtual ErrCode get_uint64(unsigned long long * valueBuf)const;
			virtual ErrCode get_float(float * valueBuf)const;
			virtual ErrCode get_double(double * valueBuf)const;
			virtual ErrCode get_bsl_string(bsl_string * valueBuf)const;
			//virtual ErrCode get_std_string(std_string * valueBuf) const;
			virtual ErrCode get_raw_string(str_t * valueBuf) const;
			//获取C风格的字符串，将字符串填入valueBuf，len是valueBuf长度，自动补0，最多填充len-1个字符
			virtual ErrCode get_cstr(char * valueBuf, size_t len) const;


			/**
			 * @brief 带有默认值的ge_XXX系列函数
			 *        返回值与get_xxx系列完全相同。
			 *        唯一的不同是：当返回值不为0（即发生错误）valueBuf中使用def默认值
			 *
			 * @param [out] valueBuf   : char*  传出的值
			 * @param [in] def   : const char&  默认值
			 * @return  ErrCode 
			 * @retval   
			 * @see 
			**/
			virtual ErrCode get_char(char * valueBuf, const char & def)const;
			virtual ErrCode get_uchar(unsigned char * valueBuf, const unsigned char & def)const;
			virtual ErrCode get_int16(int16_t * valueBuf, const int16_t & def)const;
			virtual ErrCode get_uint16(u_int16_t * valueBuf, const u_int16_t & def)const;
			virtual ErrCode get_int32(int * valueBuf, const int & def)const;
			virtual ErrCode get_uint32(u_int32_t * valueBuf, const u_int32_t & def)const;
			virtual ErrCode get_int64(long long * valueBuf, const long long & def)const;
			virtual ErrCode get_uint64(unsigned long long * valueBuf, const unsigned long long & def)const;
			virtual ErrCode get_float(float * valueBuf, const float & def)const;
			virtual ErrCode get_double(double * valueBuf, const double & def)const;
			virtual ErrCode get_bsl_string(bsl_string * valueBuf, const bsl_string & def)const;
			virtual ErrCode get_raw_string(str_t * valueBuf, const str_t & def)const;
			virtual ErrCode get_cstr(char * valueBuf, size_t len, const char * def)const;

			/**
			 * @brief 将自身转化为一个IVar
			 *
			 * 		  会抛异常
			 *
			 * @param [in/out] vpool   : VarPool* IVar句柄表，所以新分配的IVar都注册到VarPool
			 *                                    当需要销毁我创建的IVar时，只需要vpool->destroy();
			 * @param [in] errCode   : ErrCode* 错误号，0为正确，其余表示出错
			 * @return  IVar& 
			 * @retval   
			 * @see 
			**/
			virtual bsl::var::IVar& to_IVar(bsl::ResourcePool* vpool, ErrCode* errCode = NULL)const;
			/**
			 * @brief 获取自己的类型
			 *
			 * @return  int 
			 * @retval   
			 * @see 
			**/
			virtual int selfType()const{
				return CONFIG_UNIT_TYPE;
			}
			
			/**
			 * @brief 获取元素个数
			 *
			 * @return  size_t 个数
			 * @retval   
			 * @see 
			**/
			virtual size_t size()const{
				return 1;
			}

			/**
			 * @brief 长路径Unit的直接获取
			 *
			 * @param [in] path   : const str_t 长路径，例如 group.sub.key
			 * @return  const ConfigUnit& 长路径获取的ConfigUnit
			**/
			virtual const ConfigUnit & deepGet(const str_t path) const;

			/**
			 * @brief 访问不存在的key后，给出不存在的key的具体路径。比如 group.sub.nokey
			 *
			 * @return  const char*  不存在的key的路径
			**/
			const char * getErrKeyPath() const;

			/**
			 * @brief 获取子unit。ConfigUnit直接返回error unit
			 *
			 * @return  ConfigUnit &
			**/
			virtual const ConfigUnit & get_sub_unit(int index) const;
			/**
			 * @brief 比较是否相等
			 *
			 * @return  int 0表示相等 -1不相等
			**/
			virtual int equals(const ConfigUnit & conf) const;

			//以下函数的参数中，except为0则用错误号表示错误，
			//except为1则用异常表示错误
			/**
			 * @brief 增加一个unit, 连接到子unit，不做深拷贝
			 *
			 * @param [in] key   : 新增unit的key
			 * @param [in] value : 新增unit的value
			 * @param [in] objtype : unit类型。
			 *    CONFIG_UNIT_TYPE表示unit;
			 *    CONFIG_GROUP_TYPE表示group;
			 *    CONFIG_ARRAY_TYPE表示array;
			 * @return  int 0 成功，其他是错误号
			**/
			virtual int add_unit(const bsl_string & key, const bsl_string& value, 
				const int objtype=CONFIG_UNIT_TYPE, int except=0, ConfigUnit ** ref=NULL);

			/**
			 * @brief 复制一个unit, 深拷贝
			 *
			 *  将unit的内容追加到本对象下面。
			 * 类型需要匹配，只能从group复制到group，array复制到array
			 * group要检查key是否重复，不重复则追加
			 * array直接追加
			 * @return  int 0 成功，其他是错误号
			**/
			virtual int append_unit(const ConfigUnit & unit, int except=0);

			/**
			 * @brief 复制一个unit, 深拷贝
			 *
			 *  将unit的内容复制到本对象。
			 * 类型需要匹配，只能从group复制到group，array复制到array
			 * @return  int 0 成功，其他是错误号
			**/
			virtual int copy_unit(const ConfigUnit & unit, int except=0);
			
			/**
			 * @brief 删除一个unit
			 *
			 * key : 要删除的unit的key
			 * @return  int 0 成功，其他是错误号
			**/
			virtual int del_unit(const bsl_string & key, int except=0);
			/**
			 * @brief 修改unit中value的值
			 *
			 * @return  int 0 成功，其他是错误号
			**/
			virtual int set_value(const bsl_string & value, int except=0);


			//-----------------从这里往下的东西用户不需要关心-----------------

			//构造函数，
			ConfigUnit();
			ConfigUnit(const str_t& __key, const str_t& __value, const Reader * __cur_reader = NULL, ConfigGroup * father = NULL);

			//调试打印时用，缩进
			void pindent(int ind)const{
				while(ind--){
					printf("    ");
				}
			}
			//打印
			virtual void print(int indent = 0)const{
				pindent(indent);
				printf("=[%s], _value=[%s]\n", _key.c_str(), _value.c_str());
			}


			//for debug or writelog
			virtual str_t info() const{
				//const int INFO_BUF_SIZE = 256;
				//char buf[INFO_BUF_SIZE] = "";
				str_t buf;
				buf.appendf("[File:%s Line:%d Key:%s Value:%s]", 
						_at_file ? _at_file : "NULL", _at_line, _key.c_str(), _value.c_str());
				return buf;
			}
			//for child class
			virtual ConfigUnit * relativeSection(str_t, int* ){
				return NULL;
			}

			static ConfigUnit* get_err_unit();
			virtual const bsl::string & get_key_name() const;
		protected:
			virtual void popError(ErrCode *, ErrCode) const;
			void setErrorKeyPath(str_t str)const;
			const ConfigUnit & deepGetSegment(const str_t segment) const; 
			
			static const str_t g_unknown;
			str_t _key;
			str_t _value;
			str_t _cstr; //专门的一个被解析后的string，只有当value以"为开头时才起作用
			int _cstr_err;
			str_t _vstring;
			const char * _at_file;
			int _at_line;
			ConfigGroup * _father;
			//mutable str_t _to_def_str;
			ConfigUnit * create_unit(const bsl_string & key, const bsl_string& value, 
				int objtype, ConfigGroup * father);

			/**
			 * @brief 清除内容
			 *
			 * @return  void
			**/
			virtual void clear();
			
		public:
			/**
			 * @brief 复制一个unit, 深拷贝
			 *
			 *  将unit的内容复制到本对象下面。
			 * 类型需要匹配，只能从group复制到group，array复制到array
			 * group要检查key是否重复，不重复则追加
			 * array直接追加
			 * @return  int 0 成功，其他是错误号
			**/
			virtual int _append_unit(const ConfigUnit & unit,int check=1, int except=0);
		private:
			int init_unit(const str_t& __key, const str_t& __value);
	};

}









#endif  //__CONFIGUNIT_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
