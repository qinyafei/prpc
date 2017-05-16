#include "TaskRunningRpc.h"


using namespace vmr;


//--------------------------------------------
/**
* @brief   增加新任务rpc服务
*/
void ImplJobDispatch::add(::google::protobuf::RpcController* controller,
	const ::JobDispatch::TaskRequest* request,
	::JobDispatch::TaskResponse* response,
	::google::protobuf::Closure* done)
{
	const JobDispatch::TaskEntity& tsk = request->task();
	printf("***ImplJobDispatch::add:%s \n", tsk.name().c_str());
	//fortest
	if (response != NULL)
		response->set_response("test");

	if (done != NULL)
		done->Run();
}

/**
* @brief   删除任务rpc服务
*/
void ImplJobDispatch::del(::google::protobuf::RpcController* controller,
	const ::JobDispatch::TaskRequest* request,
	::JobDispatch::TaskResponse* response,
	::google::protobuf::Closure* done)
{

}

/**
* @brief   修改任务rpc服务
*/
void ImplJobDispatch::edit(::google::protobuf::RpcController* controller,
	const ::JobDispatch::TaskRequest* request,
	::JobDispatch::TaskResponse* response,
	::google::protobuf::Closure* done)
{
	//fortest
	response->set_response("test");
}
