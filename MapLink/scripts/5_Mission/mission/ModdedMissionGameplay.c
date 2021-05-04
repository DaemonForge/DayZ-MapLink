modded class MissionGameplay
{

	void MissionGameplay()
    {   
		GetRPCManager().AddRPC( "MapLink", "RPCRedirectedKicked", this, SingeplayerExecutionType.Both );
    }


	void RPCRedirectedKicked( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target ) {
		
		Param1< UApiServerData > data; 
		if ( !ctx.Read( data ) ) return;
		Print("[MAPLINK] Kicked from Game");
		UApiServerData serverData = UApiServerData.Cast(data.param1);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(GetGame().DisconnectSessionForce);
		GetDayZGame().HiveSetReconnectTo(serverData);
	}
	
	
}
modded class MissionMainMenu{
	
	override void OnMissionStart(){
		super.OnMissionStart();
		Print("MissionMainMenu - OnMissionStart");
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.DoReconnect, 10);
	}

	void DoReconnect() {
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(GetDayZGame().HiveDoReconnect, m_mainmenu);
	}
	
}