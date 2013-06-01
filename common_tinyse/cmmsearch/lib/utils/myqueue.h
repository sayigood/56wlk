#ifndef _MYQUEUE_H
#define _MYQUEUE_H

namespace xspace {

template<typename T>
class MyQueue
{
private:
	int _capacity;	//总的容量
	int _size;		//队列当前容量
	int _front;		//头指针
	int _rear;		//尾指针
	T *_array;		//队列存储
public:
	MyQueue() {
		_array = 0;
		_capacity = _size = _front = _rear = 0;
	}
	~MyQueue() {
		delete [] _array;
	}
	/**
	 * @brief 返回队列支持的最大容量
	 *
	 * @return  int 队列支持的最大容量
	 * @retval   
	 * @see 
	 * @author xiaowei
	 * @date 2008/02/06 12:22:29
	**/
	int capacity() {
		return _capacity;
	}
	/**
	 * @brief 判断队列是否为空
	 *
	 * @return  bool 返回true就是空，false非空
	 * @retval   
	 * @see 
	 * @author xiaowei
	 * @date 2008/02/06 12:23:06
	**/
	bool empty() {
		return _size == 0;
	}
	/**
	 * @brief 返回队列是否已满
	 *
	 * @return  bool 返回true就是满，false非满
	 * @retval   
	 * @see 
	 * @author xiaowei
	 * @date 2008/02/06 12:23:26
	**/
	bool full() {
		return _size == _capacity;
	}
	/**
	 * @brief 清空队列的内容，但不回收空间
	 *
	 * @return  void 
	 * @retval   
	 * @see 
	 * @author xiaowei
	 * @date 2008/02/06 12:24:07
	**/
	void clear() {
		_size = 0;
		_front = 0;
		_rear = 1;
	}
	/**
	 * @brief 创建队列
	 *
	 * @param [in qcap   : int 队列支持的最大长度
	 * @return  int 成功返回0，其他失败
	 * @retval   
	 * @see 
	 * @author xiaowei
	 * @date 2008/02/06 12:24:32
	**/
	int create(int qcap) {
		//下的算法如果qcap小于2将无法正常工作
		if (qcap < 2) {
			return -1;
		}
		_capacity = qcap;
		clear();
		_array = new T[qcap];
		if (_array == 0) {
			return -1;
		}
		return 0;
	}

	int destroy() {
		_capacity = 0;
		delete [] _array;
		return 0;
	}
	
	int push_back(const T & val)  {
		if (full()) {
			return -1;
		}
		_array[_rear] = val;
		++ _size;
		if (++ _rear == _capacity) {
			_rear = 0;
		}
		return 0;
	}

	int push_front(const T &val) {
		if (full()) {
			return -1;
		}
		_array[_front] = val;
		++ _size;
		if (--_front < 0) {
			_front = _capacity-1;
		}
		return 0;
	}

	int top_back(T &val) {
		if (empty()) {
			return -1;
		}
		int idx = _rear - 1;
		if (idx < 0) {
			idx = _capacity - 1;
		}
		val = _array[idx];
		return 0;
	}
	
	int pop_back(T &val) {
		if (empty()) {
			return -1;
		}
		if (--_rear < 0) {
			_rear = _capacity-1;
		}
		val = _array[_rear];
		-- _size;
		return 0;
	}

	int top_front(T &val) {
		if (empty()) {
			return -1;
		}
		int idx = _front + 1;
		if (idx == _capacity) {
			idx = 0;
		}
		val = _array[idx];
		return 0;
	}

	int pop_front(T &val)  {
		if (empty()) {
			return -1;
		}
		++ _front;
		if (_front == _capacity) {
			_front = 0;
		}
		val = _array[_front];
		-- _size;
		return 0;
	}

	int pop_backs(T *val, int nums) {
		int cnt = 0;
		while (cnt < nums) {
			if (pop_back(val[cnt]) != 0) {
				return cnt;
			}
			++ cnt;
		}
		return cnt;
	}
};
}
#endif

