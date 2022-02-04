modded class DayZGame extends CGame
{	
	autoptr UApiServerData ConnectTo;
	bool m_IsToTransfer = false;
		
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
			SetConnecting(true);
			DeleteTitleScreen();
			GetGame().Connect(menu, ConnectTo.IP, ConnectTo.Port, ConnectTo.Password);
			ConnectTo = NULL;
		}
	}
};
