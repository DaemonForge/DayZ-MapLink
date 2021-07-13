class PlayerDataStore extends Managed{
	string GUID;
	string m_Name;
	string m_Type = "";
	bool m_IsDead;
	float m_Health;
	float m_Blood;
	float m_Shock;
	float m_TimeSurvivedValue;
	float m_PlayersKilledValue;
	float m_InfectedKilledValue;
	float m_DistanceTraveledValue;
	float m_LongRangeShotValue;
	vector m_Position;
	vector m_Orientaion;
	string m_Map;
	string m_Server;
	string m_TransferPoint;
	int m_BleedingBits;
	autoptr TIntArray m_BleedingActiveTime;
	int	m_LifeSpanState;
	int	m_LastShavedSeconds;
	int	m_BloodType;
	bool m_HasBloodTypeVisible;
	bool m_HasBloodyHandsVisible;
	bool m_Camera3rdPerson;
	
	int m_BrokenLegState;
	autoptr array<autoptr UApiPlayerZoneHealthData> m_HealthZones;
	
	autoptr array<autoptr UApiPlayerIdFloatData> m_Modifiers;
	autoptr array<autoptr UApiPlayerIdFloatData> m_Agents;
	autoptr array<autoptr UApiMetaData> m_Stats;

	autoptr array<autoptr UApiEntityStore> m_Attachments;
	
	autoptr array<autoptr UApiStomachItem> m_Stomach;
	
	autoptr array<autoptr UApiMetaData> m_MetaData;
	
	void PlayerDataStore(PlayerBase player = NULL){
		if (!player){return;}
		this.SavePlayer(player);
		
	}	
	
	void ~PlayerDataStore(){
		//Print("[UAPI] ~PlayerDataStore() - " + GUID);
		delete m_HealthZones;
		delete m_Modifiers;
		delete m_Agents;
		delete m_Stats;
		delete m_Attachments;
		delete m_Stomach;
		delete m_MetaData;
	}
	
	void SavePlayer(PlayerBase player){
		if (!player){return;}
		int i = 0;
		
		if (player.GetIdentity()){
			GUID = player.GetIdentity().GetId();
			m_Name = player.GetIdentity().GetName();
			Print("[MAPLINK] Savinging Player " + m_Name + " ("+ GUID +")");
		}
		m_Type = player.GetType();
		m_Health = player.GetHealth("", "Health");
		if (m_Health > 0){
			m_IsDead = false;
		} else {
			m_IsDead = true;
		}
		m_Blood = player.GetHealth("", "Blood");
		m_Shock = player.GetHealth("GlobalHealth", "Shock");
		m_Position = player.GetPosition();
		m_Orientaion = player.GetOrientation();
		
		m_TimeSurvivedValue = player.StatGet(AnalyticsManagerServer.STAT_PLAYTIME);
		m_PlayersKilledValue = player.StatGet(AnalyticsManagerServer.STAT_PLAYERS_KILLED);
		m_InfectedKilledValue = player.StatGet(AnalyticsManagerServer.STAT_INFECTED_KILLED);
		m_DistanceTraveledValue = player.StatGet(AnalyticsManagerServer.STAT_DISTANCE);
		m_LongRangeShotValue = player.StatGet(AnalyticsManagerServer.STAT_LONGEST_SURVIVOR_HIT );
		m_LifeSpanState = player.GetLifeSpanState();
		m_LastShavedSeconds = player.GetLastShavedSeconds();
		m_BloodType = player.GetStatBloodType().Get();
		m_HasBloodTypeVisible = player.HasBloodTypeVisible();
		m_HasBloodyHandsVisible = player.HasBloodyHands();
		
		
		// Damage System
		DamageZoneMap zones = new DamageZoneMap;
		DamageSystem.GetDamageZoneMap(player,zones);
		for( i = 0; i < zones.Count(); i++ ){
			string zone = zones.GetKey(i);
			SaveZoneHealth(zone, player.GetHealth(zone, "Health"), player.GetHealth(zone, "Blood"), player.GetHealth(zone, "Shock"));
		}
		
		array<EntityAI> items = new array<EntityAI>;
		player.GetInventory().EnumerateInventory(InventoryTraversalType.LEVELORDER, items);
		if (items && items.Count() > 0){
			for (i = 0; i < items.Count(); i++){
				EntityAI attachment_item = EntityAI.Cast(items.Get(i));
				if (!m_Attachments){m_Attachments = new array<autoptr UApiEntityStore>;}
				if (attachment_item && (player.GetInventory().HasAttachment(attachment_item) || attachment_item == player.GetItemInHands())){
					UApiEntityStore att_itemstore = new UApiEntityStore(attachment_item);
					m_Attachments.Insert(att_itemstore);
				} else {
					break;
				}
			}
		}
		player.OnUApiSave(this);
		GetGame().GetWorldName(m_Map);
		m_Server = UApiConfig().ServerID;
	}
	
	
	
	PlayerBase CreateWithIdentity(PlayerIdentity identity, vector Pos = vector.Zero){
		if (vector.Distance(Pos, vector.Zero) < 1){
			Pos = m_Position;
		}
		Print("[MAPLINK] CreateWithIdentity at " + Pos);
		Entity playerEnt = GetGame().CreatePlayer(PlayerIdentity.Cast(identity), m_Type, Pos, 0, "NONE");
		PlayerBase player;
		Class.CastTo(player, playerEnt);
		return player;
	}
	
	void SetupPlayer(PlayerBase player, vector Pos = vector.Zero, vector Ori =  vector.Zero){
		if (!player){ return; }
		int i = 0;
		
		if (vector.Distance(Pos, vector.Zero) < 1){
			Pos = m_Position;
		}
		if (vector.Distance(Ori, vector.Zero) < 0.1){
			Ori = m_Orientaion;
		}
		
		
		player.SetHealth("", "Health", m_Health);
		player.SetHealth("", "Blood", m_Blood);
		player.SetHealth("GlobalHealth", "Shock", m_Shock);
		
		// Damage System
		DamageZoneMap zones = new DamageZoneMap;
		DamageSystem.GetDamageZoneMap(player,zones);
		for( i = 0; i < zones.Count(); i++ ){
			string zone = zones.GetKey(i);
			float health, blood, shock;
			if (ReadZoneHealth(zone, health, blood, shock)){
				player.SetHealth(zone, "Health", health);
				player.SetHealth(zone, "Blood", blood);
				player.SetHealth(zone, "Shock", shock);
			}
		}
		
		
		Print("[MAPLINK] SetupPlayer at " + Pos);
		player.SetPosition(Pos);
		player.SetOrientation(Ori);
		
		player.SetBloodType(m_BloodType);
		player.SetBloodTypeVisible(m_HasBloodTypeVisible);
		
		
		if (m_Attachments && m_Attachments.Count() > 0){
			for(i = 0; i < m_Attachments.Count(); i++){
				if (m_Attachments.Get(i)){
					m_Attachments.Get(i).Create(player);				
				}
			}
		}
		player.StatUpdate(AnalyticsManagerServer.STAT_PLAYTIME, m_TimeSurvivedValue );
		player.StatUpdate(AnalyticsManagerServer.STAT_PLAYERS_KILLED, m_PlayersKilledValue);
		player.StatUpdate(AnalyticsManagerServer.STAT_INFECTED_KILLED, m_InfectedKilledValue);
		player.StatUpdate(AnalyticsManagerServer.STAT_DISTANCE, m_DistanceTraveledValue);
		player.StatUpdate(AnalyticsManagerServer.STAT_LONGEST_SURVIVOR_HIT, m_LongRangeShotValue );
		player.SetLifeSpanStateVisible(m_LifeSpanState);
		player.SetLastShavedSeconds(m_LastShavedSeconds);
		player.SetBloodyHands(m_HasBloodyHandsVisible);
		
		player.OnUApiLoad(this);
		
		player.GetStatBloodType().Set(m_BloodType);
	}
	
	string ToJson(){
		return UApiJSONHandler<PlayerDataStore>.ToString(this);
	}
	
	
	void AddModifier(int id, float attachedTime) {
		if (!m_Modifiers){m_Modifiers = new array<autoptr UApiPlayerIdFloatData>;}
		m_Modifiers.Insert(new UApiPlayerIdFloatData(id, attachedTime));
    }
	void AddAgent(int key, float value) {
		if (!m_Agents){m_Agents = new array<autoptr UApiPlayerIdFloatData>;}
		m_Agents.Insert(new UApiPlayerIdFloatData(key, value));
    }
	void AddStomachItem(float amount, int foodstage, string className, int agents){
		if ( !m_Stomach ){ m_Stomach = new array<autoptr UApiStomachItem>; }
		m_Stomach.Insert(new UApiStomachItem(amount, foodstage, className, agents));
	}
	
	bool AddStat(string label, float data){
		if (!m_Stats) { m_Stats = new array<autoptr UApiMetaData>;}
		m_Stats.Insert(new UApiMetaData(label, data.ToString()));
		return true;
	}
	
	bool ReadStat(string label, out float data){
		for(int i = 0; i < m_Stats.Count(); i++){
			if (m_Stats.Get(i) && m_Stats.Get(i).Is(label)){
				data = m_Stats.Get(i).ReadFloat();
				return true;
			}
		}
		return false;
	}
	
	bool Write(string var, bool data){
		if (!m_MetaData) { m_MetaData = new array<autoptr UApiMetaData>;}
		m_MetaData.Insert(new UApiMetaData(var, data.ToString()));
		return true;
	}
	bool Write(string var, int data){
		if (!m_MetaData) { m_MetaData = new array<autoptr UApiMetaData>;}
		m_MetaData.Insert(new UApiMetaData(var, data.ToString()));
		return true;
	}
	bool Write(string var, float data){
		if (!m_MetaData) { m_MetaData = new array<autoptr UApiMetaData>;}
		m_MetaData.Insert(new UApiMetaData(var, data.ToString()));
		return true;
	}
	bool Write(string var, vector data){
		if (!m_MetaData) { m_MetaData = new array<autoptr UApiMetaData>;}
		m_MetaData.Insert(new UApiMetaData(var, data.ToString()));
		return true;
	}
	bool Write(string var, TStringArray data){
		if (!m_MetaData) { m_MetaData = new array<autoptr UApiMetaData>;}
		for (int ii = 0; ii < data.Count(); ii++){
			m_MetaData.Insert(new UApiMetaData(var, data.Get(ii)));
		}
		return true;
	}
	bool Write(string var, TIntArray data){
		if (!m_MetaData) { m_MetaData = new array<autoptr UApiMetaData>;}
		for (int ii = 0; ii < data.Count(); ii++){
			m_MetaData.Insert(new UApiMetaData(var, data.Get(ii).ToString()));
		}
		return true;
	}
	bool Write(string var, TBoolArray data){
		if (!m_MetaData) { m_MetaData = new array<autoptr UApiMetaData>;}
		for (int ii = 0; ii < data.Count(); ii++){
			m_MetaData.Insert(new UApiMetaData(var, data.Get(ii).ToString()));
		}
		return true;
	}
	bool Write(string var, TFloatArray data){
		if (!m_MetaData) { m_MetaData = new array<autoptr UApiMetaData>;}
		for (int ii = 0; ii < data.Count(); ii++){
			m_MetaData.Insert(new UApiMetaData(var, data.Get(ii).ToString()));
		}
		return true;
	}
	bool Write(string var, TVectorArray data){
		if (!m_MetaData) { m_MetaData = new array<autoptr UApiMetaData>;}
		for (int ii = 0; ii < data.Count(); ii++){
			m_MetaData.Insert(new UApiMetaData(var, data.Get(ii).ToString()));
		}
		return true;
	}
	bool Write(string var, string data){
		if (!m_MetaData) { m_MetaData = new array<autoptr UApiMetaData>;}
		m_MetaData.Insert(new UApiMetaData(var, data));
		return true;
	}
	bool Write(string var, Class data){
		Error("[UAPI] Trying to save undefined data class to " + var + " for " + m_Type + " try converting to a string before saving");
		return false;
	}
	
	
	bool Read(string var, out bool data){
		for(int i = 0; i < m_MetaData.Count(); i++){
			if (m_MetaData.Get(i) && m_MetaData.Get(i).Is(var)){
				data = m_MetaData.Get(i).ReadInt();
				return true;
			}
		}
		return false;
	}
	bool Read(string var, out int data){
		for(int i = 0; i < m_MetaData.Count(); i++){
			if (m_MetaData.Get(i) && m_MetaData.Get(i).Is(var)){
				data = m_MetaData.Get(i).ReadInt();
				return true;
			}
		}
		return false;
	}
	bool Read(string var, out float data){
		for(int i = 0; i < m_MetaData.Count(); i++){
			if (m_MetaData.Get(i) && m_MetaData.Get(i).Is(var)){
				data = m_MetaData.Get(i).ReadFloat();
				return true;
			}
		}
		return false;
	}
	bool Read(string var, out vector data){
		for(int i = 0; i < m_MetaData.Count(); i++){
			if (m_MetaData.Get(i) && m_MetaData.Get(i).Is(var)){
				data = m_MetaData.Get(i).ReadVector();
				return true;
			}
		}
		return false;
	}
	bool Read(string var, out TStringArray data){
		bool found = false;
		for(int i = 0; i < m_MetaData.Count(); i++){
			if (m_MetaData.Get(i) && m_MetaData.Get(i).Is(var)){
				data.Insert(m_MetaData.Get(i).ReadString());
				found = true;
			}
		}
		return found;
	}
	bool Read(string var, out TIntArray data){
		bool found = false;
		for(int i = 0; i < m_MetaData.Count(); i++){
			if (m_MetaData.Get(i) && m_MetaData.Get(i).Is(var)){
				data.Insert(m_MetaData.Get(i).ReadInt());
				found = true;
			}
		}
		return found;
	}
	bool Read(string var, out TFloatArray data){
		bool found = false;
		for(int i = 0; i < m_MetaData.Count(); i++){
			if (m_MetaData.Get(i) && m_MetaData.Get(i).Is(var)){
				data.Insert(m_MetaData.Get(i).ReadFloat());
				found = true;
			}
		}
		return found;
	}
	bool Read(string var, out TBoolArray data){
		bool found = false;
		for(int i = 0; i < m_MetaData.Count(); i++){
			if (m_MetaData.Get(i) && m_MetaData.Get(i).Is(var)){
				bool value = (m_MetaData.Get(i).ReadInt());
				data.Insert(value);
				found = true;
			}
		}
		return found;
	}
	bool Read(string var, out TVectorArray data){
		bool found = false;
		for(int i = 0; i < m_MetaData.Count(); i++){
			if (m_MetaData.Get(i) && m_MetaData.Get(i).Is(var)){
				data.Insert(m_MetaData.Get(i).ReadVector());
				found = true;
			}
		}
		return found;
	}
	bool Read(string var, out string data){
		for(int i = 0; i < m_MetaData.Count(); i++){
			if (m_MetaData.Get(i) && m_MetaData.Get(i).Is(var)){
				data = m_MetaData.Get(i).ReadString();
				return true;
			}
		}
		return false;
	}
	bool Read(string var, out Class data){
		Error("[UAPI] Trying to read undefined data class for " + var + " for " + m_Type + " try converting to a string before saving");
		return false;
	}
	
	int GetInt(string var){
		for(int i = 0; i < m_MetaData.Count(); i++){
			if (m_MetaData.Get(i) && m_MetaData.Get(i).Is(var)){return m_MetaData.Get(i).ReadInt();}
		}
		return 0;
	}
	float GetFloat(string var){
		for(int i = 0; i < m_MetaData.Count(); i++){
			if (m_MetaData.Get(i) && m_MetaData.Get(i).Is(var)){ return m_MetaData.Get(i).ReadFloat(); }
		}
		return 0;
	}
	vector GetVector(string var){
		for(int i = 0; i < m_MetaData.Count(); i++){
			if (m_MetaData.Get(i) && m_MetaData.Get(i).Is(var)){ return m_MetaData.Get(i).ReadVector(); }
		}
		return Vector(0,0,0);
	}
	string GetString(string var){
		for (int i = 0; i < m_MetaData.Count(); i++){
			if (m_MetaData.Get(i) && m_MetaData.Get(i).Is(var)){ return m_MetaData.Get(i).ReadString(); }
		}
		return "";
	}
	
	
	bool SaveZoneHealth(string zone, float health, float blood, float shock){
		if (!m_HealthZones){m_HealthZones = new array<autoptr UApiPlayerZoneHealthData>}
		m_HealthZones.Insert(new UApiPlayerZoneHealthData(zone, health, blood, shock));
		return true;
	}
	bool ReadZoneHealth(string zone, out float health, out float blood, out float shock){
		for (int i = 0; i < m_HealthZones.Count(); i++){
			if (m_HealthZones.Get(i) && m_HealthZones.Get(i).Is(zone)){ 
				health = m_HealthZones.Get(i).Health();
				blood = m_HealthZones.Get(i).Blood();
				shock = m_HealthZones.Get(i).Shock();
				return true;
			}
		}
		return false;
	}
	
	bool IsValid(){
		return (GUID && GUID != "" && m_Type != "");
	}
	
	bool IsAlive(){
		return (!m_IsDead && m_Health > 0);
	}
	
}

class UApiPlayerIdFloatData extends Managed{
	
	int m_ID;
	float m_Value = -1;
	
	void UApiPlayerIdFloatData(int id, float value = -1){
		m_ID = id;
		m_Value = value;
	}
	
	int ID(){ return m_ID; }
	float Value(){ return m_Value; }
	
}

class UApiStomachItem extends Managed {
	
	float m_Amount;
	int m_FoodStage;
	string m_ClassName;
	int m_Agents;
	
	void UApiStomachItem(float amount, int foodstage, string className, int agents){
		m_Amount = amount;
		m_FoodStage = foodstage;
		m_ClassName = className;
		m_Agents = agents;
	}
	
}

class UApiPlayerZoneHealthData extends Managed {
	
	string m_Zone;
	float m_Health;
	float m_Blood;
	float m_Shock;
	
	void UApiPlayerZoneHealthData(string zone, float health, float blood, float shock) {
		m_Zone = zone;
		m_Health = health;
		m_Blood = blood;
		m_Shock = shock;
	}
	
	bool Is(string zone){ return (m_Zone == zone); }
	string Zone(){ return m_Zone; }
	float Health(){ return m_Health; }
	float Blood(){ return m_Blood; }
	float Shock(){ return m_Shock; }
	
}