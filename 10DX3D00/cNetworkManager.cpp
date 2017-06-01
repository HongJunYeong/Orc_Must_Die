#include "stdafx.h"
#include "cNetworkManager.h"


cNetworkManager::cNetworkManager()
{
}


cNetworkManager::~cNetworkManager()
{
}

void cNetworkManager::Setup()
{
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);

	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//家南 积己
	if (m_socket == -1)
	{
		SendMessage(g_hWnd, WM_QUIT, 0, 0);
	}

	SOCKADDR_IN servaddr = { 0 };//家南 林家
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(m_sServerIP.c_str());
	servaddr.sin_port = htons(PORT_NUM);

	int re = 0;
	re = connect(m_socket, (struct sockaddr *)&servaddr, sizeof(servaddr));//楷搬 夸没

	if (re == -1)
	{
		SendMessage(g_hWnd, WM_DESTROY, 0, 0);
	}
}

SOCKET & cNetworkManager::GetSocket()
{
	return m_socket;
}

void cNetworkManager::Destroy()
{
	closesocket(m_socket);
	WSACleanup();
}

void cNetworkManager::SetNetId(int nNetId)
{
	m_nNetId = nNetId;
}

void cNetworkManager::SetServerIP(string sServerIP)
{
	m_sServerIP = sServerIP;
}

int cNetworkManager::GetNetId()
{
	return m_nNetId;
}

string cNetworkManager::GetServerIP()
{
	return m_sServerIP;
}
