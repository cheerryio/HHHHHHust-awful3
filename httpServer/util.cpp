#include "util.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <errno.h>
#include <vector>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#include "head.h"

#define maxErrorMessage 94

#pragma warning(disable:4996)

namespace fs = boost::filesystem;
using namespace std;


std::string Util::address;
uint16_t Util::port;
std::string Util::rootDir;

void Util::loadConf(std::string &confFile) {
	ifstream infile(confFile);
	if (infile.fail()) {
		char errmsg[maxErrorMessage];
		strerror_s(errmsg, maxErrorMessage, errno);
		cout << "Error:" << errmsg << endl;
	}
	string line;
	std::getline(infile, Util::address);	/*Get listen address*/
	std::getline(infile, line);				/*Get port*/
	stringstream ss(line);
	if (!(ss >> Util::port))
		cout << "Error:config port" << endl;
	std::getline(infile, Util::rootDir);	/*Get root dir*/
	return;
}

string Util::extractRequestPath(char* recvBuf) {
	string recv(recvBuf);
	stringstream ss(recv);
	string path;
	std::getline(ss, path, ' ');
	path.clear();
	std::getline(ss, path, ' ');
	return path;
}

string Util::composeRequestFilePath(string &relPath) {
	if (relPath.length() == 1)
		relPath = string("/index.html");
	fs::path root(Util::rootDir);
	fs::path file(relPath);
	fs::path full= root / file;
	return (string &)(full.string());
}

void Util::streamTransfer(const SOCKET sock, const string& full_path)
{
	std::vector<char> buffer;
	FILE* file_stream = fopen(full_path.c_str(), "rb");
	int file_size;
	string response;
	if (file_stream != nullptr)
	{
		fseek(file_stream, 0, SEEK_END);
		long file_length = ftell(file_stream);
		rewind(file_stream);
		buffer.resize(file_length);
		file_size = fread(&buffer[0], 1, file_length, file_stream);
		string file_str(buffer.begin(), buffer.end());
		if (file_str.length() > 0) {
			response = (boost::format{
				"HTTP/1.1 200 OK\r\nConnection: keep-alive\r\nContent-Length: %1%\r\n\r\n%2%"
				} % file_size % file_str).str();
			//response = "HTTP/1.1 200 OK\r\nconnection: close\r\nContent-Length: " + to_string(file_size) + "\r\n\r\n" + file_str;
			cout << "respond to client ok" << endl;
		}
		else {
			cout << "respond to client error" << endl;
		}
		fclose(file_stream);
	}
	else {
		cout << "open file error" << endl;
		string responseBody = (boost::format{ "The requested URL %s was not found on this server" } % full_path).str();
		response = (boost::format{
			"HTTP/1.1 200 NOT FOUND\r\nConnection: keep-alive\r\nContent-Type: text/html\r\nContent-Length:%d\r\n\r\n%s"
			} % responseBody.size() % responseBody).str();
	}
	send(sock, response.c_str(), response.size(), 0);
	return;
}

void Util::chunkTransfer(const SOCKET sock,const string& full_path) {
	enum { LF = 10, CR = 13 };
	char buffer[256];
	char lenBuffer[100];
	int sendNum;
	FILE* file_stream = fopen(full_path.c_str(), "rb");
	if (file_stream != nullptr) {
		sendNum = sprintf(buffer, "HTTP/1.1 200 OK\r\nConnection: keep-alive\r\nTransfer-Encoding: chunked\r\n\r\n");
		send(sock, buffer, sendNum, 0);
		size_t readin;
		while (readin = fread(buffer, 1, sizeof(buffer), file_stream)) {
			int sendlen = sprintf(lenBuffer, "%x\r\n", readin);
			send(sock, lenBuffer, sendlen, 0);
			send(sock, buffer, readin, 0);
			send(sock, "\r\n", 2, 0);
		}
		send(sock, "0\r\n", 3, 0);
		send(sock, "\r\n", 2, 0);
		fclose(file_stream);
		cout << "respond to client ok" << endl;
	}
	else {
		cout << "open file error" << endl;
		string responseBody = (boost::format{ "The requested URL %s was not found on this server"} % full_path ).str();
		string response = (boost::format{
			"HTTP/1.1 404 NOT FOUND\r\nConnection: keep-alive\r\nContent-Type: text/html\r\nContent-Length:%d\r\n\r\n%s"
			} % responseBody.size() % responseBody).str();
		send(sock, response.c_str(), response.size(), 0);
	}
	return;
}

void Util::printRequest(sockaddr_in clientAddr, const char* recvBuf) {
	//Êä³öclientAddrÄÚÈÝ
	char* localIP = inet_ntoa(clientAddr.sin_addr);
	cout << "remote address:" << localIP << ":" << clientAddr.sin_port << endl;
	cout << "remote request:" << endl << recvBuf << endl;
	return;
}