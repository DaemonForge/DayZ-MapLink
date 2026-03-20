modded class PlayerBase extends ManBase {
	
	protected autoptr Timer m_MapLink_UnderProtectionTimer;
	
	// --- Merged from _MapLinkBase ---
	override void Init()
	{
		super.Init();
		RegisterNetSyncVariableBool("m_MapLink_UnderProtection");
	}
	
	override void OnPlayerLoaded()
	{
		super.OnPlayerLoaded();
		if (GetIdentity()){
			m_MapLinkGUIDCache = GetIdentity().GetId();
			m_MapLinkNameCache = GetIdentity().GetName();
		}
	}
	

			
	protected void UpdateMapLinkProtectionClient(int time){
		MLLog.Debug("UpdateMapLinkProtectionClient" + time);
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
		MLLog.Debug("UpdateMapLinkProtection" + time);
		if (!g_Game.IsServer()){return;}
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
		#ifdef VPPADMINTOOLS
			if ( g_Game.IsServer() && hasGodmode ){
				MLLog.Log("RemoveProtectionSafe VPP ADMIN TOOLS ACTIVE");
				PlayerHasGodMode = true;
			}
		#endif
		#ifdef ZOMBERRY_AT
			if ( g_Game.IsServer() && ZBGodMode ){
				MLLog.Log("RemoveProtectionSafe ZOMBERRY ADMIN TOOLS ACTIVE");
				PlayerHasGodMode = true;
			}
		#endif
		#ifdef TRADER 
			if (g_Game.IsServer() && m_Trader_IsInSafezone){
				MLLog.Log("RemoveProtectionSafe Player Is In Trader(DrJones) Safe Zone");
				PlayerHasGodMode = true;
			}
		#endif
		#ifdef TRADERPLUS
			if (g_Game.IsServer() && IsInsideSZ && IsInsideSZ.SZStatut){
				MLLog.Log("RemoveProtectionSafe Player Is In Trader+ Safe Zone");
				PlayerHasGodMode = true;
			}		
		#endif
		if (!PlayerHasGodMode && g_Game.IsServer() ){
			SetAllowDamage(true);
		}
		m_MapLink_UnderProtection = false;
		SetSynchDirty();
	}
	
	
    override void OnStoreSave(ParamsWriteContext ctx)
    {
        super.OnStoreSave(ctx);
		//Making sure not to save freshspawns or dead people, dead people logic is handled in EEKilled
		if (!g_Game.IsClient() && GetHealth("","Health") > 0 && StatGet(AnalyticsManagerServer.STAT_PLAYTIME) >= MAPLINK_BODYCLEANUPTIME && !IsBeingTransferred() && !MapLinkShouldDelete()){
			StatUpdateByTime( AnalyticsManagerServer.STAT_PLAYTIME );
			this.SavePlayerToU();
			//Print("[UF] Would be saving player but not!");
		}
    }
	
	void SavePlayerToU(){
		if (m_MapLinkGUIDCache && m_MapLinkNameCache && g_Game.IsServer()){
			MLLog.Debug("Saving Player to API " + m_MapLinkNameCache + "(" + m_MapLinkGUIDCache + ")" + " Health:  " + GetHealth("","Health") + " PlayTime: " +  StatGet(AnalyticsManagerServer.STAT_PLAYTIME) + " IsUnconscious: " + IsUnconscious() + " IsRestrained: " + IsRestrained() );
			autoptr PlayerDataStore teststore = new PlayerDataStore(PlayerBase.Cast(this));
			U().db(PLAYER_DB).Save("MapLink", m_MapLinkGUIDCache, teststore.ToJson());
			if (IsAlive() && !IsUnconscious()){
				U().db(PLAYER_DB).PublicSave("MapLink", m_MapLinkGUIDCache, SimpleValueStore.StoreValue(UFConfig().ServerID + "~" + m_TransferPoint),NULL,"");
			} else {
				U().db(PLAYER_DB).PublicSave("MapLink", m_MapLinkGUIDCache, SimpleValueStore.StoreValue(""),NULL,"");
			}
			MLLog.Debug("Completed Saving Player to API " + m_MapLinkNameCache + "(" + m_MapLinkGUIDCache + ")");
			//NotificationSystem.SimpleNoticiation(" You're Data has been saved to the API", "Notification","Notifications/gui/data/notifications.edds", -16843010, 10, this.GetIdentity());
		} else {
			MLLog.Debug("Failed to save player to API");
		}
		
	}
	
	
	void OnUFSave( PlayerDataStore data){
		int i = 0;
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
			GetBleedingManagerServer().OnUFSave(data);
		} else {
			MLLog.Log("Bleeding Manager is NULL");
		}
		if (m_PlayerStomach){
			for (i = 0; i < m_PlayerStomach.m_StomachContents.Count(); i++){
				StomachItem stomachItem;
				if (Class.CastTo(stomachItem, m_PlayerStomach.m_StomachContents.Get(i))){
					data.AddStomachItem(stomachItem.m_Amount, stomachItem.m_FoodStage, stomachItem.m_ClassName, stomachItem.m_Agents);
				}
			}
		}
		if (GetPlayerStats()){
			for (i = 0; i < GetPlayerStats().GetPCO().m_PlayerStats.Count(); i++){
				PlayerStatBase TheStat = PlayerStatBase.Cast(GetPlayerStats().GetPCO().m_PlayerStats.Get(i));
				if (TheStat && TheStat.GetLabel() != ""){
					data.AddStat(TheStat.GetLabel(), TheStat.Get());
				}
			}
		}
		data.m_Camera3rdPerson = m_Camera3rdPerson;
	}
	
	void OnUFLoad( PlayerDataStore data){
		int i = 0;
		
		for (i = 0; i < GetPlayerStats().GetPCO().Get().Count(); i++){
			PlayerStatBase TheStat;
			float statvalue;
			if (Class.CastTo(TheStat, GetPlayerStats().GetPCO().Get().Get(i)) && data.ReadStat(TheStat.GetLabel(), statvalue)){
				TheStat.SetByFloatEx(statvalue);
			} else if (TheStat) {
				MLLog.Err("Failed to set stat for " + TheStat.GetLabel());
			}
		}
		
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
			GetBleedingManagerServer().OnUFLoad(data);
		} else {
			MLLog.Log("Bleeding Manager is NULL");
		}
		if (m_PlayerStomach && data.m_Stomach){
			for (i = 0; i < data.m_Stomach.Count(); i++){
				UStomachItem stomachItem;
				if (Class.CastTo(stomachItem, data.m_Stomach.Get(i))){
					m_PlayerStomach.AddToStomach(stomachItem.m_ClassName, stomachItem.m_Amount, stomachItem.m_FoodStage, stomachItem.m_Agents );
				}
			}
		}	
		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Call(this.SetSynchDirty);
		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.SendUFAfterLoadClient, 200);
		m_Camera3rdPerson = data.m_Camera3rdPerson && !g_Game.GetWorld().Is3rdPersonDisabled();
	}
	
	void SendUFAfterLoadClient(){
		RPCSingleParam(MAPLINK_AFTERLOADCLIENT, new Param2<bool, bool>( true, m_Camera3rdPerson ), true, GetIdentity());
	}
	
	override void OnDisconnect(){
		StatUpdateByTime( AnalyticsManagerServer.STAT_PLAYTIME );
		//If the player has played less than 1 minutes just kill them so their data doesn't save to the local database
		if ( StatGet(AnalyticsManagerServer.STAT_PLAYTIME) <= MAPLINK_BODYCLEANUPTIME){ 
			if (GetIdentity()){
				MLLog.Info("OnDisconnect Player: " + GetIdentity().GetName() + " (" + GetIdentity().GetId() +  ") they are fresh spawn PlayTime: " + StatGet(AnalyticsManagerServer.STAT_PLAYTIME));
			} else {
				MLLog.Info("OnDisconnect Player: NULL (NULL) they are fresh spawn PlayTime: " + StatGet(AnalyticsManagerServer.STAT_PLAYTIME));
			}
			SetAllowDamage(true);
			SetHealth("","", 0); 
		}
		//If the player has played less than 1 minutes just kill them so their data doesn't save to the local database
		if ( IsBeingTransferred()){ 
			if (GetIdentity()){
				MLLog.Info("OnDisconnect Player: " + GetIdentity().GetName() + " (" + GetIdentity().GetId() +  ") Is Transfering");
			} else {
				MLLog.Info("OnDisconnect Player: NULL (NULL)  Is Transfering");
			}
			SetAllowDamage(true);
			SetHealth("","", 0); 
		}
		super.OnDisconnect();
	}
	
	
	override void EEKilled( Object killer )
	{
		
		//Only save dead people who've been on the server for more than 1 minutes and who arn't tranfering
		StatUpdateByTime( AnalyticsManagerServer.STAT_PLAYTIME );
		if ( ( !IsBeingTransferred() && StatGet(AnalyticsManagerServer.STAT_PLAYTIME) > MAPLINK_BODYCLEANUPTIME ) || ( killer && killer != this )){
			this.SavePlayerToU();
			m_MLPlayerStoreCache.Remove(GetIdentity().GetId());
		}
		//If they are transfering delete
		if ( IsBeingTransferred()  && ( !killer || killer == this )){
			if (GetIdentity()){
				MLLog.Info("Marking Player: " + GetIdentity().GetName() + " (" + GetIdentity().GetId() +  ") for delete cause of transfer" );
			} else {
				MLLog.Info("Marking Player: NULL (NULL)  for delete cause of transfer" );
			}
			//g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.Delete, 400, false);
			SetPosition(vector.Zero);
			m_MapLink_ShouldDelete = true;
		}
		
		//Fresh spawn just delete the body since I have to spawn players in to send notifications about player transfers
		if ( !IsBeingTransferred() && StatGet(AnalyticsManagerServer.STAT_PLAYTIME) <= MAPLINK_BODYCLEANUPTIME && ( !killer || killer == this )){
			if (GetIdentity()){
				MLLog.Info("Deleteing Player: " + GetIdentity().GetName() + " (" + GetIdentity().GetId() +  ") cause they are fresh spawn PlayTime: " + StatGet(AnalyticsManagerServer.STAT_PLAYTIME));
			} else {
				MLLog.Info("Deleteing Player: NULL (NULL) cause they are fresh spawn PlayTime: " + StatGet(AnalyticsManagerServer.STAT_PLAYTIME));
			}
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.Delete, 400, false);
			SetPosition(vector.Zero);
			m_MapLink_ShouldDelete = true;
		}
		super.EEKilled( killer );
	}

		
	override void OnUnconsciousStart()
	{
		super.OnUnconsciousStart();
		SavePlayerToU();
	}
	
	override void OnUnconsciousStop(int pCurrentCommandID)
	{		
		super.OnUnconsciousStop(pCurrentCommandID);
		SavePlayerToU();
	}
	
	void MapLinkUpdateClientSettingsToServer(){
		if (g_Game.IsClient()){
			RPCSingleParam(MAPLINK_UPDATE3RDPERSON, new Param1<bool>(m_Camera3rdPerson), true, NULL);
		}
	}
	
	void UFKillAndDeletePlayer(){
		if (GetIdentity()){
			MLLog.Info("Killing for transfering Player: " + GetIdentity().GetName() + " (" + GetIdentity().GetId() +  ")" );
		} else{
			MLLog.Info("Killing for transfering Player: NULL (NULL)" );
		}
		SetAllowDamage(true);
		SetHealth("","", 0);
	}
	
	void MLRequestTravel(string arrivalPoint, string serverName ){
		if (g_Game.IsClient()){
			MLLog.Debug("Player: " + GetIdentity().GetId() + " is requesting to travel to " + arrivalPoint + " on Server: " + serverName);
			RPCSingleParam(MAPLINK_REQUESTTRAVEL, new Param2<string, string>(arrivalPoint,  serverName), true, NULL);
		}
		if (g_Game.IsServer()){
			MLLog.Debug("Player: " + GetIdentity().GetName() + "("+ GetIdentity().GetId() + ") is requesting to travel to " + arrivalPoint + " on Server: " + serverName);
			UFDoTravel(arrivalPoint, serverName);
		}
	}
	
	bool UFDoManualServerTravel(string arrivalPoint, UServerData serverData, int cost = 0, int id = 0){
		string pid = "NULL";
		if (GetIdentity()){
			pid = GetIdentity().GetId();
		}
		if (!serverData){
			MLLog.Err("Manual Travel of user " + pid + " to " + arrivalPoint + " failed NULL Server Data");
			return false;
		}
		// Check blacklist before allowing transfer
		string blacklistedItem = serverData.FindFirstBlacklistedItem(PlayerBase.Cast(this));
		if (blacklistedItem != ""){
			string displayName = serverData.GetItemDisplayName(blacklistedItem);
			MLLog.Info("Player " + pid + " denied transfer to " + serverData.Name + " - blacklisted item: " + blacklistedItem);
			if (GetIdentity()){
				MLNotification.Send(GetIdentity(), "MapLink - Transfer Blocked", "You cannot transfer to " + serverData.Name + " because '" + displayName + "' is not allowed on that server.", "set:maplink_icons image:status");
			}
			return false;
		}
		if (serverData && GetIdentity() && (cost <= 0 || UGetPlayerBalance(GetMapLinkConfig().GetCurrencyKey(id)) >= cost)){
			URemoveMoney(GetMapLinkConfig().GetCurrencyKey(id),cost);
			this.UFSaveTransferPoint(arrivalPoint);
			this.SavePlayerToU();
			MLLog.Info("Player: " + GetIdentity().GetName() + " (" + GetIdentity().GetId() +  ") Sending to Server: " + serverData.Name + "(" + serverData.IP  + ":" + serverData.Port.ToString() + ") at ArrivalPoint: " + arrivalPoint );
			UServerData.SendRedirectRPC(serverData, GetIdentity());
			SetAllowDamage(false);
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.UFKillAndDeletePlayer, 350, false);
			return true;
		}
		MLLog.Err("Manual Travel of user " + pid + " to " + arrivalPoint + " on " + serverData.Name + "(" + serverData.IP  + ":" + serverData.Port.ToString() + ") failed");
		return false;
	}
	
	protected bool UFDoTravel(string arrivalPoint, string serverName){
		UServerData serverData = UServerData.Cast( GetMapLinkConfig().GetServer( serverName ) );
		MapLinkDepaturePoint dpoint = MapLinkDepaturePoint.Cast( GetMapLinkConfig().GetDepaturePoint( GetPosition() ) );
		int cost;
		int id;
		if (dpoint && serverData && dpoint.GetArrivalPointData(arrivalPoint, id, cost)) {
			// Check blacklist before allowing transfer
			string blacklistedItem = serverData.FindFirstBlacklistedItem(PlayerBase.Cast(this));
			if (blacklistedItem != ""){
				string displayName = serverData.GetItemDisplayName(blacklistedItem);
				string pid_bl = "NULL";
				if (GetIdentity()){
					pid_bl = GetIdentity().GetId();
				}
				MLLog.Info("Player " + pid_bl + " denied transfer to " + serverName + " - blacklisted item: " + blacklistedItem);
				if (GetIdentity()){
					MLNotification.Send(GetIdentity(), "MapLink - Transfer Blocked", "You cannot transfer to " + serverName + " because '" + displayName + "' is not allowed on that server.", "set:maplink_icons image:status");
				}
				return false;
			}
			MLLog.Debug( "Working with Currency Key: " + GetMapLinkConfig().GetCurrencyKey(id) );
			if ( GetIdentity() && (cost <= 0 || UGetPlayerBalance(GetMapLinkConfig().GetCurrencyKey(id)) >= cost)){
				URemoveMoney(GetMapLinkConfig().GetCurrencyKey(id),cost);
				this.UFSaveTransferPoint(arrivalPoint);
				this.SavePlayerToU();
				MLLog.Info("Player: " + GetIdentity().GetName() + " (" + GetIdentity().GetId() +  ") Sending to Server: " + serverName  + " at ArrivalPoint: " + arrivalPoint );
				UServerData.SendRedirectRPC(serverData, GetIdentity());
				SetAllowDamage(false);
				g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.UFKillAndDeletePlayer, 350, false);
				return true;
			}
		}
		string pid = "NULL";
		if (GetIdentity()){
			pid = GetIdentity().GetId();
		}
		MLLog.Err("User " + pid + " Tried to travel to " + arrivalPoint + " on " + serverName + " but validation failed");
		return false;
	} 
	
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);
		
		if (rpc_type == MAPLINK_AFTERLOADCLIENT && g_Game.IsClient()) {
			Param2<bool, bool> alcdata;
			if (ctx.Read(alcdata))	{
				if (alcdata.param1){
					UFAfterLoadClient();
					m_Camera3rdPerson = alcdata.param2 && !g_Game.GetWorld().Is3rdPersonDisabled();
				}
			}
		}
		if (rpc_type == MAPLINK_UNDERPROTECTION && g_Game.IsClient()) {
			Param1<int> updata;
			if (ctx.Read(updata))	{
				UpdateMapLinkProtectionClient(updata.param1);
			}
		}
		
		if ( rpc_type == MAPLINK_REQUESTTRAVEL && sender && GetIdentity() && g_Game.IsServer() ){
			Param2<string, string> rtdata;
			if (ctx.Read(rtdata) && GetIdentity().GetId() == sender.GetId())	{
				UFDoTravel(rtdata.param1, rtdata.param2);
			}
		}
		
		if ( rpc_type == MAPLINK_UPDATE3RDPERSON && sender && GetIdentity() && g_Game.IsServer() ) {
			Param1<bool> trdpdata;
			if ( ctx.Read(trdpdata) && GetIdentity().GetId() == sender.GetId()) {
				m_Camera3rdPerson = trdpdata.param1;
			}
		}

	}
	
	void UFAfterLoadClient(){
		this.UpdateInventoryMenu();
	}
	
	
	
	override void SetSuicide(bool state)
	{
		super.SetSuicide(state);

		if (state && IsUnderMapLinkProtection() && g_Game.IsServer()){
			SetAllowDamage(true);
		}
	}
	
	
	override void SetActions(out TInputActionMap InputActionMap) {
		
		super.SetActions(InputActionMap);
		AddAction(ActionMapLinkOpenOnAny, InputActionMap);
		
	}
	
	bool FindDepaturePointForEntity(EntityAI entity, out MapLinkDepaturePoint depaturePoint){
		if (!entity)
			return false;
		
		if (!depaturePoint){ //So you can use super here and if the point is set don't set it.
			if (GetMapLinkConfig().IsDepaturePoint(entity.GetType(), entity.GetPosition())){
				depaturePoint = MapLinkDepaturePoint.Cast(GetMapLinkConfig().GetDepaturePointAny(entity.GetType(), entity.GetPosition()));
				return true;
			}
			return false;
		}
		return true;
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