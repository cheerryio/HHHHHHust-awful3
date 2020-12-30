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
	ListCONN conList;		//保存所有有效的会话SOCKET
	ListCONN::iterator itor;
	ListConErr conErrList;	//保存所有失效的会话SOCKET
	ListConErr::iterator itor1;
	FD_SET rfds, wfds;
	u_long uNonBlock;

	//初始化 配置文件
	Util::loadConf((string &)string("conf"));

	//初始化 winsock
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

	//创建 TCP socket
	srvSock = socket(AF_INET, SOCK_STREAM, 0);
	if (srvSock == INVALID_SOCKET)
	{
		verbose("Server create socket error!\n");
		WSACleanup();
		return;
	}
	verbose("Server TCP socket create OK!\n");

	//绑定 socket to Server's IP and port 5050
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

	//开始监听过程，等待客户的连接
	nRC = listen(srvSock, 50);
	if (nRC == SOCKET_ERROR)
	{
		verbose("Server socket listen error!\n");
		closesocket(srvSock);
		WSACleanup();
		return;
	}

	//将srvSock设为非阻塞模式以监听客户连接请求
	uNonBlock = 1;
	ioctlsocket(srvSock, FIONBIO, &uNonBlock);

	while (1)
	{
		//从conList中删除已经产生错误的会话SOCKET
		for (itor1 = conErrList.begin(); itor1 != conErrList.end(); itor1++)
		{
			itor = find(conList.begin(), conList.end(), *itor1);
			if (itor != conList.end()) {
				conList.erase(itor);
			}
		}

		//清空read,write套接字集合
		FD_ZERO(&rfds);
		FD_ZERO(&wfds);

		//设置等待客户连接请求
		FD_SET(srvSock, &rfds);

		for (itor = conList.begin(); itor != conList.end(); itor++)
		{
			//把所有会话SOCKET设为非阻塞模式
			uNonBlock = 1;
			ioctlsocket(*itor, FIONBIO, &uNonBlock);
			//设置等待会话SOKCET可接受数据或可发送数据
			FD_SET(*itor, &rfds);
			FD_SET(*itor, &wfds);
		}
		//开始等待
		int nTotal = select(0, &rfds, &wfds, NULL, NULL);

		//如果srvSock收到连接请求，接受客户连接请求
		if (FD_ISSET(srvSock, &rfds))
		{
			nTotal--;
			//产生会话SOCKET
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

			//将产生的会话SOCKET保存在conList中
			conList.insert(conList.end(), connSock);
		}
		if (nTotal > 0)
		{
			//检查所有有效的会话SOCKET是否有数据到来
			for (itor = conList.begin(); itor != conList.end(); itor++)
			{
				//如果会话SOCKET有数据到来，则接受客户的数据
				if (FD_ISSET(*itor, &rfds))
				{
					nRC = recv(*itor, recvBuf, BUFLEN, 0);
					recvBuf[nRC] = '\0';
					//printf(recvBuf);
					if (nRC == SOCKET_ERROR)
					{
						//接受数据错误，
						//记录下产生错误的会话SOCKET
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