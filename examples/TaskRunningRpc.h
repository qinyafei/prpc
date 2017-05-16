/**
*  @file  JobResultClient.h
*  @brief  ����ַ�������ִ�н��̼�ͨѶrpc����
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
#ifndef _TASKRUNNINGRPC_H_
#define _TASKRUNNINGRPC_H_


#include "RpcServer.h"
#include "CustomRpcChannel.h"
#include "RpcCodec.h"
#include "RpcClient.h"

#include "jobdispatch.pb.h"

using namespace vmr::base;
using namespace vmr::protorpc;


namespace vmr
{

	/** @class ImplJobDispatch
	*  @brief  ����ַ�������ִ�н��̵ķ����rpc���̳���JobDispatchService
	*   ���ɿ�������͸�ֵ
	*
	*   ���jobdispatch.proto �ļ�����
	*/
	class ImplJobDispatch : public JobDispatch::JobDispatchService
	{
	public:
		virtual void add(::google::protobuf::RpcController* controller,
			const ::JobDispatch::TaskRequest* request,
			::JobDispatch::TaskResponse* response,
			::google::protobuf::Closure* done);
		virtual void del(::google::protobuf::RpcController* controller,
			const ::JobDispatch::TaskRequest* request,
			::JobDispatch::TaskResponse* response,
			::google::protobuf::Closure* done);
		virtual void edit(::google::protobuf::RpcController* controller,
			const ::JobDispatch::TaskRequest* request,
			::JobDispatch::TaskResponse* response,
			::google::protobuf::Closure* done);

	public:
		void setPerformContext(void* context)
		{
			performContext_ = context;
		}

	private:
		void* performContext_;
	};

}//namespace 

#endif


