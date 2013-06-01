/////////////////////////////////////////////////////////////////////////////

#ifndef __UB_TIMER_H__
#define __UB_TIMER_H__
////////////////////////////////////////////////////////////////
//////// Module:  ub_timer, timer related module of general framework
///////  Comment: timer count is simple yet trouble
///////
////////////////////////////////////////////////////////////////


//一个线程中最大计数器的个数
#define MAX_TIMERR_COUNT   64
//计数器的名字最大长度
#define MAX_TIMERR_NAMELEN 32

#define TOTAL_TIMER_RES "ub_total"

//计时器的状态
enum ub_timer_status{
	UB_TIMER_START,		/**<开始计数*/
	UB_TIMER_END			/**<end status*/
};


//内部使用的时间struct
typedef struct _ub_timer_t{
	u_int			timer_cur;										/**< current count*/
	struct			timeval timevalue[MAX_TIMERR_COUNT];				/**< timerr info matrxi*/
	int				mstimer[MAX_TIMERR_COUNT];						/**< timerr info matrxi*/
	ub_timer_status	timerstatus[MAX_TIMERR_COUNT];					/**< timerr info matrxi*/
	char			timername[MAX_TIMERR_COUNT][MAX_TIMERR_NAMELEN];	/**< timername info matrxi*/
	char			formatedstr[MAX_TIMERR_COUNT * 20];				/**< formated string out*/
}ub_timer_t;


typedef struct _ub_timer_t	*pub_timer_t;

/**
 * @brief 初始化函数
 *     应该在线程启动之前运行,可以在程序最开始进行
 *
 * @return
 *      0 表示成功
 *		-1表示失败
**/
int ub_timer_init(pub_timer_t timer);


/**
 * @brief 程序结束时调用，用来关闭日志功能
 *
 * @param[in] ub_timer ub_timer_t core structure
 *
 */
int ub_timer_reset(pub_timer_t ub_timer);


/**
 * @brief 启动开始计时
 *
 * @param[in] ub_timer ub_timer_t core structure
 *
 * @param[in] task 计时的名称
 *
 * @param[in] endprev end previous timerr or not
 *
 * @return
 *	  length of truely added info
 */
int ub_timer_settask(pub_timer_t ub_timer, const char* task, int endprev = 1);


/**
 * @brief 计时end
 *
 * @param[in] ub_timer ub_timer_t core structure
 *
 * @return	number of chars been added
 */
int ub_timer_endtask(pub_timer_t ub_timer);


/**
 * @brief 取得计时信息
 *
 * @param[in] ub_timer ub_timer_t core structure
 *
 * @param[in] task 计时的名称
 *
 * @return
 *	string thing like 5ms etc.
 *
 * @comment for ub_timer_use
 */
int ub_timer_gettaskstring(pub_timer_t ub_timer, const char* task, char* dest, size_t dest_len);


/**
 * @brief 取得计时信息
 *
 * @param[in] ub_timer ub_timer_t core structure
 *
 * @param[in] task 计时的名称
 *
 * @return
 *	int timer in ms
 *
 * @comment for ub_timer_use
 */
int ub_timer_gettask(pub_timer_t ub_timer, const char* task);


/**
 * @brief 取得计时信息format output
 *
 * @param[in] ub_timer ub_timer_t core structure
 *
 * @return
 *	char timer in name:5ms name:6ms format
 *
 * @comment for ub_timer_use
 */
char* ub_timer_gettaskformat(pub_timer_t ub_timer);

#endif //__UB_TIMER_H__

/* vim: set ts=4 sw=4 tw=0 noet: */


