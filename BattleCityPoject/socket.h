#pragma once
// Net.cpp : �������̨Ӧ�ó������ڵ㡣
//
#ifndef __SOCKET__
//#include "winsock2.h"  
#include <iostream>  
#pragma comment(lib, "ws2_32.lib")  
#define BUF_SIZE  1024
using namespace std;
char TargetIP[64];
char TargetIP2[64];

//��������
SOCKET          sServer;        //�������׽���  
SOCKET          sClient;        //�ͻ����׽��� 
int             retVal_server;         //����ֵ  
//�ͻ���
SOCKET sHost; //�������׽���  
typedef struct send_info
{
	int length; //���͵���Ϣ����ĳ���
	//char face[255]; //��Ϣ����
	int face; //����
	int x;//����
	int y;
}send_info;
send_info Player_B;
int Start_Client()
{

	WSADATA wsd; //WSADATA����  
	int iMode = 1;
	SOCKADDR_IN servAddr; //��������ַ  
	int retVal; //����ֵ  
	//��ʼ���׽��ֶ�̬��  
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		MessageBoxA(NULL, "WSAStartup failed!", "", MB_OK);
		return -1;
	}
	//�����׽���  
	sHost = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sHost)
	{
		MessageBoxA(NULL, "socket failed!", "", MB_OK);
		WSACleanup();//�ͷ��׽�����Դ  
		return  -1;
	}

	//���÷�������ַ  
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(TargetIP);
	servAddr.sin_port = htons((short)4999);
	int nServAddlen = sizeof(servAddr);
	//����ioctlsocket������������Ϊ������ģʽ
	retVal = ioctlsocket(sHost, FIONBIO, (u_long FAR*)&iMode);
	if (retVal == SOCKET_ERROR)
	{
		//printf("ioctlsocket failed!");
		WSACleanup();
		return -1;
	}
	while (1) {
		//���ӷ�����ip1  
		retVal = connect(sHost, (LPSOCKADDR)&servAddr, sizeof(servAddr));
		if (SOCKET_ERROR == retVal)
		{/*  ����ģʽ
			MessageBoxA(NULL, "connect failed!", "", MB_OK);
			closesocket(sHost); //�ر��׽���
			WSACleanup(); //�ͷ��׽�����Դ
			return -1;*/
			int err = WSAGetLastError();
			//�޷�������ɷ�����Socket�ϵĲ���
			if (err == WSAEWOULDBLOCK || err == WSAEINVAL)
			{
				Sleep(5); continue;
			}
			else if (err == WSAEISCONN)//�ѽ�������
			{
				MessageBoxA(NULL, "���뷿��", "", MB_OK); break;
			}
			else
			{
				//printf("connection failed!\n");
				closesocket(sHost);
				WSACleanup();
				return -1;
			}
		}
		//���ӷ�����ip2
		servAddr.sin_addr.s_addr = inet_addr(TargetIP2);
		retVal = connect(sHost, (LPSOCKADDR)&servAddr, sizeof(servAddr));
		if (SOCKET_ERROR == retVal)
		{/*  ����ģʽ
			MessageBoxA(NULL, "connect failed!", "", MB_OK);
			closesocket(sHost); //�ر��׽���
			WSACleanup(); //�ͷ��׽�����Դ
			return -1;*/
			int err = WSAGetLastError();
			//�޷�������ɷ�����Socket�ϵĲ���
			if (err == WSAEWOULDBLOCK || err == WSAEINVAL)
			{
				Sleep(5); continue;
			}
			else if (err == WSAEISCONN)//�ѽ�������
			{
				MessageBoxA(NULL, "���뷿��", "", MB_OK); break;
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
	WSADATA         wsd;            //WSADATA����  
	SOCKADDR_IN     addrServ;;      //��������ַ  
	//��ʼ���׽��ֶ�̬��  
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		MessageBoxA(NULL, "WSAStartup failed!", "", MB_OK);
		return 1;
	}

	//�����׽���  
	sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sServer)
	{
		MessageBoxA(NULL, "socket failed!", "", MB_OK);
		WSACleanup();//�ͷ��׽�����Դ;  
		return  -1;
	}

	//�������׽��ֵ�ַ   
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(4999);
	addrServ.sin_addr.s_addr = INADDR_ANY;
	//���׽���  
	retVal_server = bind(sServer, (LPSOCKADDR)&addrServ, sizeof(SOCKADDR_IN));
	if (SOCKET_ERROR == retVal_server)
	{
		MessageBoxA(NULL, "bind failed!", "", MB_OK);
		closesocket(sServer);   //�ر��׽���  
		WSACleanup();           //�ͷ��׽�����Դ;  
		return -1;
	}

	//��ʼ����   
	retVal_server = listen(sServer, 1);
	if (SOCKET_ERROR == retVal_server)
	{
		MessageBoxA(NULL, "listen failed!", "", MB_OK);
		closesocket(sServer);   //�ر��׽���  
		WSACleanup();           //�ͷ��׽�����Դ;  
		return -1;
	}
	// ����SocketΪ������ģʽ
	int iMode = 1;
	retVal_server = ioctlsocket(sServer, FIONBIO, (u_long FAR*) &iMode);
	if (retVal_server == SOCKET_ERROR)
	{
		//printf("ioctlsocket failed!\n");
		WSACleanup();
		return -1;
	}
	//���ܿͻ�������  
	sockaddr_in addrClient;
	int addrClientlen = sizeof(addrClient);
	MessageBoxA(NULL, "�ȴ���Ҽ���", "", MB_OK);
	while (true)
	{
		sClient = accept(sServer, (sockaddr FAR*)&addrClient, &addrClientlen);
		/*����ģʽ
		if (INVALID_SOCKET == sClient)
		{
			MessageBoxA(NULL, "accept failed!", "", MB_OK);
			closesocket(sServer);   //�ر��׽���
			WSACleanup();           //�ͷ��׽�����Դ;
			return -1;
		}*/
		//������ģʽ
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
		MessageBoxA(NULL, "����Ҽ���", "", MB_OK); break;
	}
	return 0;
}
#define nothing -1
//���տͻ�������  
void Receive_Server()
{
	char recv_buf[BUF_SIZE];
	send_info info; //(1)����ṹ�����		
	memset(recv_buf, nothing, BUF_SIZE);//��ջ���
	memset(&info, nothing, sizeof(info));//��սṹ��
	retVal_server = recv(sClient, recv_buf, BUF_SIZE, 0);//(2)��ȡ����
	memcpy(&info, recv_buf, sizeof(info));//(3)�ѽ��յ�����Ϣת���ɽṹ��
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
		switch (info.face) //�жϽ������ݲ����
		{
		case DOWN:Player_B.face = DOWN; break;
		case UP:Player_B.face = UP; break;
		case LEFT:Player_B.face = LEFT; break;
		case RIGHT:Player_B.face = RIGHT; break;
		}
	}
}
//���տͻ�������  
void Receive_Client()
{
	char recv_buf[BUF_SIZE];
	send_info info; //(1)����ṹ�����		
	memset(recv_buf, nothing, BUF_SIZE);//��ջ���
	recv(sClient, recv_buf, BUF_SIZE, 0);//(2)��ȡ����
	memset(&info, nothing, sizeof(info));//��սṹ��
	memcpy(&info, recv_buf, sizeof(info));//(3)�ѽ��յ�����Ϣת���ɽṹ��
	switch (info.face) //�жϽ������ݲ����
	{
	case DOWN:Player_B.face = DOWN; break;
	case UP:Player_B.face = UP; break;
	case LEFT:Player_B.face = LEFT; break;
	case RIGHT:Player_B.face = RIGHT; break;
	}
}
void Send_Client(int facing, int xx, int yy)
{
	//��������
	char send_buf[BUF_SIZE];
	memset(send_buf, nothing, BUF_SIZE);//��ջ��棬����յĻ����ܵ��½���ʱ�������룬
	send_info info; //(1)����ṹ�����
	memset(&info, nothing, sizeof(info));//��սṹ��
	info.face = facing; info.x = xx; info.y = yy;
	memcpy(send_buf, &info, sizeof(info)); //(3)�ṹ��ת�����ַ���
	send(sHost, send_buf, sizeof(send_buf), 0);//(4)������Ϣ	
}
void Send_Server()
{
	//��������
	char send_buf[BUF_SIZE];
	send_info info; //(1)����ṹ�����
	memset(&info, nothing, sizeof(info));//��սṹ��
	memset(send_buf, nothing, BUF_SIZE);//��ջ��棬����յĻ����ܵ��½���ʱ�������룬
	memcpy(send_buf, &info, sizeof(info)); //(3)�ṹ��ת�����ַ���
	send(sHost, send_buf, sizeof(send_buf), 0);//(4)������Ϣ	
}
#endif
/*if (SOCKET_ERROR == retVal)
{
	cout << "send failed!" << endl;
	closesocket(sHost); //�ر��׽���
	WSACleanup(); //�ͷ��׽�����Դ
	return -1;
}*/
//	info.face[info.length] = '\0';
//��Ϣ���ݽ�����û�����Ļ������ܵ�����Ϣ���������쳣

