class UPlayerIdFloatData extends Managed {
	
	int m_ID;
	float m_Value = -1;
	
	void UPlayerIdFloatData(int id, float value = -1){
		m_ID = id;
		m_Value = value;
	}
	
	int ID(){ return m_ID; }
	float Value(){ return m_Value; }
	
}

class UStomachItem extends Managed {
	
	float m_Amount;
	int m_FoodStage;
	string m_ClassName;
	int m_Agents;
	
	void UStomachItem(float amount, int foodstage, string className, int agents){
		m_Amount = amount;
		m_FoodStage = foodstage;
		m_ClassName = className;
		m_Agents = agents;
	}
	
}

class UPlayerZoneHealthData extends Managed {
	
	string m_Zone;
	float m_Health;
	float m_Blood;
	float m_Shock;
	
	void UPlayerZoneHealthData(string zone, float health, float blood, float shock) {
		m_Zone = zone;
		m_Health = health;
		m_Blood = blood;
		m_Shock = shock;
	}
	
	bool Is(string zone){ 
		return (m_Zone == zone); 
	}
	string Zone(){ return m_Zone; }
	float Health(){ return m_Health; }
	float Blood(){ return m_Blood; }
	float Shock(){ return m_Shock; }
	
}