modded class MissionGameplay
{

	void MissionGameplay()
    {   
		GetRPCManager().AddRPC( "MapLink", "RPCRedirectedKicked", this, SingeplayerExecutionType.Both );
    }


	void RPCRedirectedKicked( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target ) {
		
		Param1< UApiServerData > data; 
		if ( !ctx.Read( data ) ) return;
		Print("[UPAI] Kicked from Game");
		UApiServerData serverData = UApiServerData.Cast(data.param1);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetGame().DisconnectSessionForce, 50);
		if (serverData){
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetDayZGame().HiveReconnectTo, 500, false, serverData);
		}
	}
	
}