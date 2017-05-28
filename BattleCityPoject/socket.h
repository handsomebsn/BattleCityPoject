#pragma once
// Net.cpp : 定义控制台应用程序的入口点。
//
#ifndef _SOCKET_
//#include "winsock2.h"  
#include <iostream>  
#pragma comment(lib, "ws2_32.lib")  

using namespace std;
BOOL RecvLine(SOCKET s, char* buf); //读取一行数据  

int Start_Client()
{
	const int BUF_SIZE = 255;

	WSADATA wsd; //WSADATA变量  
	SOCKET sHost; //服务器套接字  
	SOCKADDR_IN servAddr; //服务器地址  
	//char buf[BUF_SIZE]; //接收数据缓冲区  
	char bufRecv[BUF_SIZE];
	int retVal; //返回值  
	//初始化套结字动态库  
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		MessageBoxA(NULL, "WSAStartup failed!", "", MB_OK);
		return -1;
	}
	//创建套接字  
	sHost = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sHost)
	{
		MessageBoxA(NULL, "socket failed!", "", MB_OK);
		WSACleanup();//释放套接字资源  
		return  -1;
	}

	//设置服务器地址  
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servAddr.sin_port = htons((short)4999);
	int nServAddlen = sizeof(servAddr);

	//连接服务器  
	retVal = connect(sHost, (LPSOCKADDR)&servAddr, sizeof(servAddr));
	if (SOCKET_ERROR == retVal)
	{
		MessageBoxA(NULL, "connect failed!", "", MB_OK);
		closesocket(sHost); //关闭套接字  
		WSACleanup(); //释放套接字资源  
		return -1;
	}
	else
	{
		MessageBoxA(NULL, "connect successfull!", "", MB_OK);
	}
	
		//向服务器发送数据  
		//ZeroMemory(buf, BUF_SIZE);
		char buf[BUF_SIZE] = "这里是客户端";
		retVal = send(sHost, buf, strlen(buf), 0);
	    /*if (SOCKET_ERROR == retVal)
		{
			cout << "send failed!" << endl;
			closesocket(sHost); //关闭套接字  
			WSACleanup(); //释放套接字资源  
			return -1;
		}*/
		//从服务器接收数据
		ZeroMemory(bufRecv, BUF_SIZE);		
		recv(sHost, bufRecv, BUF_SIZE, 0); // 接收服务器端的数据， 只接收5个字符
		MessageBoxA(NULL, bufRecv, "", MB_OK);		
	
	//退出  
	closesocket(sHost); //关闭套接字  
	WSACleanup(); //释放套接字资源  
}
int Start_Server() {
	const int BUF_SIZE = 255;
	WSADATA         wsd;            //WSADATA变量  
	SOCKET          sServer;        //服务器套接字  
	SOCKET          sClient;        //客户端套接字  
	SOCKADDR_IN     addrServ;;      //服务器地址  
	char            buf[BUF_SIZE];  //接收数据缓冲区  
//	char            sendBuf[BUF_SIZE];//返回给客户端得数据  
	int             retVal;         //返回值  
	//初始化套结字动态库  
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		MessageBoxA(NULL,  "WSAStartup failed!", "", MB_OK);		
		return 1;
	}

	//创建套接字  
	sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sServer)
	{
		MessageBoxA(NULL,  "socket failed!", "", MB_OK);	
		WSACleanup();//释放套接字资源;  
		return  -1;
	}

	//服务器套接字地址   
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(4999);
	addrServ.sin_addr.s_addr = INADDR_ANY;
	//绑定套接字  
	retVal = bind(sServer, (LPSOCKADDR)&addrServ, sizeof(SOCKADDR_IN));
	if (SOCKET_ERROR == retVal)
	{
		MessageBoxA(NULL,  "bind failed!", "", MB_OK);
		closesocket(sServer);   //关闭套接字  
		WSACleanup();           //释放套接字资源;  
		return -1;
	}

	//开始监听   
	retVal = listen(sServer, 1);
	if (SOCKET_ERROR == retVal)
	{
		MessageBoxA(NULL,  "listen failed!", "", MB_OK);
		closesocket(sServer);   //关闭套接字  
		WSACleanup();           //释放套接字资源;  
		return -1;
	}

	//接受客户端请求  
	sockaddr_in addrClient;
	int addrClientlen = sizeof(addrClient);
	MessageBoxA(NULL, "等待连接", "", MB_OK);
	sClient = accept(sServer, (sockaddr FAR*)&addrClient, &addrClientlen);
	if (INVALID_SOCKET == sClient)
	{
		MessageBoxA(NULL,  "accept failed!", "", MB_OK);
		closesocket(sServer);   //关闭套接字  
		WSACleanup();           //释放套接字资源;  
		return -1;
	}
	else
	{
		MessageBoxA(NULL, "接收到新连接", "", MB_OK);
	}

	
		//接收客户端数据  
		ZeroMemory(buf, BUF_SIZE);
		retVal = recv(sClient, buf, BUF_SIZE, 0);
		/*if (SOCKET_ERROR == retVal)
		{
			cout << "recv failed!" << endl;
			closesocket(sServer);   //关闭套接字  
			closesocket(sClient);   //关闭套接字       
			WSACleanup();           //释放套接字资源;  
			return -1;
		}*/
		MessageBoxA(NULL, buf, "", MB_OK);
		char sendBuf[BUF_SIZE] = "这里是服务器";
		send(sClient, sendBuf, strlen(sendBuf), 0);
	


	//退出  
	closesocket(sServer);   //关闭套接字  
	closesocket(sClient);   //关闭套接字  
	WSACleanup();           //释放套接字资源;  
	return 0;
}

#endif
