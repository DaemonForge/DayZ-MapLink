class UApiServerData extends Managed {
	
	string Name;
	string Map;
	string IP;
	int Port = 2302;
	int QueryPort = 27016;
	string Password = "";
	string RespawnServer = "";
	
	void UApiServerData(string ip, int port = 2302, string password = ""){
		IP = ip;
		Port = port;
		Password = password;
	}
}
