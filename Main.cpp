#include <iostream>
#include "ace/OS.h"
#include "IGMPQuerySender.h"

using namespace std;

int main(int argc, char **argv)
{
	cout << "[TRACE] Main.cpp:  init IGMPQuery" << endl;

	ULONG SGID = 801;
	ULONG TargetIP = 0x7F000001;	// 127.0.0.1
	ULONG TargetPort = 9999;

	if (argc >= 2)
		TargetPort = atoi(argv[1]);

	IGMP_QUERY_SENDER_INSTANCE->SendIGMPQuery(SGID, TargetIP, TargetPort);

// 	while (true)
// 		ACE_OS::sleep(10);

	return 0;
}

