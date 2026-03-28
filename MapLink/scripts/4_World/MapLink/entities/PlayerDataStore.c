class PlayerDataStore extends Managed{
	string GUID;
	string m_Name;
	string m_Type = "";
	bool m_IsDead;
	float m_Health;
	float m_Blood;
	float m_Shock;
	bool m_IsUnconscious;
	bool m_IsRestrained;
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
	int m_HasBloodyHandsVisible;
	bool m_Camera3rdPerson;
	int m_PersistentFlags;
	
	int m_BrokenLegState;
	autoptr array<autoptr UPlayerZoneHealthData> m_HealthZones;
	
	autoptr array<autoptr UPlayerIdFloatData> m_Modifiers;
	autoptr array<autoptr UPlayerIdFloatData> m_Agents;
	autoptr array<autoptr UMetaData> m_Stats;

	autoptr array<autoptr UEntityStore> m_Attachments;
	
	autoptr array<autoptr UStomachItem> m_Stomach;
	
	autoptr array<autoptr UMetaData> m_MetaData;
	
	autoptr TIntArray m_Symptoms;
	
	autoptr array<autoptr UArrowData> m_ArrowsData;
	
	void PlayerDataStore(PlayerBase player = NULL){
		if (!player){return;}
		this.SavePlayer(player);
	}	
	
	void ~PlayerDataStore(){
		//MLLog.Debug("~PlayerDataStore() - " + GUID);
		delete m_HealthZones;
		delete m_Modifiers;
		delete m_Agents;
		delete m_Stats;
		delete m_Attachments;
		delete m_Stomach;
		delete m_MetaData;
		delete m_Symptoms;
		delete m_ArrowsData;
	}
	
	
	string ToJson(){
		return UJSONHandler<PlayerDataStore>.ToString(this);
	}
	
	
	void AddModifier(int id, float attachedTime) {
		if (!m_Modifiers){m_Modifiers = new array<autoptr UPlayerIdFloatData>;}
		m_Modifiers.Insert(new UPlayerIdFloatData(id, attachedTime));
    }
	void AddAgent(int key, float value) {
		if (!m_Agents){m_Agents = new array<autoptr UPlayerIdFloatData>;}
		m_Agents.Insert(new UPlayerIdFloatData(key, value));
    }
	void AddStomachItem(float amount, int foodstage, string className, int agents, float temperature = 0){
		if ( !m_Stomach ){ m_Stomach = new array<autoptr UStomachItem>; }
		m_Stomach.Insert(new UStomachItem(amount, foodstage, className, agents, temperature));
	}
	
	bool AddStat(string label, float data){
		if (!m_Stats) { m_Stats = new array<autoptr UMetaData>;}
		m_Stats.Insert(new UMetaData(label, data.ToString()));
		return true;
	}
	
	bool ReadStat(string label, out float data){
		if (!m_Stats) return false;
		for(int i = 0; i < m_Stats.Count(); i++){
			if (m_Stats.Get(i) && m_Stats.Get(i).Is(label)){
				data = m_Stats.Get(i).ReadFloat();
				return true;
			}
		}
		return false;
	}
	
	bool Write(string var, bool data){
		if (!m_MetaData) { m_MetaData = new array<autoptr UMetaData>;}
		m_MetaData.Insert(new UMetaData(var, data.ToString()));
		return true;
	}
	bool Write(string var, int data){
		if (!m_MetaData) { m_MetaData = new array<autoptr UMetaData>;}
		m_MetaData.Insert(new UMetaData(var, data.ToString()));
		return true;
	}
	bool Write(string var, float data){
		if (!m_MetaData) { m_MetaData = new array<autoptr UMetaData>;}
		m_MetaData.Insert(new UMetaData(var, data.ToString()));
		return true;
	}
	bool Write(string var, vector data){
		if (!m_MetaData) { m_MetaData = new array<autoptr UMetaData>;}
		m_MetaData.Insert(new UMetaData(var, data.ToString()));
		return true;
	}
	bool Write(string var, TStringArray data){
		if (!m_MetaData) { m_MetaData = new array<autoptr UMetaData>;}
		for (int ii = 0; ii < data.Count(); ii++){
			m_MetaData.Insert(new UMetaData(var, data.Get(ii)));
		}
		return true;
	}
	bool Write(string var, TIntArray data){
		if (!m_MetaData) { m_MetaData = new array<autoptr UMetaData>;}
		for (int ii = 0; ii < data.Count(); ii++){
			m_MetaData.Insert(new UMetaData(var, data.Get(ii).ToString()));
		}
		return true;
	}
	bool Write(string var, TBoolArray data){
		if (!m_MetaData) { m_MetaData = new array<autoptr UMetaData>;}
		for (int ii = 0; ii < data.Count(); ii++){
			m_MetaData.Insert(new UMetaData(var, data.Get(ii).ToString()));
		}
		return true;
	}
	bool Write(string var, TFloatArray data){
		if (!m_MetaData) { m_MetaData = new array<autoptr UMetaData>;}
		for (int ii = 0; ii < data.Count(); ii++){
			m_MetaData.Insert(new UMetaData(var, data.Get(ii).ToString()));
		}
		return true;
	}
	bool Write(string var, TVectorArray data){
		if (!m_MetaData) { m_MetaData = new array<autoptr UMetaData>;}
		for (int ii = 0; ii < data.Count(); ii++){
			m_MetaData.Insert(new UMetaData(var, data.Get(ii).ToString()));
		}
		return true;
	}
	bool Write(string var, string data){
		if (!m_MetaData) { m_MetaData = new array<autoptr UMetaData>;}
		m_MetaData.Insert(new UMetaData(var, data));
		return true;
	}
	bool Write(string var, Class data){
		MLLog.Err("Trying to save undefined data class to " + var + " for " + m_Type + " try converting to a string before saving");
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
		MLLog.Err("Trying to read undefined data class for " + var + " for " + m_Type + " try converting to a string before saving");
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
		if (!m_HealthZones){m_HealthZones = new array<autoptr UPlayerZoneHealthData>}
		m_HealthZones.Insert(new UPlayerZoneHealthData(zone, health, blood, shock));
		return true;
	}
	bool ReadZoneHealth(string zone, out float health, out float blood, out float shock){
		if (!m_HealthZones) return false;
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
	
	bool IsUnconscious(){
		return m_IsUnconscious;
	}
	
	bool IsRestrained(){
		return m_IsRestrained;
	}
	
	void SavePlayer(PlayerBase player){
		if (!player){ return; }
		if (!player.GetCachedIdentity(GUID, m_Name) && !player.GetIdentity()){return;}
		int i = 0;
		
		if (player.GetIdentity()){
			GUID = player.GetIdentity().GetId();
			m_Name = player.GetIdentity().GetName();
			//MLLog.Debug("Savinging Player " + m_Name + " ("+ GUID +")");
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
		m_HasBloodyHandsVisible = player.HasBloodyHandsEx();
		m_IsUnconscious = player.IsUnconscious();
		m_IsRestrained = player.IsRestrained();
		m_PersistentFlags = player.m_PersistentFlags;
		
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
				if (!m_Attachments){m_Attachments = new array<autoptr UEntityStore>;}
				if (attachment_item && (player.GetInventory().HasAttachment(attachment_item) || attachment_item == player.GetItemInHands())){
					UEntityStore att_itemstore = new UEntityStore(attachment_item);
					m_Attachments.Insert(att_itemstore);
				} else {
					break;
				}
			}
		}
		player.OnUFSave(this);
		g_Game.GetWorldName(m_Map);
		m_Server = UFConfig().ServerID;
	}
	
	
	
	PlayerBase CreateWithIdentity(PlayerIdentity identity, vector Pos = vector.Zero){
		if (vector.Distance(Pos, vector.Zero) < 1){
			Pos = m_Position;
		}
		MLLog.Debug("CreateWithIdentity at " + Pos);
		Entity playerEnt = g_Game.CreatePlayer(PlayerIdentity.Cast(identity), m_Type, Pos, 0, "NONE");
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
		
		
		MLLog.Debug("SetupPlayer at " + Pos);
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
		player.SetBloodyHandsEx(m_HasBloodyHandsVisible);
		player.m_PersistentFlags = m_PersistentFlags;
		
		player.OnUFLoad(this);
		
		player.GetStatBloodType().Set(m_BloodType);
	}
}