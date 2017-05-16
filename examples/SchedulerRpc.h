/**
*  @file  JobRpcClient.h
*  @brief  任务分发和任务执行进程间通讯rpc处理
*  @version 0.0.1
*  @since 0.0.1
*  @author
*  @date 2017-2-25  Created it
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
#ifndef _SCHEDULERRPC_H_
#define _SCHEDULERRPC_H_



#include "RpcServer.h"
#include "CustomRpcChannel.h"
#include "RpcCodec.h"
#include "RpcClient.h"

#include "jobdispatch.pb.h"


using namespace vmr::base;
using namespace vmr::protorpc;

namespace vmr
{

/** @class JobRpcClient
*  @brief rpc通信客户端，派发任务到任务执行进程
*/
class JobRpcClient : public RpcClient
{
 public:
	JobRpcClient(Reactor* loop, const char* ip, short port) :
		RpcClient(loop, ip, port),
		stub_(channel_)
	{

	}

public:
	int sendRequest(int action, int task);
  void reply(JobDispatch::TaskResponse* res);
private:
	JobDispatch::JobDispatchService::Stub stub_;
	JobDispatch::TaskResponse response_;
};



} //namespace 


#endif

