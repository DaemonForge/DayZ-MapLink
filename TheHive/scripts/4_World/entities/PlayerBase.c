modded class PlayerBase extends ManBase{
	
	protected bool m_DontLoadFromApi = true;
	protected bool m_HiveLinked = false;
	protected string m_TransferPoint = "";
	
	bool UApiSaveTransferPoint(string point = ""){
		m_TransferPoint = point;
		return true;
	}
	
	bool ToggleDontLoadFromApi(){
		if (GetIdentity()){
			Print("[UPAI]ToggleDontLoadFromApi " + GetIdentity().GetId());	
		}
		m_DontLoadFromApi = !m_DontLoadFromApi;
		return m_DontLoadFromApi;
	}
	
	bool DontLoadFromApi(){
		return m_DontLoadFromApi;
	}
	
	int GetQuickBarEntityIndex(EntityAI entity){
		return m_QuickBarBase.FindEntityIndex(entity);
	}
	

    override void OnStoreSave(ParamsWriteContext ctx)
    {
        super.OnStoreSave(ctx);
		this.SavePlayerToUApi();
    }
	
	void SavePlayerToUApi(){
		if (this.GetIdentity() && GetGame().IsServer()){
			autoptr PlayerDataStore teststore = new PlayerDataStore(PlayerBase.Cast(this));
			UApi().db(PLAYER_DB).Save("TheHive", this.GetIdentity().GetId(), teststore.ToJson());
			delete teststore;
			NotificationSystem.SimpleNoticiation(" You're Data has been saved to the API", "Notification","Notifications/gui/data/notifications.edds", -16843010, 10, this.GetIdentity());
		}
	}

    override bool OnStoreLoad(ParamsReadContext ctx, int version)
    {
        if (!super.OnStoreLoad(ctx,version))
            return false;
		 
        return true;
    }
	
	void OnUApiSave(PlayerDataStore data){
		int i = 0;
		data.m_LifeSpanState = m_LifeSpanState;
		data.m_LastShavedSeconds = m_LastShavedSeconds;
		data.m_BloodType = m_BloodType;
		data.m_HasBloodTypeVisible = m_HasBloodTypeVisible;
		data.m_HasBloodyHandsVisible = m_HasBloodyHandsVisible;
		for(i = 0; i < GetPlayerStats().GetPCO(GetGame().SaveVersion()).Get().Count(); i++){
			data.AddPlayerStat(GetPlayerStats().GetPCO(GetGame().SaveVersion()).Get().Get(i));
		}
		for(i = 0; i < m_ModifiersManager.m_ModifierList.Count(); i++){
            ModifierBase mdfr = ModifierBase.Cast(m_ModifiersManager.m_ModifierList.GetElement(i));
            if (mdfr && mdfr.IsActive() && mdfr.IsPersistent()) { 
				data.AddModifier( mdfr.GetModifierID(), mdfr.GetAttachedTime());
			}
		}
		for(i = 0; i < m_AgentPool.m_VirusPool.Count();i++){
			data.AddAgent(m_AgentPool.m_VirusPool.GetKey(i), m_AgentPool.m_VirusPool.GetElement(i));
		}
		data.m_TransferPoint = m_TransferPoint;
		data.m_BrokenLegState = m_BrokenLegState;
		
		data.m_BleedingBits = GetBleedingBits();
		if (GetBleedingManagerServer()){	
			GetBleedingManagerServer().OnUApiSave(data);
		}
	}
	
	void OnUApiLoad(PlayerDataStore data){
		int i = 0;
		SetBloodType(data.m_BloodType);
		SetLifeSpanStateVisible(data.m_LifeSpanState);
		SetLastShavedSeconds(data.m_LastShavedSeconds);
		SetBloodyHands(data.m_HasBloodyHandsVisible);
		SetBloodTypeVisible(data.m_HasBloodTypeVisible);	
		for (i = 0; i < data.m_Modifiers.Count(); i++){
			if (data.m_Modifiers.Get(i)){
				ModifierBase mdfr = m_ModifiersManager.GetModifier(data.m_Modifiers.Get(i).ID());
				if (mdfr.IsTrackAttachedTime() && data.m_Modifiers.Get(i).Value() >= 0){
					mdfr.SetAttachedTime(data.m_Modifiers.Get(i).Value());
				}
				m_ModifiersManager.ActivateModifier(data.m_Modifiers.Get(i).ID(), EActivationType.TRIGGER_EVENT_ON_CONNECT);
			}
		}
		for(i = 0; i < data.m_Agents.Count();i++){
			m_AgentPool.SetAgentCount(data.m_Agents.Get(i).ID(), data.m_Agents.Get(i).Value());
		}
		if (data.m_PlayerStats.Count() == GetPlayerStats().GetPCO(GetGame().SaveVersion()).Get().Count()){
			for(i = 0; i < GetPlayerStats().GetPCO(GetGame().SaveVersion()).Get().Count(); i++){
				GetPlayerStats().GetPCO(GetGame().SaveVersion()).Get().Get(i).SetByFloat(data.m_PlayerStats.Get(i));
			}
		} else {
			Print("[UAPI] [ERROR] Stats didn't match?");
		}
		data.m_TransferPoint = "";
		m_TransferPoint = "";
		m_BrokenLegState = data.m_BrokenLegState;
		SetBleedingBits(data.m_BleedingBits);
		if (GetBleedingManagerServer()){	
			GetBleedingManagerServer().OnUApiLoad(data);
		}
	}
	
	override void OnDisconnect(){
		
		super.OnDisconnect();
	}
	
	
	override void EEKilled( Object killer )
	{
		if (m_TransferPoint == ""){
			this.SavePlayerToUApi();
		}
		if (m_TransferPoint != ""){
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.Delete, 300,false);
		}
		
		super.EEKilled( killer );
	}

	
	void UApiKillAndDeletePlayer(){
		SetHealth("","", 0);
	}
}