#ifndef __IGMP_QUERY_SENDER_H__
#define __IGMP_QUERY_SENDER_H__

#include "ace/Thread_Manager.h"
#include "ace/Synch.h"
#include "BBNTypes.h"
#include "ace/SOCK_Dgram.h"

class CIGMPQuerySender
{
public:
	CIGMPQuerySender();
	virtual ~CIGMPQuerySender();
	HRESULT SendIGMPQuery(ULONG SGID, ULONG TargetIP, ULONG TargetPort);

private:
	// Do not allow copy or assignment
	CIGMPQuerySender(const CIGMPQuerySender&);
	CIGMPQuerySender& operator = (const CIGMPQuerySender&);

private:
	ACE_SOCK_Dgram mSock;
};

typedef ACE_Singleton<CIGMPQuerySender, ACE_Null_Mutex> SpCIGMPQuerySender;
#define IGMP_QUERY_SENDER_INSTANCE	SpCIGMPQuerySender::instance()

#endif // __IGMP_QUERY_SENDER_H__


