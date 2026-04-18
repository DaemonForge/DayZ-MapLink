class DeparturePointWidget  extends ScriptedWidgetEventHandler {
	
	static string 				m_LayoutPath = "MapLink/gui/layouts/TransferPoint.layout";
	
	DeparturePointMenu 			m_Parent;
	MapLinkArrivalPointsRef		m_ArrivalPoint;
	MapLinkSpawnPoint			m_MapLinkSpawnPoint;
	int 						m_LookupCid;
	bool 						m_ServerOnline = true;
	bool						m_HasEnoughMoney = true;
	bool						m_HasBlacklistedItems = false;
	string						m_BlacklistedItemName = "";
	string						m_TargetServerName = "";
	
	Widget						m_Root;
	
	ButtonWidget				m_Transfer;
	TextWidget	 				m_Name
	TextWidget 					m_Map_Text
	Widget						m_Cost_Frame;
	ImageWidget 				m_Cost_Image
	TextWidget 					m_Cost_Text
	ImageWidget 				m_TransferImage	TextWidget					m_Transfer_label	
	Widget						m_Status_Frame;
	ImageWidget 				m_Status_Image
	TextWidget 					m_Status_Text
	Widget						m_PlayerCount_Frame;
	TextWidget 					m_PlayerCount_Text
	Widget						m_Queue_Frame;
	TextWidget 					m_Queue_Text
	
	void DeparturePointWidget(Widget parent, DeparturePointMenu mapMenu, MapLinkArrivalPointsRef arrivalPoint,	MapLinkSpawnPoint spawnPoint){
		m_Root				= g_Game.GetWorkspace().CreateWidgets(m_LayoutPath, Widget.Cast(parent), true);
		
		m_Parent	 		= DeparturePointMenu.Cast(mapMenu);
		m_ArrivalPoint 		= MapLinkArrivalPointsRef.Cast(arrivalPoint);
		m_MapLinkSpawnPoint	= MapLinkSpawnPoint.Cast(spawnPoint);
		
		m_Transfer			= ButtonWidget.Cast(m_Root.FindAnyWidget("Transfer"));
		
		m_Name 				= TextWidget.Cast(m_Root.FindAnyWidget("ArrivalPoint_Name"));
		m_TransferImage 	= ImageWidget.Cast(m_Root.FindAnyWidget("TransferImage"));
		m_Transfer_label	= TextWidget.Cast(m_Root.FindAnyWidget("Transfer_label"));
		
		m_Map_Text 			= TextWidget.Cast(m_Root.FindAnyWidget("Map_Text"));
		
		m_Cost_Frame 		= Widget.Cast(m_Root.FindAnyWidget("Cost_Frame"));
		m_Cost_Image 		= ImageWidget.Cast(m_Root.FindAnyWidget("Cost_Image"));
		m_Cost_Text 		= TextWidget.Cast(m_Root.FindAnyWidget("Cost_Text"));
		
		m_Status_Frame 		= Widget.Cast(m_Root.FindAnyWidget("Status_Frame"));
		m_Status_Image		= ImageWidget.Cast(m_Root.FindAnyWidget("Status_Image"));
		m_Status_Text 		= TextWidget.Cast(m_Root.FindAnyWidget("Status_Text"));
		
		m_PlayerCount_Frame = Widget.Cast(m_Root.FindAnyWidget("PlayerCount_Frame"));
		m_PlayerCount_Text 	= TextWidget.Cast(m_Root.FindAnyWidget("PlayerCount_Text"));
		
		m_Queue_Frame 		= Widget.Cast(m_Root.FindAnyWidget("Queue_Frame"));
		m_Queue_Text 		= TextWidget.Cast(m_Root.FindAnyWidget("Queue_Text"));
		
		UServerData serverData = UServerData.Cast(GetMapLinkConfig().GetServer(spawnPoint.ServerName));
		if (serverData.QueryPort > 0){
			m_Status_Frame.Show(true);
			m_LookupCid = UF().api().SteamQuery(serverData.IP, serverData.QueryPort.ToString(), this, "UpdateServerStatus");
			
			m_ServerOnline = false;
		} else {
			m_Status_Frame.Show(false);
		}
		
		m_TransferImage.LoadImageFile(0, m_ArrivalPoint.GetIcon());
		
		m_Name.SetText(m_MapLinkSpawnPoint.DisplayName);
		if (m_ArrivalPoint.Cost > 0){
			m_Cost_Frame.Show(true);
			m_Cost_Text.SetText(m_ArrivalPoint.Cost.ToString());
			string costImage = GetMapLinkConfig().GetCurrency(m_ArrivalPoint.AcceptedCurrencyId).GetIcon();
			//Print("[MAPLINK] Settings Cost Image " + costImage);
			m_Cost_Image.LoadImageFile(0, costImage);
			
			PlayerBase player = PlayerBase.Cast(g_Game.GetPlayer());
			if (player && player.UGetPlayerBalance(GetMapLinkConfig().GetCurrencyKey(m_ArrivalPoint.AcceptedCurrencyId)) >= m_ArrivalPoint.Cost){
				m_Cost_Text.SetColor(ARGB(255, 105, 240, 174));
				m_HasEnoughMoney = true;
			} else {
				m_HasEnoughMoney = false;
				m_Cost_Text.SetColor(ARGB(255, 255, 61, 0));
				m_Transfer.SetAlpha(0.5);
			}
		} else {
			m_Cost_Frame.Show(false);
		}
		
		m_Map_Text.SetText(GetMapLinkConfig().GetNiceMapName(serverData.Map));
		
		// Check for blacklisted items on the target server
		m_TargetServerName = spawnPoint.ServerName;
		PlayerBase blPlayer = PlayerBase.Cast(g_Game.GetPlayer());
		if (blPlayer && serverData){
			string blacklistedType = serverData.FindFirstBlacklistedItem(blPlayer);
			if (blacklistedType != ""){
				m_HasBlacklistedItems = true;
				m_BlacklistedItemName = serverData.GetItemDisplayName(blacklistedType);
				m_Transfer.SetAlpha(0.5);
				m_Transfer_label.SetText("BLOCKED");
				m_Transfer_label.SetColor(ARGB(255, 255, 61, 0));
				m_Name.SetColor(ARGB(255, 255, 61, 0));
			}
		}
		
		m_Root.SetHandler(this);
		m_Root.Show(true);
	}
		
	void ~DeparturePointWidget(){
		UF().RequestCallCancel(m_LookupCid);
	}
	
	override bool OnClick( Widget w, int x, int y, int button )
	{		
		if (w == m_Transfer) {
			if (m_HasBlacklistedItems){
				MLNotification.Show(6, "MapLink - Transfer Blocked", "You cannot transfer to " + m_TargetServerName + " because '" + m_BlacklistedItemName + "' is not allowed on that server.", "set:maplink_icons image:status");
				return true;
			}
			if (m_ServerOnline && m_HasEnoughMoney) {
				UF().RequestCallCancel(m_LookupCid);
				m_Parent.InitTravel(m_ArrivalPoint.ArrivalPointName, m_ArrivalPoint.TransitionWaitTime, m_MapLinkSpawnPoint.ServerName);
				return true;
			}
		}
		return super.OnClick(w, x, y, button);
	}
	
	
	
	void UpdateServerStatus(int cid, int status, string oid, UFServerStatus data){	
      	if (status == UF_SUCCESS &&  m_Root && m_Root.IsVisible() && m_Status_Image && data){  //If its a success
			if (data.Status == "Online"){
				m_Status_Image.SetColor(ARGB(255, 105, 240, 174));
				m_Status_Text.SetText("Online");
				m_Status_Text.SetColor(ARGB(255, 105, 240, 174));
				m_ServerOnline = true;
				m_PlayerCount_Frame.Show(true);
				string playerCount = data.Players.ToString() + "/" + data.MaxPlayers.ToString();
				m_PlayerCount_Text.SetText(playerCount);
				if (data.QueuePlayers > 0){
					m_Queue_Frame.Show(true);
					m_Queue_Text.SetText(data.QueuePlayers.ToString());
				}
			} else {
				m_Status_Image.SetColor(ARGB(255, 255, 61, 0));
				m_Status_Text.SetText("Offline");
				m_Status_Text.SetColor(ARGB(255, 255, 61, 0));
				m_ServerOnline = false;
				m_Transfer.SetAlpha(0.5);
				m_Transfer_label.SetText("OFFLINE");
				m_Transfer_label.SetColor(ARGB(255, 255, 61, 0));
			}
      	} else {
			MLLog.Err("Error Returning Status: " + status );
			m_ServerOnline = true; //For now to ensure if an error happens
		}
	}
        
	
}