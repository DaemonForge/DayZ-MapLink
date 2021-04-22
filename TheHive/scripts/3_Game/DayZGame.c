modded class DayZGame
{
	ref map<string, map<string, vector>> m_TransferPoints = new map<string, map<string, vector>>;
	
	void DayZGame() {
	}

	override void DeferredInit() {
		super.DeferredInit();
		map<string, vector> chernoPoints = new map<string, vector>;
		map<string, vector> enochPoints = new map<string, vector>;
		chernoPoints.Insert("first", Vector(250.9, 0, 9192.55));
		enochPoints.Insert("first", Vector(12626, 0, 8241));
		m_TransferPoints.Insert("chernarusplus", chernoPoints);
		m_TransferPoints.Insert("enoch", enochPoints);
	}
	
	void HiveReconnectTo(UApiServerData data){
		GetGame().Connect(NULL, data.IP, data.Port, data.Password);
		
	}
	
	vector HiveGetSpawnLocation(string mapname, string transferPoint){
		if (mapname == "enoch"){
			return Vector(12648.6, 251.106, 8221.34);
		}
		if (mapname == "chernarusplus"){
			return Vector(230.25, 296.54, 9119.84);
		}
		/*
		if (m_TransferPoints.Contains(mapname)){
			vector pos = m_TransferPoints.Get(mapname).Get(transferPoint);
			return pos;
		}*/
		return vector.Zero;
	}
	
};

class UApiServerData {
	
	string IP;
	int Port = 2302;
	string Password = "";
	
	void UApiServerData(string ip, int port = 2302, string password = ""){
		IP = ip;
		Port = port;
		Password = password;
	}
}