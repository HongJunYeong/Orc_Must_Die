#pragma once

#define g_pNetworkManager	 cNetworkManager::GetInstance()	

#define PORT_NUM    10200
#define BUF_SIZE	2048
#define SERVER_IP   "192.168.0.33"

class cNetworkManager
{
public:

public:
	SINGLETONE(cNetworkManager);

private:
	SOCKET m_socket;
	int m_nNetId;
	string m_sServerIP;
public:
	void Setup();
	SOCKET& GetSocket();
	void Destroy();

	//Get , Set
	int GetNetId();
	string GetServerIP();

	void SetNetId(int nNetId);
	void SetServerIP(string sServerIP);
};

