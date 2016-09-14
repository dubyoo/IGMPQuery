#ifndef __INSERT_PACKET_PDU_MESSAGE_H__
#define __INSERT_PACKET_PDU_MESSAGE_H__

#include "ace/Refcounted_Auto_Ptr.h"
#include "ace/Recursive_Thread_Mutex.h"
#include "BBNTypes.h"
#include "INSPDUMsgPayload.h"

//////////////////////////////////////////////////////////////////////////
//
// CINSPDUMsgHeader
//
//////////////////////////////////////////////////////////////////////////
class CINSPDUMsgHeader : public INSPDUBufferBase
{
public:
	enum { INS_PDU_Msg_Header_Size = 12 };

	CINSPDUMsgHeader(ULONG SGID);
	virtual ~CINSPDUMsgHeader();

	void SetPayloadLength(BYTE payloadLength);

private:
	ULONG  mSGID;
	USHORT mPayloadLength;
};
typedef ACE_Refcounted_Auto_Ptr<CINSPDUMsgHeader, ACE_Recursive_Thread_Mutex> CINSPDUMsgHeaderRcPtr;




//////////////////////////////////////////////////////////////////////////
//
// CINSPDUMessage
//
//////////////////////////////////////////////////////////////////////////
class CINSPDUMessage : public INSPDUBufferBase
{
public:
	CINSPDUMessage(ULONG SGID);
	virtual ~CINSPDUMessage();

private:
	CINSPDUMsgHeaderRcPtr  mRcpINSPDUMsgHeader;
	CINSPDUMsgPayloadRcPtr mRcpINSPDUMsgPayload;
};
typedef ACE_Refcounted_Auto_Ptr<CINSPDUMessage, ACE_Recursive_Thread_Mutex> CINSPDUMessageRcPtr;



#endif
