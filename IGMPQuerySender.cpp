#include "IGMPQuerySender.h"
#include "INSPDUMessage.h"
#include "ace/INET_Addr.h"

CIGMPQuerySender::CIGMPQuerySender()
{
	ACE_INET_Addr local;
	mSock.open(local);
}

CIGMPQuerySender::~CIGMPQuerySender()
{
	mSock.close();
}

HRESULT CIGMPQuerySender::SendIGMPQuery(ULONG SGID, ULONG TargetIP, ULONG TargetPort)
{
	CINSPDUMessageRcPtr RcpINSPDUMsg= new CINSPDUMessage(SGID);
	RcpINSPDUMsg->Print();

	ACE_INET_Addr dest(TargetPort, TargetIP);
	int ret = mSock.send(RcpINSPDUMsg->GetBuffer(), RcpINSPDUMsg->GetLength(), dest);
	if (-1 == ret)
	{
		printf("[ERROR]: Failed to send IGMP Query\n");
	}
	printf("[DEBUG]: Send IGMP Query bytes %d\n", ret);

	return S_OK;
}

