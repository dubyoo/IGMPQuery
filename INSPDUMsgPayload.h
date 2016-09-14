#ifndef _INS_PDU_MSG_PAYLOAD_H__
#define _INS_PDU_MSG_PAYLOAD_H__

#include "ace/Refcounted_Auto_Ptr.h"
#include "ace/Recursive_Thread_Mutex.h"
#include "BBNTypes.h"
#include "INSPDUMessageHelper.h"


//////////////////////////////////////////////////////////////////////////
// 
// CEthernetHeader
//
//////////////////////////////////////////////////////////////////////////
class CEthernetHeader : public INSPDUBufferBase
{
public:
	enum { INS_PDU_ETHERNET_Header_Size = 14 };

	CEthernetHeader();
	virtual ~CEthernetHeader();
};
typedef ACE_Refcounted_Auto_Ptr<CEthernetHeader, ACE_Recursive_Thread_Mutex> CEthernetHeaderRcPtr;


//////////////////////////////////////////////////////////////////////////
//
// CIPHeader
//
//////////////////////////////////////////////////////////////////////////
class CIPHeader : public INSPDUBufferBase
{
public:
	enum { INS_PDU_IP_Header_Size = 24 };

	CIPHeader();
	virtual ~CIPHeader();

private:
	int InitBuffer();
	int SetChecksum();

private:
	BYTE	mVersion;
	BYTE	mIHL;
	BYTE	mDiffSvcFld;
	USHORT	mTotalLength;
	USHORT	mIdentification;
	USHORT	mFlagsAndFragmentOffset;
	BYTE	mTTL;
	BYTE	mProtocal;
	USHORT	mHeaderChecksum;
	ULONG	mSourceIP;
	ULONG	mDestinationIP;
	ULONG	mOptions;
};
typedef ACE_Refcounted_Auto_Ptr<CIPHeader, ACE_Recursive_Thread_Mutex> CIPHeaderRcPtr;


//////////////////////////////////////////////////////////////////////////
//
// CIGMPQuery
//
//////////////////////////////////////////////////////////////////////////
class CIGMPQuery : public INSPDUBufferBase
{
public:
	enum { INS_PDU_IGMP_Query_Size = 8 };

	CIGMPQuery();
	virtual ~CIGMPQuery();

private:
	int InitBuffer();
	int SetChecksum();

private:
	BYTE	mType;
	BYTE	mMaxRespTime;
	USHORT	mHeaderChecksum;
	ULONG	mMaticastAddress;
};
typedef ACE_Refcounted_Auto_Ptr<CIGMPQuery, ACE_Recursive_Thread_Mutex> CIGMPQueryRcPtr;



//////////////////////////////////////////////////////////////////////////
//
// CINSPDUMsgPayload
//
//////////////////////////////////////////////////////////////////////////
class CINSPDUMsgPayload : public INSPDUBufferBase
{
public:
	CINSPDUMsgPayload();
	virtual ~CINSPDUMsgPayload();

private:
	int InitBuffer();
	int SetCRC32();

private:
	CEthernetHeaderRcPtr	mRcpEthernetHeader;
	CIPHeaderRcPtr			mRcpIPHeader;
	CIGMPQueryRcPtr			mRcpIGMPQuery;
	ULONG					mEthernetPadding;
	ULONG					mCRC32;
};
typedef ACE_Refcounted_Auto_Ptr<CINSPDUMsgPayload, ACE_Recursive_Thread_Mutex> CINSPDUMsgPayloadRcPtr;



#endif //_INS_PDU_MSG_PAYLOAD_H__

