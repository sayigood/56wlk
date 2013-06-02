/***************************************************************************
 * 
 * 
 **************************************************************************/



/**
 * @file Configure.h
 * @brief 新的Configure库
 *  
 **/


#ifndef  __CONFIGURE_H_
#define  __CONFIGURE_H_

#include "ConfigGroup.h"
#include "sys/uio.h"
#include <vector>

#define CONFIG_GLOBAL "GLOBAL"
#define CONFIG_INCLUDE "$include"

namespace confIDL{
	struct idl_t;
}
namespace comcfg{
	const int MAX_INCLUDE_DEPTH = 1;
	class Reader;
	enum{
		CONFIG_VERSION_1_0,	//Config Version 1.0
	};
	/**
	 * @brief 配置句柄，其实是一个全局的[GLOBAL]配置组
	 *
	 * Configure是immutable的，即：无法修改，无法复用
	 *  所以load, rebuild, loadIVar三个接口互斥
	 * 要重新load，必须创建一个新的Configure句柄进行操作 
	 *
	 */
	class Configure : public ConfigGroup{
		public:
			/**
			 * @brief 从指定的文件中载入配置文件和约束文件
			 *
			 * @param [in] path   : const char* 配置文件的路径
			 * @param [in] conf   : const char* 配置文件名
			 * @param [in] range   : const char* 约束文件
			 * @param [in] version   : int 文件格式的版本（暂未使用）
			 * @return  int  0表示成功，其余为失败
			 * 				bsl::CONSTRAINT_ERROR  约束检查错误（如果不关心约束，可无视此错误）
			 * 				bsl::CONFIG_ERROR 配置文件解析错误
			 * 				bsl::ERROR 其它错误
			 * @retval   
			 * @see 
			 **/
			int load(const char * path, const char * conf, const char * range = NULL, int version = CONFIG_VERSION_1_0);
			/**
			 * @brief 将数据串行化到一个字节流，可以发网网络
			 * 			如果将这个字节流直接写入磁盘文件，不可以直接load回来，而需要读取后再rebuild
			 *
			 * @param [out] size_t * : 输出的字节流长度
			 * @return  char* 输出的字节流
			 * @retval   
			 * @see 
			 **/
			char * dump(size_t *);

			/**
			 * @brief 根据dump的字节流创建Configure
			 * 		根据字节流重建一个Configure的步骤是：
			 * 		1，getRebuildBuffer(size) 获取一个内部缓冲区
			 * 		2，将字节流中的数据拷贝到这个缓冲区
			 * 		3，调用rebuild重建数据
			 *
			 * 		调用本接口会清空现有的数据。
			 *
			 * @param [in] size   : size_t 字节流长度
			 * @return  char* 缓冲区
			 * @retval   
			 * @see 
			 **/
			char * getRebuildBuffer(size_t size);
			/**
			 * @brief 根据获得的字节流重建Configure数据
			 *
			 * @return  int 0为成功，其他为失败
			 * @retval   
			 * @see 
			 **/
			int rebuild();
			/**
			 * @brief 检查所有配置文件的最后更新时间（包括被$include）的配置文件
			 *        $include 限制层数为：MAX_INCLUDE_DEPTH
			 *        在执行lastConfigModify()，应确保原来的配置文件都没有被删除
			 *
			 * @return  time_t 所有配置文件中的最后更新时间
			 *                 返回0表示出错
			 * @retval   
			 * @see 
			**/
			time_t lastConfigModify();
			/**
			 * @brief 从一个IVar载入配置文件的数据
			 *        本接口与load/rebuild互斥
			 *        这个IVar必须是一个Dict类型
			 *
			 * @return  int 0成功，其余失败
			 * @retval   
			 * @see 
			**/
			int loadIVar(const bsl::var::IVar & );
			/**
			 * @brief 用约束文件检查一次
			 * @param [in] range   : const char* 约束文件
			 * @param [in] version   : int 文件格式的版本（暂未使用）
			 * @return  int  0表示成功，其余为失败
			 * 				bsl::CONSTRAINT_ERROR  约束检查错误
			 * 				bsl::ERROR 其它错误
			 **/
			int check_once(const char * range, int version = CONFIG_VERSION_1_0);

			virtual  ConfigUnit & operator= (ConfigUnit & unit) {
				return ConfigUnit::operator=(unit);
			}

			Configure();
			~Configure();
		protected:

			int load_1_0(const char * path, const char * conf, const char *range);
			void pushSubReader(const char * conf, int level = -1);
			friend class Reader;
			friend class Constraint;
			friend class ConfigGroup;
			friend class ConfigUnit;

			struct ReaderNode{//每一个conf文件
				str_t filename;
				Reader * reader;
				int level;
			};
			std::vector <ReaderNode *> _readers;
			str_t _path;
			Reader * _cur_reader;//当前在处理的文件
			int _cur_level;//当前文件的深度($include的层次)
			char * _dump_buffer;
			size_t _dump_size;
			confIDL::idl_t * _idl;
			void changeSection(str_t str);
			void pushPair(const str_t& key, const str_t& value);

			ConfigGroup* _section;
			//depth是一个调试变量
			int _depth;
	};
}



#endif  //__CONFIGURE_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
