/**
*  @file  RpcCodec.h
*  @brief �������ݽ��
*         protobuf���л��������л�Ϊrpcmessage
*  @version 0.0.1
*  @since 0.0.1
*  @author
*  @date 2016-7-25  Created it
*/
/******************************************************************************
*@note
Copyright 2007, BeiJing Bluestar Corporation, Limited
ALL RIGHTS RESERVED
Permission is hereby granted to licensees of BeiJing Bluestar, Inc. products
to use or abstract this computer program for the sole purpose of implementing
a product based on BeiJing Bluestar, Inc. products. No other rights to
reproduce, use, or disseminate this computer program,whether in part or  in
whole, are granted. BeiJing Bluestar, Inc. makes no representation or
warranties with respect to the performance of this computer program, and
specifically disclaims any responsibility for any damages, special or
consequential, connected with the use of this program.
For details, see http://www.bstar.com.cn/
******************************************************************************/
#ifndef _RPCCODEC_H_
#define _RPCCODEC_H_

#include <functional>
#include "google/protobuf/message.h"
#include "Buffer.h"

#include "rpc.pb.h"
#include "NetConnection.h"

using namespace vmr::base;
using namespace vmr::net;
using namespace protorpc;

// wire format
//
// Field     Length  Content
//
// size      4-byte  2+N+4
// "BS"      2-byte
// payload   N-byte
// checksum  4-byte  adler32 of "RPC0"+payload
//

namespace vmr
{

namespace protorpc
{

	static const char* csTag = "BS";
	static const int csMaxMsgLen = 1024 * 1024 * 4;

	typedef std::function<void(const NetConnection* conn, google::protobuf::Message*)> ProtobufMsgCallback;

	/** @class RpcCodec
	*  @brief �������ݽ��
	*         protobuf���л��������л�Ϊrpcmessage
	*/
class RpcCodec
{
public:
	RpcCodec(const ProtobufMsgCallback& msgcall);
	~RpcCodec();

private:
	const google::protobuf::Message* prototype_;
	ProtobufMsgCallback msgCallbackFunc_;

public:
	///�����ݽ��д��������
	int sendMsg(const NetConnection* conn, ::google::protobuf::Message* msg);
	int parseFromBuffer(const char*buf, int len, google::protobuf::Message* msg);
	int serializeToBuffer(const google::protobuf::Message* msg, Buffer* buf);
	///�������ݰ�������protobuf::message
	int encodeToMsg(const char* buf, int len, ::google::protobuf::Message* msg);
	///��proto::message������buffer
	void decodeFromMsg(::google::protobuf::Message* msg, Buffer* buf);
	///
	void codecMessage(const NetConnection* conn, Buffer* buf);

};


}

}//namespace

#endif
