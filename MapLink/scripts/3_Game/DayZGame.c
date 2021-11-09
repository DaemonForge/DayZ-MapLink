modded class DayZGame {	
	
	autoptr MapLinkCountTimerWidget m_MapLinkCountTimerWidget;
	
	void MapLinkStartCountDown(int time){
		if(!GetGame().IsClient()){return;}
		if (!m_MapLinkCountTimerWidget){
			m_MapLinkCountTimerWidget = new MapLinkCountTimerWidget;
		}
		m_MapLinkCountTimerWidget.Start(time);
	}
	
	void MapLinkStopCountDown(){
		if(!GetGame().IsClient()){return;}
		if (m_MapLinkCountTimerWidget){
			m_MapLinkCountTimerWidget.Stop();
		}
	}
};
