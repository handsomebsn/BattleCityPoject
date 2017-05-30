#pragma once
// Net.cpp : 定义控制台应用程序的入口点。
//
#ifndef __SOCKET__
//#include "winsock2.h"  
#include <iostream>  
#pragma comment(lib, "ws2_32.lib")  
#define BUF_SIZE  1024
#define MOVING 30//对应发送消息的function，表示移动
using namespace std;
char TargetIP[64];
char TargetIP2[64];

//服务器端
SOCKET          sServer;        //服务器套接字  
SOCKET          sClient;        //客户端套接字 
int             retVal_server;         //返回值  
//客户端
SOCKET sHost; //服务器套接字  
int retVal_client; //返回值  
typedef struct send_info
{
	int length; //发送的消息主体的长度
	//char face[255]; //消息主体
	int face; //朝向
	int x;//坐标
	int y;
	bool fire;
}send_info;
send_info Player_B;
int Start_Client()
{

	WSADATA wsd; //WSADATA变量  
	int iMode = 1;
	SOCKADDR_IN servAddr; //服务器地址  

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
	servAddr.sin_addr.s_addr = inet_addr(TargetIP);
	servAddr.sin_port = htons((short)4999);
	int nServAddlen = sizeof(servAddr);
	//调用ioctlsocket（）将其设置为非阻塞模式
	retVal_client = ioctlsocket(sHost, FIONBIO, (u_long FAR*)&iMode);
	if (retVal_client == SOCKET_ERROR)
	{
		//printf("ioctlsocket failed!");
		WSACleanup();
		return -1;
	}
	while (1) {
		//连接服务器ip1  
		retVal_client = connect(sHost, (LPSOCKADDR)&servAddr, sizeof(servAddr));
		if (SOCKET_ERROR == retVal_client)
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
				//printf("connection failed!\n");
				closesocket(sHost);
				WSACleanup();
				return -1;
			}
		}
		//连接服务器ip2
		servAddr.sin_addr.s_addr = inet_addr(TargetIP2);
		retVal_client = connect(sHost, (LPSOCKADDR)&servAddr, sizeof(servAddr));
		if (SOCKET_ERROR == retVal_client)
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
				//printf("connection failed!\n");
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
	//初始化套结字动态库  
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		MessageBoxA(NULL, "WSAStartup failed!", "", MB_OK);
		return 1;
	}

	//创建套接字  
	sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sServer)
	{
		MessageBoxA(NULL, "socket failed!", "", MB_OK);
		WSACleanup();//释放套接字资源;  
		return  -1;
	}

	//服务器套接字地址   
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(4999);
	addrServ.sin_addr.s_addr = INADDR_ANY;
	//绑定套接字  
	retVal_server = bind(sServer, (LPSOCKADDR)&addrServ, sizeof(SOCKADDR_IN));
	if (SOCKET_ERROR == retVal_server)
	{
		MessageBoxA(NULL, "bind failed!", "", MB_OK);
		closesocket(sServer);   //关闭套接字  
		WSACleanup();           //释放套接字资源;  
		return -1;
	}

	//开始监听   
	retVal_server = listen(sServer, 1);
	if (SOCKET_ERROR == retVal_server)
	{
		MessageBoxA(NULL, "listen failed!", "", MB_OK);
		closesocket(sServer);   //关闭套接字  
		WSACleanup();           //释放套接字资源;  
		return -1;
	}
	// 设置Socket为非阻塞模式
	int iMode = 1;
	retVal_server = ioctlsocket(sServer, FIONBIO, (u_long FAR*) &iMode);
	if (retVal_server == SOCKET_ERROR)
	{
		//printf("ioctlsocket failed!\n");
		WSACleanup();
		return -1;
	}
	//接受客户端请求  
	sockaddr_in addrClient;
	int addrClientlen = sizeof(addrClient);
	MessageBoxA(NULL, "等待玩家加入", "", MB_OK);
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
			else {
				//printf("accept failed!\n");
				closesocket(sServer);
				WSACleanup();
				return -1;
			}
		}
		MessageBoxA(NULL, "有玩家加入", "", MB_OK); break;
	}
	return 0;
}
#define nothing -1
//服务器接收客户端数据  
void Receive_Server()
{
	char recv_buf[BUF_SIZE];
	send_info info; //(1)定义结构体变量		
	memset(recv_buf, nothing, BUF_SIZE);//清空缓存
	memset(&info, nothing, sizeof(info));//清空结构体
	retVal_server = recv(sClient, recv_buf, BUF_SIZE, 0);//(2)读取数据
	memcpy(&info, recv_buf, sizeof(info));//(3)把接收到的信息转换成结构体
	//ZeroMemory(recv_buf, BUF_SIZE);	
	if (SOCKET_ERROR == retVal_server)
	{
		int err = WSAGetLastError();
		if (err == WSAEWOULDBLOCK)
		{
			Sleep(5);
			//continue;
		}
		else if (err == WSAETIMEDOUT || err == WSAENETDOWN)
		{
			//printf("recv failed!\n");
			closesocket(sServer);
			closesocket(sClient);
			WSACleanup();
			//return -1;
		}
	}
	else {
		switch (info.face) //判断接收内容并输出
		{
		case DOWN:Player_B.face = DOWN; break;
		case UP:Player_B.face = UP; break;
		case LEFT:Player_B.face = LEFT; break;
		case RIGHT:Player_B.face = RIGHT; break;
		}
		if (info.fire == true) Player_B.fire = true;
		if (info.fire == false)Player_B.fire = false;		
	}
}
//客户端接收服务器数据  
void Receive_Client()
{
	char recv_buf[BUF_SIZE];
	send_info info; //(1)定义结构体变量		
	memset(recv_buf, nothing, BUF_SIZE);//清空缓存
	memset(&info, nothing, sizeof(info));//清空结构体
	retVal_client=recv(sHost, recv_buf, BUF_SIZE, 0);//(2)读取数据	
	memcpy(&info, recv_buf, sizeof(info));//(3)把接收到的信息转换成结构体
	if (SOCKET_ERROR == retVal_client)
	{
		int err = WSAGetLastError();
		if (err == WSAEWOULDBLOCK)
		{
			Sleep(5);
			//continue;
		}
		else if (err == WSAETIMEDOUT || err == WSAENETDOWN)
		{
			//printf("recv failed!\n");
			closesocket(sHost);
			WSACleanup();
			//return -1;
		}
	}
	else {
		switch (info.face) //判断接收内容并输出
		{
		case DOWN:Player_B.face = DOWN; break;
		case UP:Player_B.face = UP; break;
		case LEFT:Player_B.face = LEFT; break;
		case RIGHT:Player_B.face = RIGHT; break;
		}
		if (info.fire == true) Player_B.fire = true;
		if (info.fire == false)Player_B.fire = false;
	}
}
//客户端发送给服务器
void Send_Client(int function,int operation)
{
	char send_buf[BUF_SIZE];
	memset(send_buf, nothing, BUF_SIZE);//清空缓存，不清空的话可能导致接收时产生乱码，
	send_info info; //(1)定义结构体变量
	memset(&info, nothing, sizeof(info));//清空结构体
	switch (function)
	{
	case MOVING:info.face = operation; break;
	}	
	memcpy(send_buf, &info, sizeof(info)); //(3)结构体转换成字符串
	send(sHost, send_buf, sizeof(send_buf), 0);//(4)发送信息	
}
//服务器发送给客户端
void Send_Server(int function,int operation)
{
	//发送数据
	char send_buf[BUF_SIZE];
	memset(send_buf, nothing, BUF_SIZE);//清空缓存，不清空的话可能导致接收时产生乱码，	
	send_info info; //(1)定义结构体变量	
	memset(&info, nothing, sizeof(info));//清空结构体
	switch (function)
	{
	case MOVING:info.face = operation; break;
	}
	memcpy(send_buf, &info, sizeof(info)); //(3)结构体转换成字符串
	send(sClient, send_buf, sizeof(send_buf), 0);//(4)发送信息	
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

