#include "INSPDUMessageHelper.h"

//////////////////////////////////////////////////////////////////////////
// 
// INSPDUBufferBase
// 
//////////////////////////////////////////////////////////////////////////
INSPDUBufferBase::INSPDUBufferBase()
	: mpBuffer(NULL)
	, mLength(0)
{

}

INSPDUBufferBase::~INSPDUBufferBase()
{
	if (mpBuffer != NULL)
	{
		delete[] mpBuffer;
		mpBuffer = NULL;
		mLength = 0;
	}
}

int INSPDUBufferBase::SetBufferSize(ULONG size)
{
	mLength = size;
	if (mpBuffer != NULL)
	{
		delete[] mpBuffer;
		mpBuffer = NULL;
	}
	mpBuffer = new BYTE[mLength];
	memset(mpBuffer, 0, mLength);
	return 0;
}

BYTE * INSPDUBufferBase::GetBuffer()
{
	return mpBuffer;
}

ULONG INSPDUBufferBase::GetLength()
{
	return mLength;
}

int INSPDUBufferBase::Print()
{
	printf("%s(%d)\n", __FILE__, __LINE__);
	if (NULL == mpBuffer)
	{
		printf("[ERROR]: mpBuffer is NULL!\n");
	}
	for (ULONG i = 0; i < mLength; i++)
	{
		if (i % 4 == 0)
		{
			if (i > 0)
				printf("\n");
			printf("\t%04lX ", i);
		}
		printf(" %02X", mpBuffer[i]);
	}
	printf("\n");
	return 0;
}



//////////////////////////////////////////////////////////////////////////
//
// CINSPDUMessageHelper
//
//////////////////////////////////////////////////////////////////////////
int CINSPDUMessageHelper::SetBitValueByULONG(ULONG val, BYTE* ptr)
{
	for (int i = 0; i < 4; i++)
		ptr[i] = (BYTE)((val >> (8 * (3 - i))) & 0x000000ff);
	return 0;
}

int CINSPDUMessageHelper::SetBitValueByUSHORT(USHORT val, BYTE* ptr)
{
	ptr[0] = (BYTE)((val >> 8) & 0x00ff);
	ptr[1] = (BYTE)(val & 0x00ff);

	return 0;
}

ULONG CINSPDUMessageHelper::Checksum(BYTE* pBuffer, unsigned int size)
{
	ULONG cksum = 0;
	if ((NULL == pBuffer) || (0 == size))
		return cksum;

	char num = 0;
	BYTE *p = pBuffer;
	while (size > 1)
	{
		cksum += ((USHORT)p[num] << 8 & 0xff00) | (USHORT)p[num + 1] & 0x00FF;
		size -= 2;
		num += 2;
	}

	if (size > 0)
	{
		cksum += ((USHORT)p[num] << 8) & 0xFFFF;
		num += 1;
	}

	while (cksum >> 16)
	{
		cksum = (cksum & 0xFFFF) + (cksum >> 16);
	}

	return ~cksum;
}



