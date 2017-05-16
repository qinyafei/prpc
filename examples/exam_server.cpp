
#include "RpcServer.h"
#include "CustomRpcChannel.h"
#include "RpcCodec.h"
#include "RpcClient.h"

#include "jobdispatch.pb.h"
#include "GlogWrapper.h"

#include "TaskRunningRpc.h"


using namespace vmr::base;
using namespace vmr::protorpc;
using namespace vmr;


int main(int argc, char** argv)
{
	Reactor loop;

	//接收定时任务服务
	ImplJobDispatch* jobDispatchService_ = new ImplJobDispatch();

	std::string servIp = "127.0.0.1";
	int servPort = 20100;
	RpcServer* rpcServer_ = new RpcServer(&loop, servIp.c_str(), servPort);
	rpcServer_->registerService(jobDispatchService_);
	rpcServer_->start();

	loop.loop();


	return 0;
}