
/**
*  @file  BlockQueue.h
*  @brief ��������
*  @version 0.0.1
*  @since 0.0.1
*  @author
*  @date 2016-7-25  Created it
*/
/******************************************************************************
*@note
Copyright 2007, BeiJing Bluestar Corporation, Limited
ALL RIGHTS RESERVED
Permission is hereby granted to licensees of BeiJing Bluestar, Inc. products
to use or abstract this computer program for the sole purpose of implementing
a product based on BeiJing Bluestar, Inc. products. No other rights to
reproduce, use, or disseminate this computer program,whether in part or  in
whole, are granted. BeiJing Bluestar, Inc. makes no representation or
warranties with respect to the performance of this computer program, and
specifically disclaims any responsibility for any damages, special or
consequential, connected with the use of this program.
For details, see http://www.bstar.com.cn/
******************************************************************************/
#ifndef _BLOCKQUEUE_H_
#define _BLOCKQUEUE_H_


#include <deque>
#include "MutexLock.h"
#include "noncopyable.h"
#include "Condition.h"

namespace vmr
{

namespace base
{


	/** @class BlockQueue
	*  @brief ���������࣬���ɿ������죬��ֵ
	*  ģ���࣬thread safe
	*/

template<typename T>
class BlockQueue : public noncopyable
{
public:
	BlockQueue()
	  :notEmpty_(mutex_)
  {}
  ~BlockQueue(){}

    ///�����������
	void put(T& val)
	{
		LockGuard lock(&mutex_);
		queue_.push_back(val);
		notEmpty_.notify();
	}

   ///ȡ����
	T get()
	{
		LockGuard lock(&mutex_);
		while (queue_.empty())
		{
			notEmpty_.wait();
		}


		T front(queue_.front());
		queue_.pop_front();

		return front;
	}

	///���г���
	int size()
	{
		LockGuard lock(&mutex_);
		return queue_.size();
	}

private:
	std::deque<T> queue_; ///< deque����
	MutexLock mutex_;   ///����
	Condition notEmpty_; ///֪ͨ

};

}

}//end of namespace base

#endif
