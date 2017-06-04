#pragma once

#define g_pNetworkManager	 cNetworkManager::GetInstance()	

#define PORT_NUM    10200
#define BUF_SIZE	4096
#define SERVER_IP   "192.168.0.33"

class cNetworkManager
{
public:

public:
	SINGLETONE(cNetworkManager);

private:
	SOCKET	m_socket;
	int		m_nNetId;
	string	m_sServerIP;

	HANDLE	m_hRecvThread;

	string				m_sPlayerName;
	ST_NETWORK			m_stMine;
	vector<ST_NETWORK>	m_vecPlayer;

	bool				m_isSettingMine;

	string				m_sLogin;

	bool				m_isUsedNetwork;
public:
	bool Setup();
	SOCKET& GetSocket();
	void Destroy();

	void RecvPoint();

	void SetPlayerName(string name);
	void AddNetworkPlayer(ST_NETWORK stNet);
	vector<ST_NETWORK>& GetNetworkPlayer();
	void DeleteNetworkPlayer(int nNetID);
	void SetMyNetworkStruct(ST_NETWORK stNet);

	//Get , Set
	int GetNetId();
	string GetServerIP();
	string GetLogin();
	ST_NETWORK&	GetMine();
	bool GetisMineSetting();

	void SetisMineSetting(bool isMine);
	void SetNetId(int nNetId);
	void SetServerIP(string sServerIP);
	void SetReady(bool isReady);

public:
	function<void()>		m_funcRefreshRoom;
	function<void(string)>	m_funcRogUpdate;
};

