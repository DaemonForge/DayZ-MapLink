modded class MissionServer extends MissionBase
{
	ref map<string, ref PlayerDataStore> m_PlayerDBQue = new map<string, ref PlayerDataStore>;
	string m_worldname;
	
	void MissionServer(){
		GetRPCManager().AddRPC( "TheHive", "RPCDisableLoadFromApi", this, SingeplayerExecutionType.Both );
	}
	
	override void OnMissionStart(){
		super.OnMissionStart();
		GetGame().GetWorldName(m_worldname);
		Print("[UAPI] On Mission Start " + m_worldname);
	}
	
	void RPCDisableLoadFromApi( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target ) {
		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		if (players){
			foreach (Man plr : players){
				PlayerBase player = PlayerBase.Cast(plr);
				if (player && player.GetIdentity() && player.GetIdentity().GetId() == sender.GetId()){
					if (player.ToggleDontLoadFromApi()){
						NotificationSystem.SimpleNoticiation(" Disabling Load from API", "Notification","Notifications/gui/data/notifications.edds", -16843010, 10, sender);
					} else {
						NotificationSystem.SimpleNoticiation(" Enabling Load from API", "Notification","Notifications/gui/data/notifications.edds", -16843010, 10, sender);
					}
					return;
				}	
			}
		}
	}
	
	void LoadPlayerFromUApiDB(int cid, int status, string oid, string data){	
      	if (status == UAPI_SUCCESS && data != "" && data != "{}" && data != "{ }"){  //If its a success
			Print("[UAPI] LoadPlayerFromDB - Success ID:" + cid + " - GUID: " + oid );
			PlayerDataStore dataload;
			if (UApiJSONHandler<PlayerDataStore>.FromString(data, dataload)){
				Print("[UAPI] UApiJSONHandler - Success");
				if (dataload.IsValid() && dataload.GUID == oid){
					Print("[UAPI] UApiJSONHandler - IsValid");
					m_PlayerDBQue.Set(oid, PlayerDataStore.Cast(dataload));
				} else if (m_PlayerDBQue.Contains(oid)){
					Print("[UAPI] m_PlayerDBQue - Remove");
					m_PlayerDBQue.Remove(oid);
				}
			}
      	} else { //could do else if to catch different kinds of errors
         	Print("[UAPI] api call failed");
      	}
	}

	override void OnClientPrepareEvent(PlayerIdentity identity, out bool useDB, out vector pos, out float yaw, out int preloadTimeout)
	{
		if (identity){
			Print("[UAPI] OnClientPrepareEvent  - " + preloadTimeout + " - Identity = " + identity.GetId());
			int cid = UApi().db(PLAYER_DB).Load("TheHive", identity.GetId(), this, "LoadPlayerFromUApiDB");	
			Print("[UAPI] db Load - ID:" + cid + " - GUID: " + identity.GetId() );
			//NotificationSystem.SimpleNoticiation(" Requesting you're login player Data From the API", "Notification","Notifications/gui/data/notifications.edds", -16843010, 10, identity);
		} else {
			Print("[UAPI] OnClientPrepareEvent - Identity = NULL");
		}
		if (GetHive())
		{
			// use character from database
			useDB = true;
		}
		else
		{
			// use following data without database
			useDB = false;
			pos = "1189.3 0.0 5392.48";
			yaw = 0;
		}
	}
	
	override void OnEvent(EventType eventTypeId, Param params) {
		
		if (eventTypeId == ClientPrepareEventTypeID){Print("[UAPI] OnEvent ClientPrepareEventTypeID");}
		if (eventTypeId == ClientNewEventTypeID){Print("[UAPI] OnEvent ClientNewEventTypeID");}
		if (eventTypeId == ClientReadyEventTypeID){Print("[UAPI] OnEvent ClientReadyEventTypeID");}
		if (eventTypeId == ClientRespawnEventTypeID){Print("[UAPI] OnEvent ClientRespawnEventTypeID");}	
		if (eventTypeId == ClientReconnectEventTypeID){Print("[UAPI] OnEvent ClientReconnectEventTypeID");}
		if (eventTypeId == ClientDisconnectedEventTypeID){Print("[UAPI] OnEvent ClientDisconnectedEventTypeID");}
		if (eventTypeId == LogoutCancelEventTypeID){Print("[UAPI] OnEvent LogoutCancelEventTypeID");}
		
		if (eventTypeId == ClientNewEventTypeID) {
			ClientNewEventParams newParams;
			Class.CastTo(newParams, params);
			if (m_PlayerDBQue.Contains(PlayerIdentity.Cast(newParams.param1).GetId()) && m_PlayerDBQue.Get(PlayerIdentity.Cast(newParams.param1).GetId()).IsValid()){
				UApiOnClientNewEvent(newParams.param1, newParams.param2, newParams.param3);
				return;
			}
		}
		super.OnEvent(eventTypeId, params);
	}
	
	//Engine Wait
	
	bool UApiOnClientNewEvent(PlayerIdentity identity, vector pos, ParamsReadContext ctx)
	{
		Print("[UAPI] UApiOnClientNewEvent - " + identity.GetId());
		PlayerDataStore playerdata;		
		if (identity && m_PlayerDBQue.Find(identity.GetId(), playerdata) && playerdata.IsValid()){
			pos = "0 0 0";
			Print("Spawning player on " + m_worldname);
			if (playerdata.m_Map != m_worldname && playerdata.m_TransferPoint == ""){
				UApiServerData serverData;
				if (playerdata.m_Map == "enoch"){
					serverData = new UApiServerData("192.95.50.50", 2632);
				}
				if (playerdata.m_Map == "chernarusplus"){
					serverData = new UApiServerData("192.95.50.50", 2602);
					
				}
				GetRPCManager().SendRPC("TheHive", "RPCRedirectedKicked", new Param1<UApiServerData>(serverData), true, identity);
				return false;
			} 
			if (playerdata.m_TransferPoint != ""){
				pos = GetDayZGame().HiveGetSpawnLocation(m_worldname, playerdata.m_TransferPoint);
			}
			Print("[UAPI] OnClientNewEvent - CreateWithIdentity - " + identity.GetId());
			PlayerBase player = PlayerBase.Cast(PlayerDataStore.Cast(playerdata).CreateWithIdentity(PlayerIdentity.Cast(identity), pos));
			Print("[UAPI] Identity Being Selected: " + identity.GetId());
			GetGame().SelectPlayer(identity, player);
			InvokeOnConnect(player, identity);
			SyncEvents.SendPlayerList();
			ControlPersonalLight(player);
			SyncGlobalLighting(player);
			PlayerDataStore.Cast(playerdata).SetupPlayer(player, pos);
			Print("[UAPI] m_PlayerDBQue - Remove");
			m_PlayerDBQue.Remove(identity.GetId());
			return true;
		}
		return false;
	}
	
	override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
	{
		super.InvokeOnConnect(player, identity);
		Print("[UAPI] InvokeOnConnect - " + identity.GetId());
	}
}