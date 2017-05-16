
#include "CustomRpcChannel.h"

#include "rpc.pb.h"
#include "google/protobuf/descriptor.h"
#include "GlogWrapper.h"

using namespace protorpc;
using namespace vmr::protorpc;

CustomRpcChannel::CustomRpcChannel()
  :codec_(std::bind(&CustomRpcChannel::onRpcMessage, this, std::placeholders::_1, std::placeholders::_2)),
   services_(NULL),
   conn_(NULL)
{
	id_ = 1;
}


CustomRpcChannel::CustomRpcChannel(const NetConnection* conn)
  :codec_(std::bind(&CustomRpcChannel::onRpcMessage, this, std::placeholders::_1, std::placeholders::_2)),
   services_(NULL)
{
  conn_ = const_cast<NetConnection*>(conn);
}


CustomRpcChannel::~CustomRpcChannel()
{
}


/**
* @brief    �麯�����̳���RpcChannel,rpc���ͻص�����
* @param[in] method:rpc����
* @param[out] request��rpc��Ϣ����
*/
void CustomRpcChannel::CallMethod(const ::google::protobuf::MethodDescriptor* method,
	::google::protobuf::RpcController* controller,
	const ::google::protobuf::Message* request,
	::google::protobuf::Message* response,
	::google::protobuf::Closure* done)
{
  if (conn_ == nullptr) 
 {
  return;
 }

  RpcMessage msg;
  msg.set_type(REQUEST);
  //TODO:must lock for atomic
  int64_t id = id_++;
  msg.set_id(id);
  msg.set_service(method->service()->name());
  msg.set_method(method->name());

  //
  std::string payload = request->SerializeAsString();
  msg.set_request(payload);

  {
    OutReplyCall reply = {response, done};
    LockGuard lock(&mutex_);
    responseCalls_[id] = reply;
  }
  
  //DLOG(INFO) << "***CustomRpcChannel::client CallMethod:len=" << payload.length() <<",data:" << payload.c_str();
  //rpc��Ϣ����
  codec_.sendMsg(conn_, &msg);

  //for test
  //onRpcMessage(&msg);
}

/**
* @brief    rpc message�����������֮�󣬷�������ͻص����ϲ��������
* @param[in] conn:rpc�ײ���������
* @param[int] rpcmsg��rpc��Ϣ
*/
void CustomRpcChannel::onRpcMessage(const NetConnection* conn, google::protobuf::Message* rpcmsg)
{
	assert(rpcmsg != NULL);
	RpcMessage* msg = dynamic_cast<RpcMessage*>(rpcmsg);
	//RpcMessage* msg = (RpcMessage*)rpcmsg;
	//�յ��������Ӧ�ظ�
	if (msg->type() == RESPONSE)
	{
		LOG(INFO) << "CustomRpcChannel::onRpcMessage, msgType=RESPONSE";

		int64_t id = msg->id();
		assert(msg->has_response());

		OutReplyCall reply;
		{
			LockGuard lock(&mutex_);
			ReplyCalls::iterator it = responseCalls_.find(id);
			if (it != responseCalls_.end())
			{
				LOG(INFO) << "CustomRpcChannel::onRpcMessage, msgType=RESPONSE 2";
				reply = it->second;
				responseCalls_.erase(it);
			}
			else
			{
				LOG(ERROR) << "invalid response" << (int)id;
				return;
			}
		}

		if (reply.response != NULL)
		{
			if (msg->has_response())
			{
				LOG(INFO) << "CustomRpcChannel::onRpcMessage, msgType=RESPONSE 3";
				reply.response->ParseFromString(msg->response());
			}

			if (reply.doneCall != NULL)
			{
				LOG(INFO) << "CustomRpcChannel::onRpcMessage, msgType=RESPONSE 4";
				reply.doneCall->Run();
			}
		}
	}
	else if (msg->type() == REQUEST)
	{
		//�յ�����
		assert(services_ != NULL);
		ServiceMap::const_iterator it = services_->find(msg->service());
		if (it != services_->end())
		{
			google::protobuf::Service* service = it->second;
			assert(service != NULL);

			const google::protobuf::ServiceDescriptor* desc = service->GetDescriptor();
			const google::protobuf::MethodDescriptor* method = desc->FindMethodByName(msg->method());
			if (method)
			{
				google::protobuf::Message* request = service->GetRequestPrototype(method).New();
				assert(request != NULL);
				//request->CopyFrom(*msg);
				DLOG(INFO) << "CustomRpcChannel::onRpcMessage,request len=" << msg->request().length();
				if (request->ParseFromString(msg->request()))
				{
					DLOG(INFO) << "CustomRpcChannel::onRpcMessage,ParseFromString success";
					google::protobuf::Message* response = service->GetResponsePrototype(method).New();
					int64_t id = static_cast<int64_t>(msg->id());
					service->CallMethod(method, NULL, request, response,
						google::protobuf::internal::NewCallback(this, &CustomRpcChannel::doneResponseCallback,
						response, id));

				}
				else
				{
					assert(0);
				}
			}
			else
			{
				assert(0);
			}
		}
		else
		{
			assert(0);
		}

#if 0
		//response
		RpcMessage response;
		response.set_type(RESPONSE);
		response.set_id(msg->id());
		//response.set_error(error);
		codec_.sendMsg(conn, &response);
#endif
	}
	else
	{

	}
}

/**
* @brief    �������л�ȡrpc����
* @param[in] conn:Զ����������
* @param[out] buf�����ݻ���buffer
*/
void CustomRpcChannel::onNetReceiveData(const NetConnection* conn, Buffer* buf)
{
	DLOG(INFO) << "***CustomRpcChannel::onNetReceiveData readpos=" << buf->readPos_;
  //����rpc��Ϣ
  codec_.codecMessage(conn, buf);
}

/**
* @brief    �ظ�rpc����
* @param[in] response:�ظ�����Ϣ�ṹ��
* @param[in] id����Ϣ��sequence id
*/
void CustomRpcChannel::doneResponseCallback(google::protobuf::Message* response, int64_t id)
{
	DLOG(INFO) << "**CustomRpcChannel::doneResponseCallback";

	RpcMessage msg;
	msg.set_type(RESPONSE);
	msg.set_id(id);
	msg.set_response(response->SerializeAsString());
	codec_.sendMsg(conn_, &msg);

}
