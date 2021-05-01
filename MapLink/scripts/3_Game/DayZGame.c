modded class DayZGame
{	
	
	ref MapLinkCountTimerWidget m_MapLinkCountTimerWidget;
	
	
	void DayZGame() {
	}

	override void DeferredInit() {
		super.DeferredInit();
	}
	
	void HiveReconnectTo(UApiServerData data){
		GetGame().Connect(NULL, data.IP, data.Port, data.Password);
	}	
	
	void MapLinkStartCountDown(int time){
		Print("[MAPLINK] MapLinkStartCountDown " + time);
		if(!GetGame().IsClient()){return;}
		if (!m_MapLinkCountTimerWidget){
			m_MapLinkCountTimerWidget = new MapLinkCountTimerWidget;
		}
		m_MapLinkCountTimerWidget.Start(time);
	}
	
	void MapLinkStopCountDown(){
		Print("[MAPLINK] MapLinkStopCountDown");
		if(!GetGame().IsClient()){return;}
		if (m_MapLinkCountTimerWidget){
			m_MapLinkCountTimerWidget.Stop();
		}
	}
};
