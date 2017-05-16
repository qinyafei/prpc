/**
*  @file  RpcServer.h
*  @brief rpc�������࣬rpc����ע��
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
#ifndef _RPCSERVER_H_
#define _RPCSERVER_H_


#include "google/protobuf/service.h"
#include "CustomRpcChannel.h"
#include <functional>
#include <vector>
#include "NetServer.h"
#include "Reactor.h"

using namespace vmr::net;
using namespace vmr::base;

namespace vmr
{

namespace protorpc
{

  typedef std::function<void(const google::protobuf::Message*)> RpcDoneCallback;

  /** @class RpcServer
  *  @brief rpc����ע�ᣬ�����������
  */
class RpcServer
{
public:
  RpcServer(Reactor* loop, const char* ip, int port);
	~RpcServer();

	//private:
public:
	ServiceMap services_; ///<����ע�����
	std::vector<google::protobuf::RpcChannel*> channels_;
    NetServer* server_;   ///<���������     
public:
	void registerService(::google::protobuf::Service* service);
	void start();
	///�½��������ӻص�
    void connNew(NetConnection* conn);
};

}

}


#endif
