
#include "RpcCodec.h"

#include <assert.h>
#include <string>
#include "SocketOpts.h"
#include "GlogWrapper.h"

using namespace vmr::protorpc;


RpcCodec::RpcCodec(const ProtobufMsgCallback& msgcall)
  :prototype_(&RpcMessage::default_instance()),
	msgCallbackFunc_(msgcall)
{
}


RpcCodec::~RpcCodec()
{
}

/**
* @brief    发送rpc请求
* @param[in] conn：rpc底层的客户端连接(tcp,unisock,udp)
* @param[in] msg：rpc消息
* @return
* @retval 0：successful
*/
int RpcCodec::sendMsg(const NetConnection* conn, ::google::protobuf::Message* msg)
{
  if (conn == NULL)
    {
      return -1;
    }

	Buffer buf;
	decodeFromMsg(msg, &buf);
        
	//
	std::string msgdata = buf.readAll();
	DLOG(INFO) << "sendMsg size=" << msgdata.length() << " sendMsg=" << msgdata.c_str();
	const_cast<NetConnection*>(conn)->send(msgdata.data(), msgdata.length());

	return 0;
}

/**
* @brief    从buffer负载，打包成Message
* @param[in] buf：消息体内容
* @param[in] len：消息体长度
* @param[out] msg：打包后的消息
* @return
* @retval 0：successful
*/
int RpcCodec::parseFromBuffer(const char* buf, int len, google::protobuf::Message* msg)
{
	if (msg != NULL)
	{
		bool bret = msg->ParseFromArray(buf, len);
		RpcMessage* rmsg = dynamic_cast<RpcMessage*>(msg);
		//rmsg->set_request(std::string(buf, len));
		DLOG(INFO) << "parseFromBuffer: " << buf << "len=" <<rmsg->request().length();
		return static_cast<int>(bret);
	}

	return 0;
}

/**
* @brief    Message解包到Buffer
* @param[in] msg：消息体
* @param[out] buf：解包后的内存值
* @return
* @retval 0：successful
*/
int RpcCodec::serializeToBuffer(const google::protobuf::Message* msg, Buffer* buf)
{
	assert(msg != NULL && buf != NULL);
	int bytesize = msg->ByteSize();
	buf->ensureWritableBytes(bytesize);

	uint8_t* begin = reinterpret_cast<uint8_t*>(buf->peekWrite());
	uint8_t* end = msg->SerializeWithCachedSizesToArray(begin);
	assert(end - begin == bytesize);

	buf->hasWritten(bytesize);

	return bytesize;
}

/**
* @brief    Rpc Message打包
*/
int RpcCodec::encodeToMsg(const char* buf, int len, ::google::protobuf::Message* msg)
{
	int taglen = strlen(csTag);
	if (memcmp(buf, csTag, taglen) == 0)
	{
		const char* payload = buf + taglen;
		int payloadLen = len - taglen;

		if (parseFromBuffer(payload, payloadLen, msg) == 1)
		{
			return 0;
		}
		else
		{
			LOG(ERROR) << "RpcCodec::encodeToMsg parseFromBuffer failed";
		}
	}
	else
	{
		LOG(ERROR) << "invalid protocol data";
	}

	return -1;
}

/**
* @brief    Rpc Message解包
*/
void RpcCodec::decodeFromMsg(::google::protobuf::Message* msg, Buffer* buf)
{
	assert(msg != NULL && buf != NULL);
	//add header tag
	buf->write(csTag, strlen(csTag));

	int bytesize = serializeToBuffer(msg, buf);
	int payloadLen = buf->readableBytes();
	DLOG(INFO) << "RpcCodec::decodeFromMsg size=" << bytesize << "payloadlen=" << payloadLen << " buf=" << buf->peekRead();
	uint32_t netlen = sockets::hostToNetwork32(payloadLen);
	buf->prepend(reinterpret_cast<const char*>(&netlen), sizeof(netlen));

	uint32_t testlen = sockets::networkToHost32(netlen);
	DLOG(INFO) << "##netlen:" << netlen << " ori:" << testlen << " all:" << buf->readableBytes();
}


/**
* @brief    收到网络数据包，解包回调上层应用
*/
void RpcCodec::codecMessage(const NetConnection* conn, Buffer* buf)
{
	assert(buf != NULL);

	while (buf->readableBytes() >= (sizeof(int) + strlen(csTag)))
	{
		unsigned int len = 0;
		//memcpy(&len, buf->read(sizeof(int)).data(), sizeof(int));
		len = *(unsigned int*)(buf->read(sizeof(int)).data());
		len = sockets::networkToHost32(len);
		if (len > csMaxMsgLen || len < sizeof(int) + strlen(csTag))
		{
			LOG(ERROR) << "RpcCodec::codecMessage, invalid rpc message";
			break;
		}
		else if (buf->readableBytes() >= len)
		{
			LOG(INFO) << "RpcCodec::codecMessage:len=" << len << "msg=" << buf->peekRead();
			google::protobuf::Message* msg(prototype_->New());
			int err = encodeToMsg(buf->peekRead(), len, msg);
			if (err == 0)
			{
				msgCallbackFunc_(conn, msg);
				buf->retrieve(len);
			}
			else
			{
				assert(0);
			}
		}
		else
		{
			break;
		}
	}

}
