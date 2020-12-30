#include "head.h"

typedef list<SOCKET> ListCONN;
typedef list<SOCKET> ListConErr;

void printRequest(sockaddr_in clientAddr, char* recvBuf);

void main(int argc, char* argv[])
{
	WSADATA wsaData;
	int nRC;
	sockaddr_in srvAddr, clientAddr;
	SOCKET srvSock;
	int nAddrLen = sizeof(sockaddr);
	char sendBuf[BUFLEN], recvBuf[BUFLEN];
	ListCONN conList;		//����������Ч�ĻỰSOCKET
	ListCONN::iterator itor;
	ListConErr conErrList;	//��������ʧЧ�ĻỰSOCKET
	ListConErr::iterator itor1;
	FD_SET rfds, wfds;
	u_long uNonBlock;

	//��ʼ�� �����ļ�
	Util::loadConf((string &)string("conf"));

	//��ʼ�� winsock
	nRC = WSAStartup(0x0101, &wsaData);
	if (nRC)
	{
		verbose("Server initialize winsock error!\n");
		return;
	}
	if (wsaData.wVersion != 0x0101)
	{
		verbose("Server's winsock version error!\n");
		WSACleanup();
		return;
	}
	verbose("Server's winsock initialized !\n");

	//���� TCP socket
	srvSock = socket(AF_INET, SOCK_STREAM, 0);
	if (srvSock == INVALID_SOCKET)
	{
		verbose("Server create socket error!\n");
		WSACleanup();
		return;
	}
	verbose("Server TCP socket create OK!\n");

	//�� socket to Server's IP and port 5050
	srvAddr.sin_family = AF_INET;
	srvAddr.sin_port = htons(Util::port);
	inet_pton(AF_INET, (Util::address).c_str(), &(srvAddr.sin_addr.S_un.S_addr));;
	nRC = bind(srvSock, (LPSOCKADDR)&srvAddr, sizeof(srvAddr));
	if (nRC == SOCKET_ERROR)
	{
		verbose("Server socket bind error!\n");
		closesocket(srvSock);
		WSACleanup();
		return;
	}
	verbose("Server socket bind OK!\n");

	//��ʼ�������̣��ȴ��ͻ�������
	nRC = listen(srvSock, 50);
	if (nRC == SOCKET_ERROR)
	{
		verbose("Server socket listen error!\n");
		closesocket(srvSock);
		WSACleanup();
		return;
	}

	//��srvSock��Ϊ������ģʽ�Լ����ͻ���������
	uNonBlock = 1;
	ioctlsocket(srvSock, FIONBIO, &uNonBlock);

	while (1)
	{
		//��conList��ɾ���Ѿ���������ĻỰSOCKET
		for (itor1 = conErrList.begin(); itor1 != conErrList.end(); itor1++)
		{
			itor = find(conList.begin(), conList.end(), *itor1);
			if (itor != conList.end()) {
				conList.erase(itor);
			}
		}

		//���read,write�׽��ּ���
		FD_ZERO(&rfds);
		FD_ZERO(&wfds);

		//���õȴ��ͻ���������
		FD_SET(srvSock, &rfds);

		for (itor = conList.begin(); itor != conList.end(); itor++)
		{
			//�����лỰSOCKET��Ϊ������ģʽ
			uNonBlock = 1;
			ioctlsocket(*itor, FIONBIO, &uNonBlock);
			//���õȴ��ỰSOKCET�ɽ������ݻ�ɷ�������
			FD_SET(*itor, &rfds);
			FD_SET(*itor, &wfds);
		}
		//��ʼ�ȴ�
		int nTotal = select(0, &rfds, &wfds, NULL, NULL);

		//���srvSock�յ��������󣬽��ܿͻ���������
		if (FD_ISSET(srvSock, &rfds))
		{
			nTotal--;
			//�����ỰSOCKET
			SOCKET connSock = accept(srvSock,
				(LPSOCKADDR)&clientAddr,
				&nAddrLen);
			if (connSock == INVALID_SOCKET)
			{
				verbose("Server accept connection request error!\n");
				closesocket(srvSock);
				WSACleanup();
				return;
			}

			//�������ĻỰSOCKET������conList��
			conList.insert(conList.end(), connSock);
		}
		if (nTotal > 0)
		{
			//���������Ч�ĻỰSOCKET�Ƿ������ݵ���
			for (itor = conList.begin(); itor != conList.end(); itor++)
			{
				//����ỰSOCKET�����ݵ���������ܿͻ�������
				if (FD_ISSET(*itor, &rfds))
				{
					nRC = recv(*itor, recvBuf, BUFLEN, 0);
					recvBuf[nRC] = '\0';
					//printf(recvBuf);
					if (nRC == SOCKET_ERROR)
					{
						//�������ݴ���
						//��¼�²�������ĻỰSOCKET
						conErrList.insert(conErrList.end(), *itor);
					}
					else if (nRC == 0) {
						
					}
					else
					{
						Util::printRequest(clientAddr,recvBuf);
						string filePath = Util::extractRequestPath(recvBuf);
						filePath = Util::composeRequestFilePath(filePath);
						Util::chunkTransfer(*itor, filePath);
						
						//Util::streamTransfer(*itor, filePath);
						//conErrList.insert(conErrList.end(), *itor);
					}
				}
			}
		}
	}
	closesocket(srvSock);
	WSACleanup();
}