class RespawnServerWidget extends ScriptedWidgetEventHandler
{
	static string m_LayoutPath = "MapLink/gui/layouts/RespawnServerEntry.layout";
	
	RespawnServerMenu		m_Parent;
	RespawnServerEntry		m_Entry;
	int						m_LookupCid;
	bool					m_ServerOnline = true;
	
	Widget					m_Root;
	Widget					m_HoverBG;
	
	ButtonWidget			m_SelectServer;
	TextWidget				m_Name;
	TextWidget				m_Map_Text;
	
	Widget					m_Status_Frame;
	ImageWidget				m_Status_Image;
	TextWidget				m_Status_Text;
	Widget					m_PlayerCount_Frame;
	TextWidget				m_PlayerCount_Text;
	Widget					m_Queue_Frame;
	TextWidget				m_Queue_Text;
	
	void RespawnServerWidget(Widget parent, RespawnServerMenu menu, RespawnServerEntry entry)
	{
		m_Root			= g_Game.GetWorkspace().CreateWidgets(m_LayoutPath, Widget.Cast(parent), true);
		
		m_Parent		= RespawnServerMenu.Cast(menu);
		m_Entry			= entry;
		
		m_SelectServer	= ButtonWidget.Cast(m_Root.FindAnyWidget("SelectServer"));
		m_Name			= TextWidget.Cast(m_Root.FindAnyWidget("Server_Name"));
		m_Map_Text		= TextWidget.Cast(m_Root.FindAnyWidget("Map_Text"));
		m_HoverBG		= m_Root.FindAnyWidget("rootFrame_hover");
		
		m_Status_Frame		= Widget.Cast(m_Root.FindAnyWidget("Status_Frame"));
		m_Status_Image		= ImageWidget.Cast(m_Root.FindAnyWidget("Status_Image"));
		m_Status_Text		= TextWidget.Cast(m_Root.FindAnyWidget("Status_Text"));
		
		m_PlayerCount_Frame	= Widget.Cast(m_Root.FindAnyWidget("PlayerCount_Frame"));
		m_PlayerCount_Text	= TextWidget.Cast(m_Root.FindAnyWidget("PlayerCount_Text"));
		
		m_Queue_Frame		= Widget.Cast(m_Root.FindAnyWidget("Queue_Frame"));
		m_Queue_Text		= TextWidget.Cast(m_Root.FindAnyWidget("Queue_Text"));
		
		m_Name.SetText(entry.Name);
		m_Map_Text.SetText(GetMapLinkConfig().GetNiceMapName(entry.MapName));
		
		if (entry.QueryPort > 0)
		{
			m_Status_Frame.Show(true);
			m_LookupCid = U().api().SteamQuery(entry.IP, entry.QueryPort.ToString(), this, "UpdateServerStatus");
			m_ServerOnline = false;
		}
		else
		{
			m_Status_Frame.Show(false);
		}
		
		m_Root.SetHandler(this);
		m_Root.Show(true);
	}
	
	void ~RespawnServerWidget()
	{
		U().RequestCallCancel(m_LookupCid);
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (w == m_SelectServer)
		{
			if (m_ServerOnline)
			{
				U().RequestCallCancel(m_LookupCid);
				m_Parent.OnServerSelected(m_Entry);
				return true;
			}
		}
		return super.OnClick(w, x, y, button);
	}
	
	// DayZ-style hover: white text → red on hover, subtle row highlight
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (w == m_SelectServer && m_ServerOnline)
		{
			RespawnServerMenu.ColorHighlight(m_SelectServer);
			if (m_HoverBG) m_HoverBG.SetColor(ARGB(40, 200, 0, 0));
			return true;
		}
		return false;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (w == m_SelectServer)
		{
			if (m_ServerOnline)
				RespawnServerMenu.ColorNormal(m_SelectServer);
			else
				RespawnServerMenu.ColorDisable(m_SelectServer);
			if (m_HoverBG) m_HoverBG.SetColor(ARGB(0, 0, 0, 0));
			return true;
		}
		return false;
	}
	
	void UpdateServerStatus(int cid, int status, string oid, UFServerStatus data)
	{
		if (status == UF_SUCCESS && m_Root && m_Root.IsVisible() && m_Status_Image && data)
		{
			if (data.Status == "Online")
			{
				m_Status_Image.SetColor(ARGB(255, 105, 240, 174));
				m_Status_Text.SetText("Online");
				m_Status_Text.SetColor(ARGB(255, 105, 240, 174));
				m_ServerOnline = true;
				m_PlayerCount_Frame.Show(true);
				string playerCount = data.Players.ToString() + "/" + data.MaxPlayers.ToString();
				m_PlayerCount_Text.SetText(playerCount);
				if (data.QueuePlayers > 0)
				{
					m_Queue_Frame.Show(true);
					m_Queue_Text.SetText(data.QueuePlayers.ToString());
				}
			}
			else
			{
				m_Status_Image.SetColor(ARGB(255, 255, 61, 0));
				m_Status_Text.SetText("Offline");
				m_Status_Text.SetColor(ARGB(255, 255, 61, 0));
				m_ServerOnline = false;
				RespawnServerMenu.ColorDisable(m_SelectServer);
			}
		}
		else
		{
			MLLog.Err("RespawnServerWidget - Error Returning Status: " + status);
			m_ServerOnline = true; // Default to online if query fails
		}
	}
}
