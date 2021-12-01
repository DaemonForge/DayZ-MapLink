modded class MissionServer extends MissionBase
{
	ref map<string, autoptr PlayerDataStore> m_PlayerDBQue = new map<string, autoptr PlayerDataStore>;
	string m_worldname;
	int MapLinkConfigRefreshTimer = 0;
	static int MAPLINK_CONFIG_REFRESH_TIME = 90000;
	
	override void OnMissionStart(){
		super.OnMissionStart();
		GetGame().GetWorldName(m_worldname);
		MLLog.Info("On Mission Start World: " + m_worldname + " Server: " + UApiConfig().ServerID);
	}
	
	
	void LoadPlayerFromUApiDB(int cid, int status, string oid, string data){	
      	if (status == UAPI_SUCCESS){  //If its a success
			PlayerDataStore dataload;
			if (UApiJSONHandler<PlayerDataStore>.FromString(data, dataload)){
				MLLog.Debug("LoadPlayerFromDB - Success ID:" + cid + " - GUID: " + oid );
				if (dataload.IsValid() && dataload.GUID == oid){
					MLLog.Info("Add Player to PlayerQue " + dataload.m_Name + "(" + dataload.GUID + ") Health:  " + dataload.m_Health + " PlayTime: " + dataload.m_TimeSurvivedValue + " Server: " + dataload.m_Server + " TransferPoint: " + dataload.m_TransferPoint);
					m_PlayerDBQue.Set(oid, PlayerDataStore.Cast(dataload));
				} else if (m_PlayerDBQue.Contains(oid)) { //This shouldn't be needed any more
					MLLog.Log("Player is dead or data invlaid Removing Player from Queue " + oid);
					m_PlayerDBQue.Remove(oid);
				}
				return;
			}
			MLLog.Err("LoadPlayerFromDB - Error Loading Data from the API - ID:" + cid + " - GUID: " + oid);
			return;
      	} 
		if (status == UAPI_EMPTY){
			MLLog.Info("LoadPlayerFromDB - Empty Response - ID:" + cid + " - GUID: " + oid );
			return;
		}
        MLLog.Err("LoadPlayerFromDB - API CALL FAILED - CHECK OVER YOUR CONFIGS AND ENSURE THAT THE API IS RUNNING");
	}

	override void OnClientPrepareEvent(PlayerIdentity identity, out bool useDB, out vector pos, out float yaw, out int preloadTimeout)
	{
		int CurrentTime = GetGame().GetTime();
		if (CurrentTime > MapLinkConfigRefreshTimer){
			MapLinkConfigRefreshTimer = CurrentTime + MAPLINK_CONFIG_REFRESH_TIME;
			GetMapLinkConfig().Load();
		}
		if (identity){
			int cid = UApi().db(PLAYER_DB).Load("MapLink", identity.GetId(), this, "LoadPlayerFromUApiDB");	
			MLLog.Info("Requesting Player Data from DataBase Call ID:" + cid + " - GUID: " + identity.GetId() );
			//NotificationSystem.SimpleNoticiation(" Requesting you're login player Data From the API", "Notification","Notifications/gui/data/notifications.edds", -16843010, 10, identity);
		} else {
			MLLog.Info("Requesting Player Data from DataBase - GUID: NULL");
		}
		super.OnClientPrepareEvent(identity, useDB, pos, yaw, preloadTimeout);
	}
	
	override void OnEvent(EventType eventTypeId, Param params) {		
		if (eventTypeId == ClientNewEventTypeID) {
			ClientNewEventParams newParams;
			Class.CastTo(newParams, params);
			//If the player was created, end if not spawn a new fresh spawn
			//Also need to spawn fresh spawns to be able to kick them with the redirect or they will get kick with a player not created message instead
			if (UApiOnClientNewEvent(newParams.param1, newParams.param2, newParams.param3)){ 
				MLLog.Info("Player " + PlayerIdentity.Cast(newParams.param1).GetId() +" Was Created from API");
				return;
			}
			MLLog.Info("Player " + PlayerIdentity.Cast(newParams.param1).GetId() + " Spawning Fresh");
				
		}
		super.OnEvent(eventTypeId, params);
	}
		
	bool UApiOnClientNewEvent(PlayerIdentity identity, vector pos, ParamsReadContext ctx)
	{
		PlayerDataStore playerdata;		
		if (identity && m_PlayerDBQue.Contains(identity.GetId()) &&  m_PlayerDBQue.Find(identity.GetId(), playerdata) && playerdata.IsValid()) {
			pos = "0 0 0";
			vector ori = "0 0 0";
			UApiServerData serverData;
			string transferPoint =  playerdata.m_TransferPoint;
			string FromServerName = playerdata.m_Server;
			
			if (!playerdata.IsAlive() || playerdata.IsUnconscious()){
				UApiServerData CurServerData = UApiServerData.Cast(GetMapLinkConfig().GetServer(UApiConfig().ServerID));
				if (CurServerData && CurServerData.RespawnServer && CurServerData.RespawnServer != "" && CurServerData.RespawnServer != UApiConfig().ServerID){
					serverData = UApiServerData.Cast(GetMapLinkConfig().GetServer(CurServerData.RespawnServer));
					if (serverData){
						NotificationSystem.Create(new StringLocaliser("Map Link"),new StringLocaliser(" Redirecting to the correct server - " + CurServerData.RespawnServer), "set:maplink_icons image:redirect", -16843010, 16, identity);
						GetRPCManager().SendRPC("MapLink", "RPCRedirectedKicked", new Param1<UApiServerData>(serverData), true, identity);
					}
				}
				MLLog.Info("Player " + identity.GetId() +" IsAlive: " + playerdata.IsAlive() + " IsUnconscious: " + playerdata.IsUnconscious() + " IsRestrained: " + playerdata.IsRestrained()  + " on the API, spawning them fresh");
				MLLog.Debug("Removing Player from Queue " + identity.GetId());
				m_PlayerDBQue.Remove(identity.GetId());
			    return false;
			}
			MLLog.Log("Spawning player " + identity.GetId() + " on: " + UApiConfig().ServerID + " World: " + m_worldname + " at " + transferPoint);
			if (FromServerName != UApiConfig().ServerID && transferPoint == "") {
				serverData = UApiServerData.Cast(GetMapLinkConfig().GetServer(playerdata.m_Server));
				NotificationSystem.Create(new StringLocaliser("Map Link"),new StringLocaliser(" Redirecting to the correct server - " + FromServerName), "set:maplink_icons image:redirect", -16843010, 16, identity);
				GetRPCManager().SendRPC("MapLink", "RPCRedirectedKicked", new Param1<UApiServerData>(serverData), true, identity);
				MLLog.Info("Player " + identity.GetId() + " Redirected to correct server " +  FromServerName);
				MLLog.Debug("Removing Player from Queue " + identity.GetId());
				m_PlayerDBQue.Remove(identity.GetId());
				return false;
			}
			if (FromServerName != UApiConfig().ServerID && transferPoint != "") {
				MapLinkSpawnPointPos pointPos;
				if (!Class.CastTo(pointPos, GetMapLinkConfig().SpawnPointPos(transferPoint))){
					serverData = UApiServerData.Cast(GetMapLinkConfig().GetServer(FromServerName));
					NotificationSystem.Create(new StringLocaliser("Map Link"),new StringLocaliser(" Error On Connect This server isn't set up correctly sending you back to your orginal server - " + playerdata.m_Server), "set:maplink_icons image:redirect", -16843010, 16, identity);
					GetRPCManager().SendRPC("MapLink", "RPCRedirectedKicked", new Param1<UApiServerData>(serverData), true, identity);
					MLLog.Err("Server isn't set up to receive this arrival point(" + transferPoint + ") Player " + identity.GetId() + " Redirected back to previous server " +  playerdata.m_Server);
					
					MLLog.Debug("Removing Player from Queue " + identity.GetId());
					m_PlayerDBQue.Remove(identity.GetId());
					return false;
				}
				pos = pointPos.Get();
				ori = pointPos.GetOrientation();
			}
			PlayerBase player = PlayerBase.Cast(PlayerDataStore.Cast(playerdata).CreateWithIdentity(PlayerIdentity.Cast(identity), pos));
			GetGame().SelectPlayer(identity, player);
			InvokeOnConnect(player, identity);
			SyncEvents.SendPlayerList();
			ControlPersonalLight(player);
			SyncGlobalLighting(player);
			PlayerDataStore.Cast(playerdata).SetupPlayer(player, pos, ori);
			if (FromServerName != UApiConfig().ServerID && transferPoint != "") {
				int protectionTime = GetMapLinkConfig().GetProtectionTime(transferPoint);
				MLLog.Info("Adding Protection to " + playerdata.GUID + "  at " + transferPoint + " for " + protectionTime);
				if (protectionTime > 0){
					GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(player.UpdateMapLinkProtection, protectionTime);
				}
			}
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(player.SavePlayerToUApi, 100);
			MLLog.Debug("Removing Player from Queue " + identity.GetId());
			m_PlayerDBQue.Remove(identity.GetId());
			return true;
		}
		return false;
	}
	
	override void HandleBody(PlayerBase player)
	{
		if ( player && player.IsBeingTransfered() ){
			MLLog.Debug("HandleBody IsBeingTransfered Killing Player"); //Fail Safe
			player.SetAllowDamage(true);
			player.SetHealth("", "Health", 0);
			player.SetHealth("", "", 0);
		}
		super.HandleBody( player );
		if (player && ( player.MapLinkShoudDelete() || player.IsBeingTransfered() ) ) {
			MLLog.Debug("HandleBody IsBeingTransfered Removing body");
			// remove the body
			player.Delete();	
		}
	}
}