class UApiServerData extends Managed {
	
	string Name;
	string Map;
	string IP;
	int Port = 2302;
	string Password = "";
	
	void UApiServerData(string ip, int port = 2302, string password = ""){
		IP = ip;
		Port = port;
		Password = password;
	}
}

class MapLinkDepaturePoint extends Managed {
	string DisplayName;
	string ServerName;
	int SafeZoneRadius;
	string TerminalType;
	vector Position;
	vector Orientation;
	ref array<ref MapLinkArrivalPointsRef> ArrivalPoints = new array<ref MapLinkArrivalPointsRef>;
	
	void MapLinkDepaturePoint(string displayName){
		DisplayName = displayName;
		if (DisplayName == "Demo000"){
			ArrivalPoints.Insert(new MapLinkArrivalPointsRef);
		}
	}
	 
}

class MapLinkArrivalPointsRef {
	string ArrivalPointName;
	int TransitionWaitTime;
	int Cost;
	int AcceptedCurrencyId;
		
	MapLinkMoneyValue GetHighestDenomination(float amount){
		if (GetMapLinkConfig() && GetMapLinkConfig().Currencies.Get(AcceptedCurrencyId)){
			return MapLinkMoneyValue.Cast(GetMapLinkConfig().Currencies.Get(AcceptedCurrencyId).GetHighestDenomination(amount));
		}
		return NULL;
	}
	
	int GetAmount(MapLinkMoneyValue MoneyObj, float amount){
		if (GetMapLinkConfig() && GetMapLinkConfig().Currencies.Get(AcceptedCurrencyId)){
			return GetMapLinkConfig().Currencies.Get(AcceptedCurrencyId).GetAmount(MapLinkMoneyValue.Cast(MoneyObj), amount);
		}
		return 0;
	}
}

class MapLinkArrivalPoint extends Managed {
	string Name;
	ref array<ref MapLinkSpawnPoint> SpawnPoints = new array<ref MapLinkSpawnPoint>;
	
	void MapLinkArrivalPoint(string displayName){
		Name = displayName;
		if (Name == "Demo000"){
			SpawnPoints.Insert(new MapLinkSpawnPoint(true));
		}
	}
	
	
	vector GetSpawn(string serverName){
		for (int i = 0; i < SpawnPoints.Get(i); i++){
			if (SpawnPoints.Get(i).ServerName == serverName){
				return SpawnPoints.Get(i).GetSpawnPos();
			}
		}
		return vector.Zero;
	}
	
	int ProtectionTime(string serverName){
		for (int i = 0; i < SpawnPoints.Get(i); i++){
			if (SpawnPoints.Get(i).ServerName == serverName){
				return SpawnPoints.Get(i).ProtectionTime;
			}
		}
		return 0;
	}
	
	MapLinkSpawnPoint Get(string serverName){
		for (int i = 0; i < SpawnPoints.Get(i); i++){
			if (SpawnPoints.Get(i).ServerName == serverName){
				return SpawnPoints.Get(i);
			}
		}
		return NULL;
	}
	
	string FallBackServer(string serverName){
		for (int i = 0; i < SpawnPoints.Get(i); i++){
			if (SpawnPoints.Get(i).ServerName != serverName){
				return SpawnPoints.Get(i).ServerName;
			}
		}
		return "";
	}
}


class MapLinkSpawnPoint extends Managed{
	string ServerName;
	string EntryPointDisplayName;
	int ProtectionTime;
	ref array<ref MapLinkSpawnPointPos> Positions = new array<ref MapLinkSpawnPointPos>;
	
	void MapLinkSpawnPoint(bool defaultCfg = false){
		Positions.Insert(new MapLinkSpawnPointPos());	
	}
	
	
	vector GetSpawnPos(){
		if ( Positions.Count() == 1){
			return Positions.Get(0).Get();
		}
		int i = 0;
		float Max = 0;
		for (i = 0; i < Positions.Count(); i++){
			Max = Max + Positions.Get(i).Chance;
		}
		float random = Math.RandomFloat(0, Max);
		i = -1;
		while (++i < Positions.Count() && Positions.Get(i).Chance < random){
			//Don't need anything to happen here
		}
		return Positions.Get(i).Get();
	} 
}

class MapLinkSpawnPointPos{
	float X;
	float Y;
	float Z;
	float D;
	float Radius;
	float Chance;
	
	vector GetPosition(){
		float y = Y;
		if (Y < 1){
			y = GetGame().SurfaceY(X,Z);
		}
		return Vector(X, y, Z);
	}
	
	vector GetOrientation(){
		return Vector(D, 0, 0);
	}
	
	vector Get(){
	
		if (Radius < 2 ){
			return GetPosition();
		}
		
        vector rndPos = GetRandomPosition();
        int maxCalcs = 25;
        while ((GetGame().SurfaceIsSea(rndPos[0], rndPos[2]) || GetGame().SurfaceIsPond(rndPos[0],rndPos[2])) && maxCalcs-- > 0)
        {
            rndPos = GetRandomPosition();
        }
		if ( maxCalcs > 0 ){
			return Vector(rndPos[0], GetGame().SurfaceY(rndPos[0],rndPos[2]), rndPos[2]);
		}
		return GetPosition();
	}
	
	vector GetRandomPosition()
    {
		vector pos = Vector(X, 0, Z);
        float rndT = Math.RandomFloat(0,1) * 2 * Math.PI;
        float rndR = m_randomradius * Math.Sqrt(Math.RandomFloat(0,1)) * 0.90;
        //Adding 10% of the radius to make so its not in the center feel like this will just seem less random
        float rndRD = m_randomradius * 0.10;
        rndR = rndR + rndRD;

        float NewX = pos[0] + rndR * Math.Cos(rndT);
        float NewY = pos[2] + rndR * Math.Sin(rndT);
        return Vector(NewX, 0, NewY);
    }

}

class MapLinkCurrency extends Managed {
	
	int ID;
	ref array<ref MapLinkMoneyValue> MoneyValues = new array<ref MapLinkMoneyValue>;
	
	void MapLinkCurrency(int id = 1){
		if (id == -1){
			ID = 0;
			MoneyValues.Insert(new MapLinkMoneyValue("MoneyRuble100", 100));
			MoneyValues.Insert(new MapLinkMoneyValue("MoneyRuble50", 50));
			MoneyValues.Insert(new MapLinkMoneyValue("MoneyRuble25", 25));
			MoneyValues.Insert(new MapLinkMoneyValue("MoneyRuble10", 10));
			MoneyValues.Insert(new MapLinkMoneyValue("MoneyRuble5", 5));
			MoneyValues.Insert(new MapLinkMoneyValue("MoneyRuble1", 1));
		}
	}
	
	void SortMoney(){
		array<MapLinkMoneyValue> StartingValues =  new array<MapLinkMoneyValue>;
		for (int h = 0; h < MoneyValues.Count(); h++){
			StartingValues.Insert(MoneyValues.Get(h));
		}
		ref array<ref MapLinkMoneyValue> SortedMoneyValues = new array<ref MapLinkMoneyValue>;
		while (StartingValues.Count() > 0){
			ref MapLinkMoneyValue HighestValue = StartingValues.Get(0);
			for (int i = 1; i < StartingValues.Count(); i++){
				if (StartingValues.Get(i).Value > HighestValue.Value){
					HighestValue = StartingValues.Get(i);
				}
			}
			StartingValues.RemoveItem(HighestValue);
			SortedMoneyValues.Insert(HighestValue);
		}
		if (StartingValues.Count() == 1){
			SortedMoneyValues.Insert(StartingValues.Get(0));
		}
		MoneyValues = SortedMoneyValues;
	}
	
	MapLinkMoneyValue GetHighestDenomination(float amount){
		int LastIndex = MoneyValues.Count() - 1;
		for (int i = 0; i < MoneyValues.Count(); i++){
			if (GetAmount(MoneyValues.Get(i), amount) > 0){
				return MoneyValues.Get(i);
			}
		}
		return NULL;
	}
	
	int GetAmount(MapLinkMoneyValue MoneyObj, float amount){
		if (MoneyObj){
			return Math.Floor(amount / MoneyObj.Value);
		} 
		return 0;
	}
	
}

class MapLinkMoneyValue{
	
	string Item;
	float Value
	
	void MapLinkMoneyValue(string item, float value ){
		Item = item;
		Value = value;
	}
}