modded class MissionGameplay
{
	
	void MissionGameplay()
    {   
    }
	
	override void UFrameworkReadyTokenReceived(){
		
		super.UFrameworkReadyTokenReceived();
	}
	
	
}

modded class MissionMainMenu {
		
	static bool MAPLINK_DISABLECUTSCENE_ONTRANSFER = true;
	
	override void OnInit() {
		if ( GetDayZGame().IsToTransfer() && MAPLINK_DISABLECUTSCENE_ONTRANSFER){
			m_NoCutscene = true;
		}
		super.OnInit();
	}
	
	
	
	override void OnMissionStart(){
		super.OnMissionStart();
		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Call(this.DoReconnect);
	}

	
	
	override void OnMissionFinish(){
		super.OnMissionFinish();
	}
	
	

	void DoReconnect() { 
		GetDayZGame().HiveDoReconnect(m_mainmenu);
	}
	
}