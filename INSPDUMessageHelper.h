#ifndef __INS_PDU_MESSAGE_HELPER_H__
#define __INS_PDU_MESSAGE_HELPER_H__

#include "BBNTypes.h"
#include "ace/Singleton.h"
#include "ace/Null_Mutex.h"

//////////////////////////////////////////////////////////////////////////
// 
// INSPDUBufferBase
// 
//////////////////////////////////////////////////////////////////////////
class INSPDUBufferBase
{
public:
	INSPDUBufferBase();
	virtual ~INSPDUBufferBase();

	virtual int SetBufferSize(ULONG size);
	virtual BYTE * GetBuffer();
	virtual ULONG GetLength();
	virtual int Print();  // for debug

protected:
	BYTE * mpBuffer;
	ULONG  mLength;
};


//////////////////////////////////////////////////////////////////////////
//
// CINSPDUMessageHelper
//
//////////////////////////////////////////////////////////////////////////
class CINSPDUMessageHelper
{
public:
	static int SetBitValueByULONG(ULONG val, BYTE* ptr);
	static int SetBitValueByUSHORT(USHORT val, BYTE* ptr);
	static ULONG Checksum(BYTE* pBuffer, unsigned int size);
};


#endif //__INS_PDU_MESSAGE_HELPER_H__
