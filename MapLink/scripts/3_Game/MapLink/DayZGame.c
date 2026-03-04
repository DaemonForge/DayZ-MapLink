modded class DayZGame {	
	
	autoptr MapLinkCountTimerWidget m_MapLinkCountTimerWidget;
	
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
