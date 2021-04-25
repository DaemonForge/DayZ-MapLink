modded class DayZGame
{	
	void DayZGame() {
	}

	override void DeferredInit() {
		super.DeferredInit();
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
		if (mapname == "namalsk"){
			return Vector(8013.432129, 9.643253, 7619.701172);
		}
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