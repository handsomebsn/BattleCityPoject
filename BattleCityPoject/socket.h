#pragma once
// Net.cpp : 定义控制台应用程序的入口点。
//
#ifndef __SOCKET__
//#include "winsock2.h"  
#include <iostream>  
#pragma comment(lib, "ws2_32.lib")  
#define BUF_SIZE  1024
using namespace std;
//服务器端
SOCKET          sServer;        //服务器套接字  
SOCKET          sClient;        //客户端套接字 
//客户端
SOCKET sHost; //服务器套接字  

typedef struct send_info
{
	int length; //发送的消息主体的长度
	//char face[255]; //消息主体
	int face; //朝向
	int x;//坐标
	int y;
}send_info;
send_info Player_B;
int Start_Client()
{

	WSADATA wsd; //WSADATA变量  
	int iMode=1;
	SOCKADDR_IN servAddr; //服务器地址  
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
	//调用ioctlsocket（）将其设置为非阻塞模式
	retVal = ioctlsocket(sHost, FIONBIO, (u_long FAR*)&iMode);
	if (retVal == SOCKET_ERROR)
	{
		printf("ioctlsocket failed!");
		WSACleanup();
		return -1;
	}
	while (1) {
		//连接服务器  
		retVal = connect(sHost, (LPSOCKADDR)&servAddr, sizeof(servAddr));
		if (SOCKET_ERROR == retVal)
		{/*  阻塞模式
			MessageBoxA(NULL, "connect failed!", "", MB_OK);
			closesocket(sHost); //关闭套接字
			WSACleanup(); //释放套接字资源
			return -1;*/
			int err = WSAGetLastError();
			//无法立即完成非阻塞Socket上的操作
			if (err == WSAEWOULDBLOCK || err == WSAEINVAL)
			{
				Sleep(5); continue;
			}
			else if (err == WSAEISCONN)//已建立连接
			{
				MessageBoxA(NULL, "进入房间", "", MB_OK); break;
			}
			else
			{
				printf("connection failed!\n");
				closesocket(sHost);
				WSACleanup();
				return -1;
			}
		}
	}
	return 0;
}
int Start_Server() {

	WSADATA         wsd;            //WSADATA变量  

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
	// 设置Socket为非阻塞模式
		int iMode = 1;
	retVal = ioctlsocket(sServer, FIONBIO, (u_long FAR*) &iMode);
	if (retVal == SOCKET_ERROR)
	{
		printf("ioctlsocket failed!\n");
		WSACleanup();
		return -1;
	}
	//接受客户端请求  
	sockaddr_in addrClient;
	int addrClientlen = sizeof(addrClient);
	MessageBoxA(NULL, "等待玩家加入ing", "", MB_OK);
	while (true)
	{
		sClient = accept(sServer, (sockaddr FAR*)&addrClient, &addrClientlen);
		/*阻塞模式
		if (INVALID_SOCKET == sClient)
		{
			MessageBoxA(NULL, "accept failed!", "", MB_OK);
			closesocket(sServer);   //关闭套接字  
			WSACleanup();           //释放套接字资源;  
			return -1;
		}*/
		//非阻塞模式
		if (INVALID_SOCKET == sClient)
		{
			int err = WSAGetLastError();
			if (err == WSAEWOULDBLOCK)
			{
				Sleep(5);
				continue;
			}
			else{
				printf("accept failed!\n");
				closesocket(sServer);
				WSACleanup();
				return -1;
			}
		}
		MessageBoxA(NULL, "有玩家加入", "", MB_OK);break;
	}
	return 0;
}

void Receive_Server()
{
	//接收客户端数据  
	char recv_buf[BUF_SIZE];
	send_info info; //(1)定义结构体变量		
	memset(recv_buf, 0, BUF_SIZE);//清空缓存
	recv(sClient, recv_buf, BUF_SIZE, 0);//(2)读取数据
	memset(&info, 0, sizeof(info));//清空结构体
	memcpy(&info, recv_buf, sizeof(info));//(3)把接收到的信息转换成结构体
	switch (info.face) //判断接收内容并输出
	{
	case DOWN:Player_B.face = DOWN; break;
	case UP:Player_B.face = UP; break;
	case LEFT:Player_B.face = LEFT; break;
	case RIGHT:Player_B.face = RIGHT; break;
	}
}
void Receive_Client()
{
	//接收客户端数据  
	char recv_buf[BUF_SIZE];
	send_info info; //(1)定义结构体变量		
	memset(recv_buf, 0, BUF_SIZE);//清空缓存
	recv(sClient, recv_buf, BUF_SIZE, 0);//(2)读取数据
	memset(&info, 0, sizeof(info));//清空结构体
	memcpy(&info, recv_buf, sizeof(info));//(3)把接收到的信息转换成结构体
	switch (info.face) //判断接收内容并输出
	{
	case DOWN:Player_B.face = DOWN; break;
	case UP:Player_B.face = UP; break;
	case LEFT:Player_B.face = LEFT; break;
	case RIGHT:Player_B.face = RIGHT; break;
	}
}
void Send_Client(int face)
{
	//发送数据
	char send_buf[BUF_SIZE];
	send_info info; //(1)定义结构体变量
	memset(&info, 0, sizeof(info));//清空结构体
	info.face = face;
	memset(send_buf, 0, BUF_SIZE);//清空缓存，不清空的话可能导致接收时产生乱码，
	memcpy(send_buf, &info, sizeof(info)); //(3)结构体转换成字符串
	send(sHost, send_buf, sizeof(send_buf), 0);//(4)发送信息	
}
void Send_Server()
{
	//发送数据
	char send_buf[BUF_SIZE];
	send_info info; //(1)定义结构体变量
	memset(&info, 0, sizeof(info));//清空结构体
	info.face = DOWN;
	memset(send_buf, 0, BUF_SIZE);//清空缓存，不清空的话可能导致接收时产生乱码，
	memcpy(send_buf, &info, sizeof(info)); //(3)结构体转换成字符串
	send(sHost, send_buf, sizeof(send_buf), 0);//(4)发送信息	
}
#endif
	    /*if (SOCKET_ERROR == retVal)
		{
			cout << "send failed!" << endl;
			closesocket(sHost); //关闭套接字  
			WSACleanup(); //释放套接字资源  
			return -1;
		}*/	
		//	info.face[info.length] = '\0';
		//消息内容结束，没有这句的话，可能导致消息乱码或输出异常