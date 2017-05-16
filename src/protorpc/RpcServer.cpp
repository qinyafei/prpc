#include "RpcServer.h"
#include <functional>

#include "GlogWrapper.h"

using namespace vmr::protorpc;

RpcServer::RpcServer(Reactor* loop, const char* ip, int port)
{
	server_ = new NetServer(loop, ip, port);
	server_->setConnCallback(std::bind(&RpcServer::connNew, this, std::placeholders::_1));
}


RpcServer::~RpcServer()
{
	services_.clear();

	delete server_;
	server_ = NULL;
}

/**
* @brief    注册rpc 方法
* @param[in] service：服务实例指针
*/
void RpcServer::registerService(::google::protobuf::Service* service)
{
	const google::protobuf::ServiceDescriptor* desc = service->GetDescriptor();
	services_[desc->name()] = service;
}

/**
* @brief    新建客户端连接
* @param[in] conn：client链接
*/
void RpcServer::connNew(NetConnection* conn)
{
	DLOG(INFO) << "##RpcServer::connNew";

	if (conn->isConnected())
	{
		CustomRpcChannel* channel = new CustomRpcChannel(conn);
		conn->setMsgCallback(std::bind(&CustomRpcChannel::onNetReceiveData, channel, std::placeholders::_1, std::placeholders::_2));
		channel->setServices(&services_);
		conn->setContext(static_cast<void*>(channel));
	}
	else
	{
		//connection has disconnected
		//销毁rpcchannel
		CustomRpcChannel* channel = (CustomRpcChannel*)(conn->getContext());
		delete channel;
		channel = NULL;
		
		LOG(WARNING) << "##RpcServer|connNew disconnect";
	}
}


void RpcServer::start()
{
  server_->start();
}
