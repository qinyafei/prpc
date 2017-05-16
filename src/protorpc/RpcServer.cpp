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
* @brief    ע��rpc ����
* @param[in] service������ʵ��ָ��
*/
void RpcServer::registerService(::google::protobuf::Service* service)
{
	const google::protobuf::ServiceDescriptor* desc = service->GetDescriptor();
	services_[desc->name()] = service;
}

/**
* @brief    �½��ͻ�������
* @param[in] conn��client����
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
		//����rpcchannel
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
