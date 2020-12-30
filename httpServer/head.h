#pragma once
// socksrv.cpp : Defines the entry point for the console application.
#include <stdio.h>
#include <winsock2.h>
#include <list>
#include <algorithm>
#include <string.h>
#include <ws2tcpip.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <errno.h>

#include "util.h"

#pragma warning(disable:4996);
#pragma comment(lib,"ws2_32.lib")		//º”‘ÿws2_32.dll

#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define verbose printf

using namespace std;