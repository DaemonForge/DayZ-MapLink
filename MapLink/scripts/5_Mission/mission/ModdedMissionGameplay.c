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
modded class MissionMainMenu {
	
	protected bool m_oldValue_NoCutscene;
	
	override void OnInit() {
		if ( GetDayZGame().IsToTransfer() ){
			m_oldValue_NoCutscene = m_NoCutscene;
			m_NoCutscene = true;
		}
		super.OnInit();
	}
	
	
	
	override void OnMissionStart(){
		super.OnMissionStart();
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.DoReconnect, 10);
	}

	
	
	override void OnMissionFinish(){
		super.OnMissionFinish();
		m_NoCutscene = m_oldValue_NoCutscene;
	}
	
	

	void DoReconnect() {
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(GetDayZGame().HiveDoReconnect, m_mainmenu);
	}
	
}