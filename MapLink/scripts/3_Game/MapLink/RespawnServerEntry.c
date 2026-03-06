class RespawnServerEntry extends Managed
{
	string Name;
	string MapName;
	string IP;
	int Port;
	string Password;
	int QueryPort;
	bool IsCurrentServer;
	
	void RespawnServerEntry(string name, string mapName, string ip, int port, string password, int queryPort, bool isCurrent)
	{
		Name = name;
		MapName = mapName;
		IP = ip;
		Port = port;
		Password = password;
		QueryPort = queryPort;
		IsCurrentServer = isCurrent;
	}
}
