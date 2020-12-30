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
	/*加载配置文件至类静态成员*/
	static void loadConf(std::string &confFile);

	/*从http请求解析请求文件地址*/
	static std::string extractRequestPath(char* recvBuf);

	/*得到请求相对路径，根据rootDir与相对路径构造请求文件绝对路径*/
	static std::string composeRequestFilePath(std::string &relPath);

	/**/
	static void streamTransfer(const SOCKET sock, const std::string& full_path);

	static void chunkTransfer(const SOCKET sock,const std::string& full_path);

	static void printRequest(sockaddr_in clientAddr, const char *recvBuf);
};