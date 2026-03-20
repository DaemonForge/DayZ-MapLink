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

	
	
	override void OnMissionFinish()
	{
	#ifdef EXPANSIONMODCORE
		ExpansionGlobalID.s_IsMissionLoaded = false;
	#endif

	#ifdef EXPANSIONUI
		//Expansion_DestroyItemTooltip();
		//Expansion_DestroyItemInspection();
	#endif

		super.OnMissionFinish();

	#ifdef EXPANSIONMODAI
		//! Clean up eAI command menu static reference
		if (eAICommandMenu.instance)
		{
			eAICommandMenu.instance.Close();
			eAICommandMenu.instance = null;
		}
	#endif
	}
	
	

	void DoReconnect() { 
		GetDayZGame().HiveDoReconnect(m_mainmenu);
	}
	
}