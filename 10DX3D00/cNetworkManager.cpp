#include "stdafx.h"
#include "cNetworkManager.h"

unsigned int __stdcall ThreadFunc1(LPVOID p);

cNetworkManager::cNetworkManager()
	:m_funcRefreshRoom(NULL)
	, m_isSettingMine(false)
{
}


cNetworkManager::~cNetworkManager()
{
}

bool cNetworkManager::Setup()
{
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);

	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//家南 积己
	if (m_socket == -1)
	{
		return false;
	}

	SOCKADDR_IN servaddr = { 0 };//家南 林家
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(m_sServerIP.c_str());
	servaddr.sin_port = htons(PORT_NUM);

	int re = 0;
	re = connect(m_socket, (struct sockaddr *)&servaddr, sizeof(servaddr));//楷搬 夸没

	if (re == -1)
	{
		return false;
	}

	return true;
}

SOCKET & cNetworkManager::GetSocket()
{
	return m_socket;
}

void cNetworkManager::Destroy()
{
	if (m_isUsedNetwork)
	{
		ST_NETWORK stNet;
		stNet.eNetType = E_NETWORK_LOGOUT;
		stNet.sPlayerName = g_pGameManager->GetPlayerName();
		stNet.nNetID = m_nNetId;

		send(m_socket, (char*)&stNet, sizeof(stNet), 0);
	}

	Sleep(500);

	CloseHandle(m_hRecvThread);
	closesocket(m_socket);
	WSACleanup();
}

void cNetworkManager::SetNetId(int nNetId)
{
	m_nNetId = nNetId;
}

void cNetworkManager::SetisUsedNetwork(bool isUsedNetwork)
{
	m_isUsedNetwork = isUsedNetwork;
}

void cNetworkManager::SetServerIP(string sServerIP)
{
	m_sServerIP = sServerIP;
}

void cNetworkManager::SetisReady(bool isReady)
{
	m_stMine.isReady = isReady;
}

void cNetworkManager::RecvPoint()
{
}

void cNetworkManager::SetPlayerName(string name)
{
	
}

void cNetworkManager::AddNetworkPlayer(ST_NETWORK stNet)
{
	m_vecPlayer.push_back(stNet);
}

vector<ST_NETWORK>& cNetworkManager::GetNetworkPlayer()
{
	return m_vecPlayer;
}

void cNetworkManager::DeleteNetworkPlayer(int nNetID)
{
	vector<ST_NETWORK>::iterator iter = m_vecPlayer.begin();

	for (iter; iter != m_vecPlayer.end();)
	{
		if ((*iter).nNetID == nNetID)
			iter = m_vecPlayer.erase(iter);
		else
			iter++;
	}
}

void cNetworkManager::SetMyNetworkStruct(ST_NETWORK stNet)
{
}

int cNetworkManager::GetNetId()
{
	return m_nNetId;
}

string cNetworkManager::GetServerIP()
{
	return m_sServerIP;
}

string cNetworkManager::GetLogin()
{
	return m_sLogin;
}

ST_NETWORK & cNetworkManager::GetMine()
{
	return m_stMine;
}

bool cNetworkManager::GetisMineSetting()
{
	return m_isSettingMine;
}

bool cNetworkManager::GetisUsedNetwork()
{
	return m_isUsedNetwork;
}

void cNetworkManager::SetisMineSetting(bool isMine)
{
	m_isSettingMine = isMine;
}

unsigned int __stdcall ThreadFunc1(LPVOID p)
{
	cNetworkManager* g = (cNetworkManager*)p;
	g->RecvPoint();

	return 0;
}