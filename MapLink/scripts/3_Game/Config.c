class MapLinkConfig extends UApiConfigBase {

	ref array<ref UApiServerData> Servers = new array<ref UApiServerData>;
	ref array<ref MapLinkDepaturePoint> DepaturePoints = new array<ref MapLinkDepaturePoint>;
	ref array<ref MapLinkArrivalPoint> ArrivalPoints = new array<ref MapLinkArrivalPoint>;
	ref array<ref MapLinkCurrency> Currencies = new array<ref MapLinkCurrency>;
	
	static float MAX_DEPATUREPOINT_DISTANCE = 50;
	
	override void SetDefaults(){
		/*
	
		  This is to set the defaults for the mod before requesting a load so that way 
		if it doesn't exsit the API will create the file
	
		*/
		Servers.Insert(new UApiServerData(""));
		DepaturePoints.Insert(new MapLinkDepaturePoint("Demo000"));
		ArrivalPoints.Insert(new MapLinkArrivalPoint("Demo000"));
		Currencies.Insert(new MapLinkCurrency(-1));
	}

	override void OnDataReceive(){
		SetDataReceived();
		/*
		if(ModVersion != CurrentVersion){
			DoSome Code Upgrade
		
			Save(); //Resave the upgrade Version Back to the server
		}
		*/
		
	}
	
	void Valiate(){
		
		
	}	
	
	
	override void Load(){
		if (!m_DataReceived){
			SetDefaults();
		}
		m_DataReceived = false;
		//Set the Defaults so that way, when you load if this its the server Requesting the data it will create it based on the defaults
		UApi().Rest().GlobalsLoad("MapLink", this, this.ToJson());
	}
	
	
	override void Save(){
		if (GetGame().IsServer()){
			UApi().Rest().GlobalsSave("MapLink", this.ToJson());
		}
	}
	
	
	override string ToJson(){
		return UApiJSONHandler<MapLinkConfig>.ToString(this);
	}
	
	// This is called by the API System on the successfull response from the API
	override void OnSuccess(string data, int dataSize) {
		if (UApiJSONHandler<MapLinkConfig>.FromString(data, this)){
			OnDataReceive();
		} else {
			Print("[UAPI] CallBack Failed errorCode: Invalid Data");
		}
	}
	
	MapLinkDepaturePoint GetDepaturePoint(vector Pos){
		float closestPointDistance = MAX_DEPATUREPOINT_DISTANCE;
		int closestPointIndex = -1;
		for (int i = 0; i < DepaturePoints.Count(); i++){
			float curPointDistance = vector.Distance( DepaturePoints.Get(i).Position, Pos);
			if (curPointDistance < closestPointDistance){
				closestPointIndex = i;
				closestPointDistance = curPointDistance;
			}
		}
		if (closestPointIndex >= 0){
			return MapLinkDepaturePoint.Cast(DepaturePoints.Get(closestPointIndex));
		}
		Error("[MAPLINK] COULD NOT FIND A MAP LINK DEPATURE POINT");
		return NULL;
	}
	
	
	MapLinkSpawnPointPos SpawnPointPos(string arrivalPoint){
		return GetSpawnPointPos(UApiConfig().ServerID, arrivalPoint);
	}
	
	
	MapLinkSpawnPointPos GetSpawnPointPos(string serverName, string arrivalPoint){
		for (int i = 0; i< ArrivalPoints.Count(); i++){
			if(ArrivalPoints.Get(i).Name == arrivalPoint){
				return MapLinkSpawnPointPos.Cast(ArrivalPoints.Get(i).GetSpawnPos(serverName));
			}
		}
		return NULL;
	}
	
	UApiServerData GetServer(string serverName){
		for(int i = 0; i < Servers.Count(); i++){
			if (Servers.Get(i).Name == serverName){
				return UApiServerData.Cast(Servers.Get(i));
			}
		}
		return NULL;
	}
	
	
	MapLinkArrivalPoint GetArrivalPoint(string arrivalPoint){
		for (int i = 0; i< ArrivalPoints.Count(); i++){
			if(ArrivalPoints.Get(i).Name == arrivalPoint){
				return MapLinkArrivalPoint.Cast(ArrivalPoints.Get(i));
			}
		}
		return NULL;
	}
	
	
	string GetNiceMapName(string worldName){
		string LowerWorldName = worldName;
		LowerWorldName.ToLower();
		switch (LowerWorldName){
			case "enoch":
				return "Liviona";
			case "enochgloom":
				return "Liviona";
			case "chernarusplus":
				return "Chernarus";
			case "chernarusplusgloom":
				return "Chernarus";
			case "deerisle":
				return "Deer Isle";
			case "chiemsee":
				return "Chiemsee";
			case "rostow":
				return "Rostow";
			case "namalsk":
				return "Namalsk";
			case "esseker":
				return "Esseker";
		}
		string FirstLeter = worldName.Substring(0,1);
		FirstLetter.ToUpper();
		worldName.Set(0, FirstLeter);
		return worldName;
	}
	
	string GetCostIcon(int id){
		for (int i = 0; i < Currencies.Count(); i++){
			if (Currencies.Get(i).ID == id){
				string icon = Currencies.Get(i).Icon;
				if (icon.Contains(".paa") || icon.Contains("set:") || icon.Contains(".edds") ){
					return icon;
				}
				return "set:maplink_money image:"+icon;
			}
		}
		return "set:maplink_money image:dollar";
	}
	
}


static ref MapLinkConfig m_MapLinkConfig;
static MapLinkConfig GetMapLinkConfig(){
	if (!m_MapLinkConfig){
		m_MapLinkConfig = new MapLinkConfig;
		m_MapLinkConfig.Load();
	}
	return m_MapLinkConfig;
}