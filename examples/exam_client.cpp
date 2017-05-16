

#include "RpcServer.h"
#include "CustomRpcChannel.h"
#include "RpcCodec.h"
#include "RpcClient.h"

#include "jobdispatch.pb.h"
#include "GlogWrapper.h"


#include "Thread.h"
#include "SchedulerRpc.h"

using namespace vmr;
using namespace vmr::base;
using namespace vmr::protorpc;
//--------------------------

void rpcClientProc(void* context)
{
	JobRpcClient* pclient = static_cast<JobRpcClient*>(context);
	if (pclient == NULL)
	{
		return;
	}

	while (!pclient->isConnected())
	{
		usleep(10000);
	}

	int count = 0; 
	while (count < 10)
	{
		pclient->sendRequest(1, 1);
		usleep(5000000);
		count++;
	}

}



int main(int argc, char** argv)
{
	Reactor loop;

	std::string servIp = "127.0.0.1";
	int servPort = 20100;

	JobRpcClient* client_ = new JobRpcClient(&loop, servIp.c_str(), servPort);
	if (!client_->isConnected())
	{
		client_->connect();
	}

	Thread th(std::bind(rpcClientProc, (void*)client_));
	th.start();

	loop.loop();

	return 0;
}