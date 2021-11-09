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
	
	bool IsUnconscious(){
		return m_IsUnconscious;
	}
	
	bool IsRestrained(){
		return m_IsRestrained;
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