#pragma once
// Net.cpp : 定义控制台应用程序的入口点。
//
#ifndef __SOCKET__
//#include "winsock2.h"  
#include <iostream>  
#pragma comment(lib, "ws2_32.lib")  
#define BUF_SIZE  1024
using namespace std;
typedef struct send_info
{
	int length; //发送的消息主体的长度
	char face[255]; //消息主体
}send_info;

int Start_Client()
{
	WSADATA wsd; //WSADATA变量  
	SOCKET sHost; //服务器套接字  
	SOCKADDR_IN servAddr; //服务器地址  
	char recv_buf[BUF_SIZE];
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
	else{
		MessageBoxA(NULL, "connect successfull!", "", MB_OK);
	}
		

		//发送数据
		char send_buf[BUF_SIZE] ;
		send_info info; //(1)定义结构体变量
		memset(&info, 0, sizeof(info));//清空结构体
		memcpy(info.face,"DOWN",sizeof("DOWN"));
		info.length = strlen(info.face);
		memset(send_buf, 0, BUF_SIZE);//清空缓存，不清空的话可能导致接收时产生乱码，
		memcpy(send_buf, &info, sizeof(info)); //(3)结构体转换成字符串
		retVal=send(sHost, send_buf, sizeof(send_buf), 0);//(4)发送信息	
		
	
	//退出  
	closesocket(sHost); //关闭套接字  
	WSACleanup(); //释放套接字资源  
	return 0;
}
int Start_Server() {

	WSADATA         wsd;            //WSADATA变量  
	SOCKET          sServer;        //服务器套接字  
	SOCKET          sClient;        //客户端套接字  
	SOCKADDR_IN     addrServ;;      //服务器地址  
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
		char recv_buf[BUF_SIZE] ;
		send_info info; //(1)定义结构体变量		
		memset(recv_buf, 0, BUF_SIZE);//清空缓存
		retVal = recv(sClient, recv_buf, BUF_SIZE, 0);//(2)读取数据
		memset(&info, 0, sizeof(info));//清空结构体
		memcpy(&info, recv_buf, sizeof(info));//(3)把接收到的信息转换成结构体
		info.face[info.length] = '\0';
		//消息内容结束，没有这句的话，可能导致消息乱码或输出异常
		if (info.face) //判断接收内容并输出
			MessageBoxA(NULL, info.face, "", MB_OK);
		//至此，结构体的发送与接收已经顺利结束了

	//退出  
	closesocket(sServer);   //关闭套接字  
	closesocket(sClient);   //关闭套接字  
	WSACleanup();           //释放套接字资源;  
	return 0;
}

#endif
	    /*if (SOCKET_ERROR == retVal)
		{
			cout << "send failed!" << endl;
			closesocket(sHost); //关闭套接字  
			WSACleanup(); //释放套接字资源  
			return -1;
		}*/