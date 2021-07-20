modded class DayZGame
{	
	
	autoptr MapLinkCountTimerWidget m_MapLinkCountTimerWidget;
	
	autoptr UApiServerData ConnectTo;
	bool m_IsToTransfer = false;
	
	void DayZGame() {
		
	}

	
	bool IsToTransfer(){
		return ( m_IsToTransfer && ConnectTo && ConnectTo.IP != "" );
	}
	
	string TransferToServerName(){
		if (ConnectTo){
			return ConnectTo.Name;
		}
		return "ERROR";
	}
	
	override void DeferredInit() {
		super.DeferredInit();
	}
	
	void HiveSetReconnectTo(UApiServerData data){
		ConnectTo = UApiServerData.Cast(data);
		if (ConnectTo){
			m_IsToTransfer = true;
		}
	}	
	
	
	void HiveDoReconnect(UIScriptedMenu menu){
		if (ConnectTo && m_IsToTransfer){
			m_IsToTransfer = false;
			GetGame().Connect(menu, ConnectTo.IP, ConnectTo.Port, ConnectTo.Password);
			ConnectTo = NULL;
		}
	}
	
	
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
