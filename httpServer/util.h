#pragma once

#include <string>
#include "head.h"
#define MAXCONN 5
#define BUFLEN 2000

class Util
{
/*Global env*/
public:
	static std::string address;
	static uint16_t port;
	static std::string rootDir;

/*Funcs*/
public:
	/*���������ļ����ྲ̬��Ա*/
	static void loadConf(std::string &confFile);

	/*��http������������ļ���ַ*/
	static std::string extractRequestPath(char* recvBuf);

	/*�õ��������·��������rootDir�����·�����������ļ�����·��*/
	static std::string composeRequestFilePath(std::string &relPath);

	/**/
	static void streamTransfer(const SOCKET sock, const std::string& full_path);

	static void chunkTransfer(const SOCKET sock,const std::string& full_path);

	static void printRequest(sockaddr_in clientAddr, const char *recvBuf);
};