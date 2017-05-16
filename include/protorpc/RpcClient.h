#ifndef _RPCCLIENT_H_
#define _RPCCLIENT_H_

#include "Reactor.h"
#include "NetClient.h"
#include "CustomRpcChannel.h"
#include "NetConnection.h"

using namespace vmr::base;
using namespace vmr::net;

namespace vmr
{

	namespace protorpc
	{

		/** @class RpcClient
		*  @brief rpc �ͻ��˻��࣬����rpc������Ҫ�Ӵ˼̳�
		*/
		class RpcClient
		{
		public:
			RpcClient(Reactor* loop, const char* ip, short port);
			virtual ~RpcClient();

		public:
			Reactor* loop_;
			NetClient client_;
			CustomRpcChannel* channel_;
      bool connected_;
		public:
			///��������
			void connect()
			{
				client_.setConnCallback(std::bind(&RpcClient::connectionNew, this, std::placeholders::_1));
				client_.setMsgCallback(std::bind(&CustomRpcChannel::onNetReceiveData, channel_, std::placeholders::_1, std::placeholders::_2));
				client_.connect();
			}
			///���ӳɹ��ص�
			void connectionNew(const NetConnection* conn)
			{
				if (conn->isConnected())
				{
					channel_->setConnection(conn);
					connected_ = true;
				}
			}

			bool isConnected()
			{
				return connected_;
			}
			///���������麯���ӿ�
			virtual void sendRequest(){}

		};



	}
}


#endif
