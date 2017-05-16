/**
*  @file  CustomController.h
*  @brief 基于protobuf自带rpc架构和网上的rpc框架
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
#ifndef _RPCCHANNEL_H_
#define _RPCCHANNEL_H_

#include <map>
#include <string>

#include "google/protobuf/service.h"
//#include "google/protobuf/stubs/casts.h"
#include "google/protobuf/stubs/common.h"
#include "google/protobuf/stubs/strutil.h"
#include "google/protobuf/stubs/substitute.h"

#include "rpcservice.pb.h"

#include "RpcCodec.h"
#include "MutexLock.h"
#include "NetConnection.h"

using namespace vmr::base;

namespace vmr
{

	namespace protorpc
	{

		/** @class CustomController
		*  @brief rpc控制器，继承自protobuf自带google::protobuf::RpcController
		*/
		class CustomController : public google::protobuf::RpcController
		{
		public:
			void Reset()
			{
				printf("Reset() not expected during this test.\n");
			}

			bool Failed() const
			{
				printf("Failed() not expected during this test.\n");
				return false;
			}

			std::string ErrorText() const
			{
				printf("ErrorText() not expected during this test.\n");
				return "";
			}

			void StartCancel()
			{
				printf("StartCancel() not expected during this test.\n");
			}

			void SetFailed(const std::string& reason)
			{
				printf("SetFailed() not expected during this test.\n");
			}

			bool IsCanceled() const
			{
				printf("IsCanceled() not expected during this test.\n");
				return false;
			}

			void NotifyOnCancel(google::protobuf::Closure* callback)
			{
				printf("NotifyOnCancel() not expected during this test.\n");
			}

		};





		typedef std::map<std::string, ::google::protobuf::Service*> ServiceMap;

		/** @class CustomRpcChannel
		*  @brief Rpc服务接收，处理，回复通道
		*    继承自protobuf自带google::protobuf::RpcChannel
		*/
		class CustomRpcChannel : public google::protobuf::RpcChannel
		{
		public:
			CustomRpcChannel();
			CustomRpcChannel(const NetConnection* conn);
			~CustomRpcChannel();

			///rpc服务response
			struct OutReplyCall
			{
				google::protobuf::Message* response;
				google::protobuf::Closure* doneCall;
			};

			typedef std::map<int64_t, OutReplyCall> ReplyCalls;
		private:
			ServiceMap* services_; ///<注册过的rpc服务队列
			RpcCodec codec_;       ///<protobuf序列化数据编解码
			int64_t id_;            
			ReplyCalls responseCalls_;  ///<rpc response回复队列
			MutexLock mutex_;
			NetConnection* conn_;  ///<网络链接

		public:
			///rpc客户端接口调用，重写自RpcChannel
			virtual void CallMethod(const ::google::protobuf::MethodDescriptor* method,
				::google::protobuf::RpcController* controller,
				const ::google::protobuf::Message* request,
				::google::protobuf::Message* response,
				::google::protobuf::Closure* done);
			///rpc消息回调接口
			void onRpcMessage(const NetConnection* conn, google::protobuf::Message* rpcmsg);
			///网络数据回调接口
			void onNetReceiveData(const NetConnection* conn, Buffer* buf);
			///response 回调接口
			void doneResponseCallback(google::protobuf::Message* response, int64_t id);
			///设置registed service
			void setServices(ServiceMap* services)
			{
				services_ = services;
			}
			void setConnection(const NetConnection* conn)
			{
			  conn_ = const_cast<NetConnection*>(conn);
			}

		};

	}

} //namespace


#endif 
