class DeparturePointWidget  extends ScriptedWidgetEventHandler {
	
	static string 				m_LayoutPath = "MapLink/gui/layouts/TransferPoint.layout";
	
	DeparturePointMenu 			m_Parent;
	MapLinkArrivalPointsRef		m_ArrivalPoint;
	MapLinkSpawnPoint			m_MapLinkSpawnPoint;
	
	Widget						m_Root;
	
	ButtonWidget				m_Transfer;
	TextWidget	 				m_Name
	TextWidget 					m_Map_Text
	Widget						m_Cost_Frame;
	ImageWidget 				m_Cost_image
	TextWidget 					m_Cost_Text
	ImageWidget 				m_TransferImage
	
	
	void DeparturePointWidget(Widget parent, DeparturePointMenu mapMenu, MapLinkArrivalPointsRef arrivalPoint,	MapLinkSpawnPoint spawnPoint){
		m_Root				= GetGame().GetWorkspace().CreateWidgets(m_LayoutPath, Widget.Cast(parent), true);
		m_Parent	 		= DeparturePointMenu.Cast(mapMenu);
		m_ArrivalPoint 		= MapLinkArrivalPointsRef.Cast(arrivalPoint);
		m_MapLinkSpawnPoint	= MapLinkSpawnPoint.Cast(spawnPoint);
		
		m_Transfer			= ButtonWidget.Cast(m_Root.FindAnyWidget("Transfer"));
		
		m_Name 				= TextWidget.Cast(m_Root.FindAnyWidget("ArrivalPoint_Name"));
		m_Map_Text 			= TextWidget.Cast(m_Root.FindAnyWidget("Map_Text"));
		m_Cost_image 		= ImageWidget.Cast(m_Root.FindAnyWidget("Cost_image"));
		m_Cost_Text 		= TextWidget.Cast(m_Root.FindAnyWidget("Cost_Text"));
		m_TransferImage 	= ImageWidget.Cast(m_Root.FindAnyWidget("TransferImage"));
		m_Cost_Frame 		= Widget.Cast(m_Root.FindAnyWidget("Cost_Frame"));
		
		
		
		
		UApiServerData serverData = UApiServerData.Cast(GetMapLinkConfig().GetServer(spawnPoint.ServerName));
		
		m_TransferImage.LoadImageFile(0, m_ArrivalPoint.GetIcon());
		
		m_Name.SetText(m_MapLinkSpawnPoint.DisplayName);
		if (m_ArrivalPoint.Cost > 0){
			m_Cost_Frame.Show(true);
			m_Cost_Text.SetText(m_ArrivalPoint.Cost.ToString());
		
			m_Cost_image.LoadImageFile(0, MapLinkConfig().GetCostIcon(m_ArrivalPoint.AcceptedCurrencyId));
		} else {
			m_Cost_Frame.Show(false);
		}
		m_Map_Text.SetText(GetMapLinkConfig().GetNiceMapName(serverData.Map));
		
		m_Root.SetHandler(this);
		m_Root.Show(true);
	}
		
	override bool OnClick( Widget w, int x, int y, int button )
	{		
		if (w == m_Transfer) {
			m_Parent.InitTravel(m_ArrivalPoint.ArrivalPointName, m_ArrivalPoint.TransitionWaitTime, m_MapLinkSpawnPoint.ServerName);
			return true;
		}
		return super.OnClick(w, x, y, button);
	}
	
}