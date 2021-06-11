modded class MissionServer extends MissionBase
{
	ref map<string, ref PlayerDataStore> m_PlayerDBQue = new map<string, ref PlayerDataStore>;
	string m_worldname;
	int MapLinkConfigRefreshTimer = 0;
	static int MAPLINK_CONFIG_REFRESH_TIME = 90000;
	
	override void OnMissionStart(){
		super.OnMissionStart();
		GetGame().GetWorldName(m_worldname);
		Print("[UAPI] On Mission Start World: " + m_worldname + " Server: " + UApiConfig().ServerID);
	}
	
	
	void LoadPlayerFromUApiDB(int cid, int status, string oid, string data){	
      	if (status == UAPI_SUCCESS){  //If its a success
			PlayerDataStore dataload;
			if (UApiJSONHandler<PlayerDataStore>.FromString(data, dataload)){
				Print("[UAPI] LoadPlayerFromDB - Success ID:" + cid + " - GUID: " + oid );
				if (dataload.IsValid() && dataload.GUID == oid){
					m_PlayerDBQue.Set(oid, PlayerDataStore.Cast(dataload));
				} else if (m_PlayerDBQue.Contains(oid)) { //This shouldn't be needed any more
					m_PlayerDBQue.Remove(oid);
				}
				return;
			}
			Print("[UAPI] LoadPlayerFromDB - Error Loading Data from the API - ID:" + cid + " - GUID: " + oid);
      	} 
		if (status == UAPI_EMPTY){
			Print("[UAPI] LoadPlayerFromDB - Empty Response - ID:" + cid + " - GUID: " + oid );
			return;
		}
        Error("[UAPI] LoadPlayerFromDB - API CALL FAILED - CHECK OVER YOUR CONFIGS AND ENSURE THAT THE API IS RUNNING");
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
			Print("[UAPI] DB Load - ID:" + cid + " - GUID: " + identity.GetId() );
			//NotificationSystem.SimpleNoticiation(" Requesting you're login player Data From the API", "Notification","Notifications/gui/data/notifications.edds", -16843010, 10, identity);
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
				Print("[UAPI] Player " + PlayerIdentity.Cast(newParams.param1).GetId() +" Was Created from API");
				return;
			}
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
			
			if (!playerdata.IsAlive()){
				UApiServerData CurServerData = UApiServerData.Cast(GetMapLinkConfig().GetServer(UApiConfig().ServerID));
				if (CurServerData && CurServerData.RespawnServer && CurServerData.RespawnServer != "" && CurServerData.RespawnServer != UApiConfig().ServerID){
					serverData = UApiServerData.Cast(GetMapLinkConfig().GetServer(CurServerData.RespawnServer));
					if (serverData){
						NotificationSystem.Create(new StringLocaliser("Map Link"),new StringLocaliser(" Redirecting to the correct server - " + CurServerData.RespawnServer), "set:maplink_icons image:redirect", -16843010, 16, identity);
						GetRPCManager().SendRPC("MapLink", "RPCRedirectedKicked", new Param1<UApiServerData>(serverData), true, identity);
					}
				}
				m_PlayerDBQue.Remove(identity.GetId());
			    Print("[UAPI] Player " + identity.GetId() +" dead on the API, spawning them fresh");
				return false;
			}
			Print("[UAPI] Spawning player " + identity.GetId() + " on: " + UApiConfig().ServerID + " World: " + m_worldname + " at " + transferPoint);
			if (FromServerName != UApiConfig().ServerID && transferPoint == "") {
				serverData = UApiServerData.Cast(GetMapLinkConfig().GetServer(playerdata.m_Server));
				NotificationSystem.Create(new StringLocaliser("Map Link"),new StringLocaliser(" Redirecting to the correct server - " + FromServerName), "set:maplink_icons image:redirect", -16843010, 16, identity);
				GetRPCManager().SendRPC("MapLink", "RPCRedirectedKicked", new Param1<UApiServerData>(serverData), true, identity);
				Print("[UAPI] Player " + identity.GetId() + " Redirected to correct server " +  FromServerName);
				m_PlayerDBQue.Remove(identity.GetId());
				return false;
			}
			if (FromServerName != UApiConfig().ServerID && transferPoint != "") {
				MapLinkSpawnPointPos pointPos;
				if (!Class.CastTo(pointPos, GetMapLinkConfig().SpawnPointPos(transferPoint))){
					serverData = UApiServerData.Cast(GetMapLinkConfig().GetServer(FromServerName));
					NotificationSystem.Create(new StringLocaliser("Map Link"),new StringLocaliser(" Error On Connect This server isn't set up correctly sending you back to your orginal server - " + playerdata.m_Server), "set:maplink_icons image:redirect", -16843010, 16, identity);
					GetRPCManager().SendRPC("MapLink", "RPCRedirectedKicked", new Param1<UApiServerData>(serverData), true, identity);
					m_PlayerDBQue.Remove(identity.GetId());
					Print("[UAPI] Error Server isn't set up to receive this arrival point Player " + identity.GetId() + " Redirected back to previous server " +  playerdata.m_Server);
					
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
				Print("[MAPLINK] UApiOnClientNewEvent - UpdateMapLinkProtection - " + transferPoint + " - " + protectionTime);
				if (protectionTime > 0){
					GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(player.UpdateMapLinkProtection, protectionTime);
				}
			}
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(player.SavePlayerToUApi, 100);
			m_PlayerDBQue.Remove(identity.GetId());
			return true;
		}
		return false;
	}
}