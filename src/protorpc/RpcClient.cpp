#include "RpcClient.h"


using namespace vmr::protorpc;


RpcClient::RpcClient(Reactor* loop, const char* ip, short port)
  :client_(loop, ip, port),
   connected_(false)
{
	channel_ = new CustomRpcChannel();
}


RpcClient::~RpcClient()
{
}
