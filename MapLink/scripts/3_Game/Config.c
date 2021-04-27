class MapLinkConfig extends UApiConfigBase {

	ref array<ref UApiServerData> Servers = new array<ref UApiServerData>;
	ref array<ref MapLinkDepaturePoint> DepaturePoints = new array<ref MapLinkDepaturePoint>;
	ref array<ref MapLinkArrivalPoint> ArrivalPoints = new array<ref MapLinkArrivalPoint>;
	ref array<ref MapLinkCurrency> Currencies = new array<ref MapLinkCurrency>;
	
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
	
}


static ref MapLinkConfig m_MapLinkConfig;
static MapLinkConfig GetMapLinkConfig(){
	if (!m_MapLinkConfig){
		m_MapLinkConfig = new MapLinkConfig;
		m_MapLinkConfig.Load();
	}
	return m_MapLinkConfig;
}