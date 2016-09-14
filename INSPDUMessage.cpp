#include "INSPDUMessage.h"
#include "INSPDUMessageHelper.h"

//////////////////////////////////////////////////////////////////////////
//
// CINSPDUMessage
//
//////////////////////////////////////////////////////////////////////////
CINSPDUMessage::CINSPDUMessage(ULONG SGID)
{
	mRcpINSPDUMsgHeader  = new CINSPDUMsgHeader(SGID);
	mRcpINSPDUMsgPayload = new CINSPDUMsgPayload();
	mRcpINSPDUMsgHeader->SetPayloadLength((BYTE)mRcpINSPDUMsgPayload->GetLength());

	SetBufferSize(mRcpINSPDUMsgHeader->GetLength() + mRcpINSPDUMsgPayload->GetLength());
	memcpy(mpBuffer, mRcpINSPDUMsgHeader->GetBuffer(), mRcpINSPDUMsgHeader->GetLength());
	memcpy(mpBuffer + mRcpINSPDUMsgHeader->GetLength(), mRcpINSPDUMsgPayload->GetBuffer(), mRcpINSPDUMsgPayload->GetLength());
}

CINSPDUMessage::~CINSPDUMessage()
{

}



//////////////////////////////////////////////////////////////////////////
//
// CINSPDUMsgHeader
//
//////////////////////////////////////////////////////////////////////////
CINSPDUMsgHeader::CINSPDUMsgHeader(ULONG SGID)
	: mSGID(SGID)
	, mPayloadLength(0)
{
	SetBufferSize(INS_PDU_Msg_Header_Size);

	ULONG index = 0;
	mpBuffer[index++] = 0x82;	// INS PDU Type
	mpBuffer[index++] = 0x01;	// Version

	index += 2;					// skip Msg Length
	
	mpBuffer[index++] = 0x01;	// SGID Type
	mpBuffer[index++] = 0x04;	// SGID Length
	
	CINSPDUMessageHelper::SetBitValueByULONG(mSGID, mpBuffer + index);	// SGID
	index += 4;

	mpBuffer[index++] = 0x04;	// Packet PDU Type

}

CINSPDUMsgHeader::~CINSPDUMsgHeader()
{

}


void CINSPDUMsgHeader::SetPayloadLength(BYTE payloadLength)
{
	CINSPDUMessageHelper::SetBitValueByUSHORT(payloadLength + 8, mpBuffer + 2);
	mpBuffer[11] = (BYTE)payloadLength;
}

