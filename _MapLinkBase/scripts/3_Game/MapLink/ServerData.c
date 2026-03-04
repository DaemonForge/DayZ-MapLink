class UServerData extends Managed {
	
	string Name;
	string Map;
	string IP;
	int Port = 2302;
	int QueryPort = 27016;
	string Password = "";
	string RespawnServer = "";
	autoptr TStringArray ItemBlacklist = new TStringArray;
	
	void UServerData(string ip, int port = 2302, string password = ""){
		IP = ip;
		Port = port;
		Password = password;
	}
	
	bool IsItemBlacklisted(string itemType){
		if (!ItemBlacklist || ItemBlacklist.Count() == 0){
			return false;
		}
		string lowerType = itemType;
		lowerType.ToLower();
		for (int i = 0; i < ItemBlacklist.Count(); i++){
			string pattern = ItemBlacklist.Get(i);
			pattern.ToLower();
			if (pattern == ""){
				continue;
			}
			// Partial match: check if item type contains the pattern
			if (lowerType.Contains(pattern)){
				return true;
			}
			// Inheritance match: check if item inherits from the pattern class
			if (g_Game.IsKindOf(itemType, pattern)){
				return true;
			}
		}
		return false;
	}
	
	string FindFirstBlacklistedItem(Man player){
		if (!player || !ItemBlacklist || ItemBlacklist.Count() == 0){
			return "";
		}
		array<EntityAI> items = new array<EntityAI>;
		player.GetInventory().EnumerateInventory(InventoryTraversalType.LEVELORDER, items);
		for (int i = 0; i < items.Count(); i++){
			EntityAI item = items.Get(i);
			if (item && IsItemBlacklisted(item.GetType())){
				return item.GetType();
			}
		}
		return "";
	}
	
	string GetItemDisplayName(string className){
		string displayName = className;
		if (GetGame()){
			string path = "CfgVehicles " + className + " displayName";
			if (GetGame().ConfigIsExisting(path)){
				GetGame().ConfigGetText(path, displayName);
			}
		}
		if (displayName == ""){
			displayName = className;
		}
		return displayName;
	}
	
	static void SendRedirectRPC(UServerData serverData, PlayerIdentity identity){
		if (!serverData || !identity) return;
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(serverData.Name);
		rpc.Write(serverData.IP);
		rpc.Write(serverData.Port);
		rpc.Write(serverData.Password);
		rpc.Send(null, MAPLINK_REDIRECTKICKED, true, identity);
	}
	
	static UServerData ReadFromRPC(ParamsReadContext ctx){
		string name, ip, password;
		int port;
		if (ctx.Read(name) && ctx.Read(ip) && ctx.Read(port) && ctx.Read(password)){
			UServerData serverData = new UServerData(ip, port, password);
			serverData.Name = name;
			return serverData;
		}
		return null;
	}
}
