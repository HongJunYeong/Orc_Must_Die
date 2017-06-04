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

	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//소켓 생성
	if (m_socket == -1)
	{
		return false;
	}

	SOCKADDR_IN servaddr = { 0 };//소켓 주소
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(m_sServerIP.c_str());
	servaddr.sin_port = htons(PORT_NUM);

	int re = 0;
	re = connect(m_socket, (struct sockaddr *)&servaddr, sizeof(servaddr));//연결 요청

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

	Sleep(3000);

	closesocket(m_socket);
	WSACleanup();
	CloseHandle(m_hRecvThread);
}

void cNetworkManager::SetNetId(int nNetId)
{
	m_nNetId = nNetId;
}

void cNetworkManager::SetServerIP(string sServerIP)
{
	m_sServerIP = sServerIP;
}

void cNetworkManager::SetReady(bool isReady)
{
	m_stMine.isReady = isReady;
}

void cNetworkManager::RecvPoint()
{
	SOCKET sock = g_pNetworkManager->GetSocket();
	char szMsg[BUF_SIZE];

	while (true)
	{
		int n = 10;
		int nStrLen = recv(sock, szMsg, sizeof(szMsg) - 1, 0);

		szMsg[nStrLen] = 0;
		ST_NETWORK* stNet = (ST_NETWORK*)&szMsg;

		m_stMine.eNetType = stNet->eNetType;

		switch (m_stMine.eNetType)
		{
		case E_SET_MY_NETWORK_ID:
		{
			g_pNetworkManager->SetNetId(stNet->nNetID);

			ST_NETWORK stPlayer;
			stPlayer.eNetType = E_NONE;
			g_pNetworkManager->SetMyNetworkStruct(stPlayer);

			m_isSettingMine = true;
			m_isUsedNetwork = true;
			cout << "[네트워크 ID] : " << g_pNetworkManager->GetNetId();
		}
		break;
		case E_SET_OTHER_NETWORK_ID:
		{
			AddNetworkPlayer(*stNet);
		}
		break;
		case E_REFRESH_WAITING_ROOM:
		{
			m_funcRefreshRoom();
			//m_sLogin = stNet->sPlayerName + " 님이 입장 하셨습니다";
			//m_funcRogUpdate(stNet->sPlayerName + " 님이 입장하셨습니다.");
		}
		break;
		case E_READY:
			for (int i = 0; i < m_vecPlayer.size(); i++)
			{
				if (m_vecPlayer[i].nNetID == stNet->nNetID)
				{
					m_vecPlayer[i].isReady = true;
					break;
				}
			}
			m_funcRefreshRoom();
			cout << "[네트워크 ID] : "<< stNet->nNetID << " 준비완료" << endl;
			break;
		case E_NETWORK_LOGOUT:
		{
			vector<ST_NETWORK>::iterator iter = m_vecPlayer.begin();

			for (iter; iter != m_vecPlayer.end();)
			{
				if ((*iter).nNetID == stNet->nNetID)
					iter = m_vecPlayer.erase(iter);
				else
					iter++;
			}

			m_funcRefreshRoom();
		}
		break;
		default:
			break;
		}

		m_stMine.eNetType = E_NONE;
	}
	closesocket(sock);
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