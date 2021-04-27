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
	
	
	override void Load(){
		m_DataReceived = false;
		SetDefaults();
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
	
	
	vector
	
}


static ref MapLinkConfig m_MapLinkConfig;
static MapLinkConfig GetMapLinkConfig(){
	if (!m_MapLinkConfig){
		m_MapLinkConfig = new MapLinkConfig;
		m_MapLinkConfig.Load();
	}
	return m_MapLinkConfig;
}