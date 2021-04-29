class DeparturePointWidget  extends ScriptedWidgetEventHandler {
	
	static string 				m_LayoutPath = "MapLink/gui/layouts/TransferPoint.layout";
	
	DeparturePointMenu 			m_Parent;
	MapLinkArrivalPointsRef		m_ArrivalPoint;
	MapLinkSpawnPoint			m_MapLinkSpawnPoint;
	
	Widget						m_Root;
	
	ButtonWidget				m_Transfer;
	TextWidget	 				m_Name
	TextWidget 					m_Map_Text
	ImageWidget 				m_Cost_image
	TextWidget 					m_Cost_Text
	ImageWidget 				m_TransferImage
	
	
	void DeparturePointWidget(Widget parent, DeparturePointMenu mapMenu, MapLinkArrivalPointsRef arrivalPoint,	MapLinkSpawnPoint spawnPoint){
		
		m_Root				= Widget.Cast(GetGame().GetWorkspace().CreateWidgets(m_LayoutPath, parent));
		
		m_Parent	 		= DeparturePointMenu.Cast(mapMenu);
		m_ArrivalPoint 		= MapLinkArrivalPointsRef.Cast(arrivalPoint);
		m_MapLinkSpawnPoint	= MapLinkSpawnPoint.Cast(spawnPoint);
		
		m_Transfer			= ButtonWidget.Cast(layoutRoot.FindAnyWidget("Transfer"));
		
		m_Name 				= TextWidget.Cast(layoutRoot.FindAnyWidget("Transfer"));
		m_Map_Text 			= TextWidget.Cast(layoutRoot.FindAnyWidget("Map_Text"));
		m_Cost_image 		= ImageWidget.Cast(layoutRoot.FindAnyWidget("Cost_image"));
		m_Cost_Text 		= TextWidget.Cast(layoutRoot.FindAnyWidget("Cost_Text"));
		m_TransferImage 	= ImageWidget.Cast(layoutRoot.FindAnyWidget("TransferImage"));
		
		
		
		UApiServerData serverData = UApiServerData.Cast(GetMapLinkConfig().GetServer(spawnPoint.ServerName));
		
		m_TransferImage.LoadImageFile(0, m_ArrivalPoint.GetIcon());
		
		m_Name.SetText(m_MapLinkSpawnPoint.EntryPointDisplayName);
		m_Cost_Text.SetText(m_ArrivalPoint.Cost.ToString());
		
		m_Cost_image.LoadImageFile(0, MapLinkConfig().GetCostIcon(m_ArrivalPoint.AcceptedCurrencyId));
		
		m_Map_Text.SetText(GetNiceMapName(serverData.Map));
		
		m_Root.SetHandler(this);
	}
	
	
	override bool OnClick( Widget w, int x, int y, int button )
	{		
		//Print("[BASICMAP] MarkerListItem OnClick");
		if (w == m_Transfer) {
			m_Parent.InitTravel(m_ArrivalPoint, m_MapLinkSpawnPoint);
			return true;
		}
		return super.OnClick(w, x, y, button);
	}
	
}