#include "ace/OS_NS_netdb.h"
#include "INSPDUMsgPayload.h"
//#include "SDBContainer.h"

//////////////////////////////////////////////////////////////////////////
// 
// CEthernetHeader
//
//////////////////////////////////////////////////////////////////////////
CEthernetHeader::CEthernetHeader()
{
	SetBufferSize(INS_PDU_ETHERNET_Header_Size);

	BBN_MAC_ADDR2 DA = { 0x01, 0x00, 0x5e, 0x00, 0x00, 0x01 };	// 01:00:5e:00:00:01
	ULONG index = 0;
	for (int i = 0; i < 6; i++)
		mpBuffer[index++] = DA.mVal[i];

	ACE_OS::macaddr_node_t SA;
	ACE_OS::getmacaddress(&SA);
	for (int i = 0; i < 6; i++)
		mpBuffer[index++] = SA.node[i];

	CINSPDUMessageHelper::SetBitValueByUSHORT(0x0800, mpBuffer + index);
}

CEthernetHeader::~CEthernetHeader()
{

}



//////////////////////////////////////////////////////////////////////////
//
// CIPHeader
//
//////////////////////////////////////////////////////////////////////////
CIPHeader::CIPHeader()
	: mVersion(0x04)	// IPv4
	, mIHL(6)			// 6 * 4 = 24 bytes header
	, mDiffSvcFld(0)
	, mTotalLength(32)
	, mIdentification(0)
	, mFlagsAndFragmentOffset(0)
	, mTTL(1)
	, mProtocal(2)
	, mHeaderChecksum(0)
	, mSourceIP(0/*SDB_VIRTUALIP_INSTANCE->GetVirtualIP()*/)
	, mDestinationIP(0xE0000001)	// 224.0.0.1
	, mOptions(0x94040000)
{
	InitBuffer();
	SetChecksum();
}

CIPHeader::~CIPHeader()
{
	
}

int CIPHeader::InitBuffer()
{
	SetBufferSize(INS_PDU_IP_Header_Size);

	ULONG index = 0;
	mpBuffer[index++] = ((mVersion << 4) & 0xf0) + (mIHL & 0x0f);
	mpBuffer[index++] = mDiffSvcFld;
	CINSPDUMessageHelper::SetBitValueByUSHORT(mTotalLength, mpBuffer + index);
	index += 2;
	CINSPDUMessageHelper::SetBitValueByUSHORT(mIdentification, mpBuffer + index);
	index += 2;
	CINSPDUMessageHelper::SetBitValueByUSHORT(mFlagsAndFragmentOffset, mpBuffer + index);
	index += 2;
	mpBuffer[index++] = mTTL;
	mpBuffer[index++] = mProtocal;

	index += 2;		// skip checksum

	CINSPDUMessageHelper::SetBitValueByULONG(mSourceIP, mpBuffer + index);
	index += 4;
	CINSPDUMessageHelper::SetBitValueByULONG(mDestinationIP, mpBuffer + index);
	index += 4;
	CINSPDUMessageHelper::SetBitValueByULONG(mOptions, mpBuffer + index);		// Router Alert option
	index += 4;
	return 0;
}

int CIPHeader::SetChecksum()
{
	mHeaderChecksum = CINSPDUMessageHelper::Checksum(mpBuffer, mLength);
	CINSPDUMessageHelper::SetBitValueByUSHORT(mHeaderChecksum, mpBuffer + 10);
	return 0;
}



//////////////////////////////////////////////////////////////////////////
//
// CIGMPQuery
//
//////////////////////////////////////////////////////////////////////////
CIGMPQuery::CIGMPQuery()
	: mType(0x11)
	, mMaxRespTime(10)
	, mHeaderChecksum(0)
	, mMaticastAddress(0x00000000)	// 0.0.0.0
{
	InitBuffer();
	SetChecksum();
}

CIGMPQuery::~CIGMPQuery()
{

}

int CIGMPQuery::InitBuffer()
{
	SetBufferSize(INS_PDU_IGMP_Query_Size);

	ULONG index = 0;
	mpBuffer[index++] = mType;
	mpBuffer[index++] = mMaxRespTime;

	index += 2;		// skip checksum

	CINSPDUMessageHelper::SetBitValueByULONG(mMaticastAddress, mpBuffer + index);
	index += 4;

	return 0;
}

int CIGMPQuery::SetChecksum()
{
	mHeaderChecksum = CINSPDUMessageHelper::Checksum(mpBuffer, mLength);
	CINSPDUMessageHelper::SetBitValueByUSHORT(mHeaderChecksum, mpBuffer + 2);
	return 0;
}


//////////////////////////////////////////////////////////////////////////
//
// CINSPDUMsgPayload
//
//////////////////////////////////////////////////////////////////////////
CINSPDUMsgPayload::CINSPDUMsgPayload()
	: mRcpEthernetHeader(NULL)
	, mRcpIPHeader(NULL)
	, mRcpIGMPQuery(NULL)
	, mEthernetPadding(0)
	, mCRC32(0)
{
	mRcpEthernetHeader = new CEthernetHeader();
	mRcpIPHeader = new CIPHeader();
	mRcpIGMPQuery = new CIGMPQuery();

	mRcpEthernetHeader->Print();
	mRcpIPHeader->Print();
	mRcpIGMPQuery->Print();

	InitBuffer();
	SetCRC32();
}

CINSPDUMsgPayload::~CINSPDUMsgPayload()
{

}

int CINSPDUMsgPayload::InitBuffer()
{
	ULONG EhernetHeaderLength = mRcpEthernetHeader->GetLength();
	ULONG IPHeaderLength = mRcpIPHeader->GetLength();
	ULONG IGMPQueryLength = mRcpIGMPQuery->GetLength();

	SetBufferSize(EhernetHeaderLength + IPHeaderLength + IGMPQueryLength + 8);

	memcpy(mpBuffer, mRcpEthernetHeader->GetBuffer(), EhernetHeaderLength);
	memcpy(mpBuffer + EhernetHeaderLength, mRcpIPHeader->GetBuffer(), IPHeaderLength);
	memcpy(mpBuffer + EhernetHeaderLength + IPHeaderLength, mRcpIGMPQuery->GetBuffer(), IGMPQueryLength);

	CINSPDUMessageHelper::SetBitValueByULONG(mEthernetPadding, mpBuffer + EhernetHeaderLength + IPHeaderLength + IGMPQueryLength);
	return 0;
}

int CINSPDUMsgPayload::SetCRC32()
{
//	mCRC32 = SpCrc32Cal::instance()->Crc32(mpBuffer, mLength - 4);
	mCRC32 = ACE::crc32(mpBuffer, mLength - 4);
	printf("mCRC32 = %lX\n", mCRC32);
	CINSPDUMessageHelper::SetBitValueByULONG(mCRC32, mpBuffer + mLength - 4);
	return 0;
}

