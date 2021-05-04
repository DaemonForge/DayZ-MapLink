modded class PlayerBase extends ManBase{
	
	protected bool m_MapLink_UnderProtection = false;
	
	protected string m_TransferPoint = "";
	protected ref Timer m_MapLink_UnderProtectionTimer;
	
	
	override void Init()
	{
		super.Init();
		RegisterNetSyncVariableBool("m_MapLink_UnderProtection");
	}
	
	bool IsUnderMapLinkProtection(){
		return (m_MapLink_UnderProtection);
	}
	
	
	bool UApiSaveTransferPoint(string point = ""){
		m_TransferPoint = point;
		return true;
	}
	
	
	bool IsBeingTransfered(){
		return (m_TransferPoint != "");
	}
	
	protected void UpdateMapLinkProtectionClient(int time){
		Print("[MAPLINK] UpdateMapLinkProtectionClient" + time);
		if (time > 0){
			GetDayZGame().MapLinkStartCountDown(time);		
			GetInputController().OverrideRaise(true, false);
			GetInputController().OverrideMeleeEvade(true, false);
		}
		if (time <= 0){
			GetDayZGame().MapLinkStopCountDown();		
			GetInputController().OverrideMeleeEvade(false, false);
			GetInputController().OverrideRaise(false, false);
		}
	}
	
	
	void UpdateMapLinkProtection(int time = -1){
		Print("[MAPLINK] UpdateMapLinkProtection" + time);
		if (!GetGame().IsServer()){return;}
		RPCSingleParam(MAPLINK_UNDERPROTECTION, new Param1<int>(time), true, GetIdentity());
		if (m_MapLink_UnderProtection && time < 0){
			GetInputController().OverrideMeleeEvade(false, false);
			GetInputController().OverrideRaise(false, false);
			if (m_MapLink_UnderProtectionTimer){
				if (m_MapLink_UnderProtectionTimer.IsRunning()){
					m_MapLink_UnderProtectionTimer.Stop();
				}
			}
			RemoveProtectionSafe();
			return;
		}
		SetAllowDamage(false);
		m_MapLink_UnderProtection = true;
		GetInputController().OverrideMeleeEvade(true, false);
		GetInputController().OverrideRaise(true, false);
		
		SetSynchDirty();
		
		if (!m_MapLink_UnderProtectionTimer){
			m_MapLink_UnderProtectionTimer = new Timer;
		}
		if (m_MapLink_UnderProtectionTimer.IsRunning()){
			m_MapLink_UnderProtectionTimer.Stop();
		}
		m_MapLink_UnderProtectionTimer.Run(time, this, "UpdateMapLinkProtection", new Param1<int>(-1), false);
	}

	protected void RemoveProtectionSafe(){
		bool PlayerHasGodMode = false;
		#ifdef JM_COT
			if ( GetGame().IsServer() && m_JMHasGodMode ){
				Print("[MAPLINK] RemoveProtectionSafe COT ADMIN TOOLS ACTIVE");
				PlayerHasGodMode = true;
			}
		#endif
		#ifdef VPPADMINTOOLS
			if ( GetGame().IsServer() && hasGodmode ){
				Print("[MAPLINK] RemoveProtectionSafe VPP ADMIN TOOLS ACTIVE");
				PlayerHasGodMode = true;
			}
		#endif
		#ifdef ZOMBERRY_AT
			if ( GetGame().IsServer() && ZBGodMode ){
				Print("[MAPLINK] RemoveProtectionSafe ZOMBERRY ADMIN TOOLS ACTIVE");
				PlayerHasGodMode = true;
			}
		#endif
		#ifdef TRADER 
			if (GetGame().IsServer() && m_Trader_IsInSafezone){
				Print("[MAPLINK] RemoveProtectionSafe Player Is In Safe Zone");
				PlayerHasGodMode = true;
			}
		#endif
		if (!PlayerHasGodMode){
			SetAllowDamage(true);
		}
		m_MapLink_UnderProtection = false;
		SetSynchDirty();
	}
	
	
    override void OnStoreSave(ParamsWriteContext ctx)
    {
        super.OnStoreSave(ctx);
		//Making sure not to save freshspawns or dead people, dead people logic is handled in EEKilled
		if (!GetGame().IsClient() && GetHealth("","Health") > 0 && StatGet(AnalyticsManagerServer.STAT_PLAYTIME) > 90 ){ 
			this.SavePlayerToUApi();
		}
    }
	
	void SavePlayerToUApi(){
		if (this.GetIdentity() && GetGame().IsServer()){
			autoptr PlayerDataStore teststore = new PlayerDataStore(PlayerBase.Cast(this));
			UApi().db(PLAYER_DB).Save("MapLink", this.GetIdentity().GetId(), teststore.ToJson());
			delete teststore;
			//NotificationSystem.SimpleNoticiation(" You're Data has been saved to the API", "Notification","Notifications/gui/data/notifications.edds", -16843010, 10, this.GetIdentity());
		}
	}
	
	void OnUApiSave(ref PlayerDataStore data){
		int i = 0;
		data.m_TimeSurvivedValue = StatGet(AnalyticsManagerServer.STAT_PLAYTIME);
		data.m_PlayersKilledValue = StatGet(AnalyticsManagerServer.STAT_PLAYERS_KILLED);
		data.m_InfectedKilledValue = StatGet(AnalyticsManagerServer.STAT_INFECTED_KILLED);
		data.m_DistanceTraveledValue = StatGet(AnalyticsManagerServer.STAT_DISTANCE);
		data.m_LongRangeShotValue = StatGet(AnalyticsManagerServer.STAT_LONGEST_SURVIVOR_HIT );
		data.m_LifeSpanState = GetLifeSpanState();
		data.m_LastShavedSeconds = GetLastShavedSeconds();
		data.m_BloodType = GetStatBloodType().Get();
		data.m_HasBloodTypeVisible = HasBloodyHands();
		data.m_HasBloodyHandsVisible = HasBloodTypeVisible();
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
		} else {
			Print("[MAPLINK] Bleeding Manager is NULL");
		}
		
	}
	
	void OnUApiLoad(ref PlayerDataStore data){
		int i = 0;
		
		StatUpdate(AnalyticsManagerServer.STAT_PLAYTIME, data.m_TimeSurvivedValue );
		StatUpdate(AnalyticsManagerServer.STAT_PLAYERS_KILLED, data.m_PlayersKilledValue);
		StatUpdate(AnalyticsManagerServer.STAT_INFECTED_KILLED, data.m_InfectedKilledValue);
		StatUpdate(AnalyticsManagerServer.STAT_DISTANCE, data.m_DistanceTraveledValue);
		StatUpdate(AnalyticsManagerServer.STAT_LONGEST_SURVIVOR_HIT, data.m_LongRangeShotValue );
		SetLifeSpanStateVisible(data.m_LifeSpanState);
		SetLastShavedSeconds(data.m_LastShavedSeconds);
		SetBloodyHands(data.m_HasBloodyHandsVisible);
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
		data.m_TransferPoint = "";
		m_TransferPoint = "";
		m_BrokenLegState = data.m_BrokenLegState;
		//SetBleedingBits(data.m_BleedingBits);
		if (GetBleedingManagerServer()){	
			GetBleedingManagerServer().OnUApiLoad(data);
		} else {
			Print("[MAPLINK] Bleeding Manager is NULL");
		}
		GetStatWet().Set(data.m_Stat_Wet);
		GetStatSpecialty().Set(data.m_Stat_Specialty);
		GetStatHeatBuffer().Set(data.m_Stat_HeatBuffer);
		GetStatStamina().Set(data.m_Stat_Stamina);
		GetStatToxicity().Set(data.m_Stat_Toxicity);
		GetStatWater().Set(data.m_Stat_Water);
		GetStatEnergy().Set(data.m_Stat_Energy);
		GetStatBloodType().Set(data.m_BloodType);
		
		SetBloodType(data.m_BloodType);
		SetBloodTypeVisible(data.m_HasBloodTypeVisible);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(this.SetSynchDirty);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.SendUApiAfterLoadClient, 300);
	}
	
	void SendUApiAfterLoadClient(){
		RPCSingleParam(MAPLINK_AFTERLOADCLIENT, new Param1<bool>( true ), true, GetIdentity());
	}
	
	override void OnDisconnect(){
		//If the player has played less than 1 minutes just kill them so their data doesn't save to the local database
		if ( StatGet(AnalyticsManagerServer.STAT_PLAYTIME) <= 60){ 
			SetHealth("","", 0); 
		}
		super.OnDisconnect();
	}
	
	
	override void EEKilled( Object killer )
	{
		//Only save dead people who've been on the server for more than 1 minutes and who arn't tranfering
		if (m_TransferPoint == "" && StatGet(AnalyticsManagerServer.STAT_PLAYTIME) > 60){
			this.SavePlayerToUApi();
		}
		//If they are transfering delete or a fresh spawn just delete the body
		if ( (m_TransferPoint != "" || StatGet(AnalyticsManagerServer.STAT_PLAYTIME) <= 60 ) && ( !killer || killer == this )){
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.Delete, 300,false);
		}
		
		super.EEKilled( killer );
	}

	
	void UApiKillAndDeletePlayer(){
		SetAllowDamage(true);
		SetHealth("","", 0);
	}
	
	void UApiRequestTravel(string arrivalPoint, string serverName ){
		if (GetGame().IsClient()){
			RPCSingleParam(MAPLINK_REQUESTTRAVEL, new Param2<string, string>(arrivalPoint,  serverName), true, NULL);
		}
		if (GetGame().IsServer()){
			UApiDoTravel(arrivalPoint, serverName);
		}
	}
	
	protected void UApiDoTravel(string arrivalPoint, string serverName){
		UApiServerData serverData = UApiServerData.Cast( GetMapLinkConfig().GetServer( serverName ) );
		MapLinkDepaturePoint dpoint = MapLinkDepaturePoint.Cast( GetMapLinkConfig().GetDepaturePoint( GetPosition() ) );
		if (dpoint && serverData && dpoint.HasArrivalPoint(arrivalPoint) && GetIdentity()){
			this.UApiSaveTransferPoint(arrivalPoint);
			this.SavePlayerToUApi();
			Print("[MAPLINK] Do Travel Verified for " + GetIdentity().GetId() +  " Sending to Server: " + serverName  + " at ArrivalPoint: " + arrivalPoint );
			GetGame().AdminLog("[MAPLINK]  Player: " + GetIdentity().GetName() + " (" + GetIdentity().GetId() +  ") Sending to Server: " + serverName  + " at ArrivalPoint: " + arrivalPoint );
			GetRPCManager().SendRPC("MapLink", "RPCRedirectedKicked", new Param1<UApiServerData>(serverData), true, GetIdentity());
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.UApiKillAndDeletePlayer, 400, false);
		} else {
			string id = "NULL";
			if (GetIdentity()){
				id = GetIdentity().GetId();
			}
			Error("[MAPLINK] User " + id + " Tried to travel to " + arrivalPoint + " on " + serverName + " but validation failed");
		}
			
	} 
	
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);
		
		if (rpc_type == MAPLINK_AFTERLOADCLIENT && GetGame().IsClient()) {
			Param1<bool> alcdata;
			if (ctx.Read(alcdata))	{
				if (alcdata.param1){
					UApiAfterLoadClient();
				}
			}
		}
		if (rpc_type == MAPLINK_UNDERPROTECTION && GetGame().IsClient()) {
			Param1<int> updata;
			if (ctx.Read(updata))	{
				UpdateMapLinkProtectionClient(updata.param1);
			}
		}
		
		if ( rpc_type == MAPLINK_REQUESTTRAVEL && sender && GetIdentity() && GetGame().IsServer() ){
			Param2<string, string> rtdata;
			if (ctx.Read(rtdata) && GetIdentity().GetId() == sender.GetId())	{
				UApiDoTravel(rtdata.param1, rtdata.param2);
			}
		}
	}
	
	void UApiAfterLoadClient(){
		this.UpdateInventoryMenu();
	}
	
	
	
	override void SetSuicide(bool state)
	{
		super.SetSuicide(state);

		if (state && IsUnderMapLinkProtection() && GetGame().IsServer()){
			SetAllowDamage(true);
		}
	}
	
}

modded class DayZPlayerMeleeFightLogic_LightHeavy
{
    override bool HandleFightLogic(int pCurrentCommandID, HumanInputController pInputs, EntityAI pEntityInHands, HumanMovementState pMovementState, out bool pContinueAttack)
	{
        PlayerBase player = PlayerBase.Cast(m_DZPlayer);
        
        if (player)
        {
            if (player.IsUnderMapLinkProtection())
            return false;
        }

        return super.HandleFightLogic(pCurrentCommandID, pInputs, pEntityInHands, pMovementState, pContinueAttack);
    }
}

modded class ActionUnpin extends ActionSingleUseBase
{
    override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
        if (player.IsUnderMapLinkProtection())
            	return false;

		return super.ActionCondition(player, target, item);
	}
}