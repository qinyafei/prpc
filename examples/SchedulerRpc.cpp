#include "SchedulerRpc.h"

#include <string>

using namespace vmr;

/**
* @brief    分发任务rpc客户端发送请求
* @param[in] action：rpc请求类型
* @param[in] task：任务抽象实体
*/
int JobRpcClient::sendRequest(int action, int task)
{
	int ret = 0;

	JobDispatch::TaskRequest req;
	JobDispatch::TaskEntity* entity = req.mutable_task();
	if (entity == nullptr)
	{
		  return -1;
	}

	entity->set_id("id");
	entity->set_name("name");
	entity->set_tasktype("video");
	entity->set_timemode("everytime");
	entity->set_plantimer("plantimer");
	entity->set_args("args");
	entity->set_result("result");
	entity->set_remarks("remarks");


	JobDispatch::TaskResponse response;
	switch (action)
	{
	case 1:
	{
#if 1/*3000000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION*/
		stub_.add(NULL, &req, &response_, google::protobuf::internal::NewCallback(this, &JobRpcClient::reply, &response_));
#else
		stub_.add(NULL, &req, &response_, google::protobuf::NewCallback(this, &JobRpcClient::reply, &response_));
#endif
	}
	break;
	case 2:
		break;
	case 3:
#if 1/*3000000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION*/
		stub_.edit(NULL, &req, &response_, google::protobuf::internal::NewCallback(this, &JobRpcClient::reply, &response_));
#else
		stub_.edit(NULL, &req, &response_, google::protobuf::NewCallback(this, &JobRpcClient::reply, &response_));
#endif
		break;
	default:
		break;
	}

	printf(">>>JobRpcClient::sendRequest exit \n");

	return ret;
}

void JobRpcClient::reply(JobDispatch::TaskResponse* res)
{
	printf("***JobRpcClient|reply response \n");

	if (res != NULL)
	{
		std::string response = res->response();
		printf("***JobRpcClient|reply:%s \n", response.c_str());
	}
}
