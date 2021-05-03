modded class DayZGame
{	
	
	ref MapLinkCountTimerWidget m_MapLinkCountTimerWidget;
	
	autoptr UApiServerData ConnectTo;
	
	void DayZGame() {
	}

	override void DeferredInit() {
		super.DeferredInit();
	}
	
	void HiveSetReconnectTo(UApiServerData data){
		Print("HiveSetReconnectTo");
		ConnectTo = UApiServerData.Cast(data);
		Print(ConnectTo);
	}	
	
	
	void HiveDoReconnect(UIScriptedMenu menu){
		Print("HiveDoReconnect");
		Print(ConnectTo);
		if (ConnectTo){
			GetGame().Connect(menu, ConnectTo.IP, ConnectTo.Port, ConnectTo.Password);
			ConnectTo = NULL;
		}
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
