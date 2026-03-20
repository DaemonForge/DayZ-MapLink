modded class DayZGame {	
	
	// --- Merged from _MapLinkBase ---
	autoptr UServerData ConnectTo;
	bool m_IsToTransfer = false;
	// --- End _MapLinkBase properties ---
	
	autoptr MapLinkCountTimerWidget m_MapLinkCountTimerWidget;
	
	// --- Merged from _MapLinkBase ---
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
	
	void HiveSetReconnectTo(UServerData data){
		ConnectTo = UServerData.Cast(data);
		if (ConnectTo){
			m_IsToTransfer = true;
		}
	}	
	
	void HiveDoReconnect(UIScriptedMenu menu){
		if (ConnectTo && m_IsToTransfer){
			m_IsToTransfer = false;
			SetConnecting(true);
			DeleteTitleScreen();
			g_Game.Connect(menu, ConnectTo.IP, ConnectTo.Port, ConnectTo.Password);
			ConnectTo = NULL;
		}
	}
	// --- End _MapLinkBase methods ---
	
	void MapLinkStartCountDown(int time){
		if(!g_Game.IsClient()){return;}
		if (!m_MapLinkCountTimerWidget){
			m_MapLinkCountTimerWidget = new MapLinkCountTimerWidget;
		}
		m_MapLinkCountTimerWidget.Start(time);
	}
	
	void MapLinkStopCountDown(){
		if(!g_Game.IsClient()){return;}
		if (m_MapLinkCountTimerWidget){
			m_MapLinkCountTimerWidget.Stop();
		}
	}
	
	bool MLReadPublicData(string in, out string server, out string transferPoint){
		Print("[MAPLINK] MLReadPublicData " + in);
		if (in != "" && in.Length() > 1){
			TStringArray results = new TStringArray;
			in.Split("~", results);
			if (results.Count() == 1){
				server = results.Get(0);
				transferPoint = "";
				return true;
			} else if (results.Count() == 2){
				server = results.Get(0);
				transferPoint = results.Get(1);
				return true;
			} else if (results.Count() >= 3){
				Error2("[MAPLINK] MLReadPublicData", "Error reading public data");
				server = "";
				transferPoint = "";
				return false;
			}
		}
		return false;
	}
	
	override void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx){
		if (rpc_type == MAPLINK_REDIRECTKICKED && IsClient()){
			UServerData serverData = UServerData.ReadFromRPC(ctx);
			if (serverData){
				MLLog.Info("Kicked from Game");
				GetCallQueue(CALL_CATEGORY_SYSTEM).Call(DisconnectSessionForce);
				GetDayZGame().HiveSetReconnectTo(serverData);
				return;
			}
		}
		if (rpc_type == MAPLINK_NOTIFICATION && IsClient()){
			MLNotification.Receive(ctx);
			return;
		}
		super.OnRPC(sender, target, rpc_type, ctx);
	}
};
