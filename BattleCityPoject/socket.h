#pragma once
// Net.cpp : �������̨Ӧ�ó������ڵ㡣
//
#ifndef _SOCKET_
//#include "winsock2.h"  
#include <iostream>  
#pragma comment(lib, "ws2_32.lib")  

using namespace std;
BOOL RecvLine(SOCKET s, char* buf); //��ȡһ������  

int Start_Client()
{
	const int BUF_SIZE = 255;

	WSADATA wsd; //WSADATA����  
	SOCKET sHost; //�������׽���  
	SOCKADDR_IN servAddr; //��������ַ  
	//char buf[BUF_SIZE]; //�������ݻ�����  
	char bufRecv[BUF_SIZE];
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
	servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servAddr.sin_port = htons((short)4999);
	int nServAddlen = sizeof(servAddr);

	//���ӷ�����  
	retVal = connect(sHost, (LPSOCKADDR)&servAddr, sizeof(servAddr));
	if (SOCKET_ERROR == retVal)
	{
		MessageBoxA(NULL, "connect failed!", "", MB_OK);
		closesocket(sHost); //�ر��׽���  
		WSACleanup(); //�ͷ��׽�����Դ  
		return -1;
	}
	else
	{
		MessageBoxA(NULL, "connect successfull!", "", MB_OK);
	}
	
		//���������������  
		//ZeroMemory(buf, BUF_SIZE);
		char buf[BUF_SIZE] = "�����ǿͻ���";
		retVal = send(sHost, buf, strlen(buf), 0);
	    /*if (SOCKET_ERROR == retVal)
		{
			cout << "send failed!" << endl;
			closesocket(sHost); //�ر��׽���  
			WSACleanup(); //�ͷ��׽�����Դ  
			return -1;
		}*/
		//�ӷ�������������
		ZeroMemory(bufRecv, BUF_SIZE);		
		recv(sHost, bufRecv, BUF_SIZE, 0); // ���շ������˵����ݣ� ֻ����5���ַ�
		MessageBoxA(NULL, bufRecv, "", MB_OK);		
	
	//�˳�  
	closesocket(sHost); //�ر��׽���  
	WSACleanup(); //�ͷ��׽�����Դ  
}
int Start_Server() {
	const int BUF_SIZE = 255;
	WSADATA         wsd;            //WSADATA����  
	SOCKET          sServer;        //�������׽���  
	SOCKET          sClient;        //�ͻ����׽���  
	SOCKADDR_IN     addrServ;;      //��������ַ  
	char            buf[BUF_SIZE];  //�������ݻ�����  
//	char            sendBuf[BUF_SIZE];//���ظ��ͻ��˵�����  
	int             retVal;         //����ֵ  
	//��ʼ���׽��ֶ�̬��  
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		MessageBoxA(NULL,  "WSAStartup failed!", "", MB_OK);		
		return 1;
	}

	//�����׽���  
	sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sServer)
	{
		MessageBoxA(NULL,  "socket failed!", "", MB_OK);	
		WSACleanup();//�ͷ��׽�����Դ;  
		return  -1;
	}

	//�������׽��ֵ�ַ   
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(4999);
	addrServ.sin_addr.s_addr = INADDR_ANY;
	//���׽���  
	retVal = bind(sServer, (LPSOCKADDR)&addrServ, sizeof(SOCKADDR_IN));
	if (SOCKET_ERROR == retVal)
	{
		MessageBoxA(NULL,  "bind failed!", "", MB_OK);
		closesocket(sServer);   //�ر��׽���  
		WSACleanup();           //�ͷ��׽�����Դ;  
		return -1;
	}

	//��ʼ����   
	retVal = listen(sServer, 1);
	if (SOCKET_ERROR == retVal)
	{
		MessageBoxA(NULL,  "listen failed!", "", MB_OK);
		closesocket(sServer);   //�ر��׽���  
		WSACleanup();           //�ͷ��׽�����Դ;  
		return -1;
	}

	//���ܿͻ�������  
	sockaddr_in addrClient;
	int addrClientlen = sizeof(addrClient);
	MessageBoxA(NULL, "�ȴ�����", "", MB_OK);
	sClient = accept(sServer, (sockaddr FAR*)&addrClient, &addrClientlen);
	if (INVALID_SOCKET == sClient)
	{
		MessageBoxA(NULL,  "accept failed!", "", MB_OK);
		closesocket(sServer);   //�ر��׽���  
		WSACleanup();           //�ͷ��׽�����Դ;  
		return -1;
	}
	else
	{
		MessageBoxA(NULL, "���յ�������", "", MB_OK);
	}

	
		//���տͻ�������  
		ZeroMemory(buf, BUF_SIZE);
		retVal = recv(sClient, buf, BUF_SIZE, 0);
		/*if (SOCKET_ERROR == retVal)
		{
			cout << "recv failed!" << endl;
			closesocket(sServer);   //�ر��׽���  
			closesocket(sClient);   //�ر��׽���       
			WSACleanup();           //�ͷ��׽�����Դ;  
			return -1;
		}*/
		MessageBoxA(NULL, buf, "", MB_OK);
		char sendBuf[BUF_SIZE] = "�����Ƿ�����";
		send(sClient, sendBuf, strlen(sendBuf), 0);
	


	//�˳�  
	closesocket(sServer);   //�ر��׽���  
	closesocket(sClient);   //�ر��׽���  
	WSACleanup();           //�ͷ��׽�����Դ;  
	return 0;
}

#endif
