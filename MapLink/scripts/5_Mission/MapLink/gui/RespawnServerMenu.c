// RespawnServerMenu - sub-widget overlay parented to InGameMenu's layoutRoot.
// Shown when player clicks RESPAWN while dead, BEFORE the vanilla RespawnDialogue.
// Selecting "RESPAWN HERE" proceeds with vanilla flow (character picker etc).
// Selecting another server redirects via HiveSetReconnectTo + disconnect.
class RespawnServerMenu extends ScriptedWidgetEventHandler
{
	static string m_LayoutPath = "MapLink/gui/layouts/RespawnMenu.layout";
	
	protected Widget m_Root;
	protected Widget m_ServerGrid;
	protected ButtonWidget m_VanillaRespawn;
	protected TextWidget m_Heading;
	protected InGameMenu m_InGameMenu;
	
	protected ref array<ref RespawnServerWidget> m_ServerWidgets;
	protected ref array<ref RespawnServerEntry> m_ServerEntries;
	
	void RespawnServerMenu(Widget parent, InGameMenu menu)
	{
		m_InGameMenu = menu;
		MLLog.Info("RespawnServerMenu: Constructor - parent=" + parent.GetName() + " parentVisible=" + parent.IsVisible().ToString());
		
		m_Root = g_Game.GetWorkspace().CreateWidgets(m_LayoutPath, parent);
		if (!m_Root)
		{
			MLLog.Err("RespawnServerMenu: FAILED to create layout from " + m_LayoutPath);
			return;
		}
		
		MLLog.Info("RespawnServerMenu: Layout created - root=" + m_Root.GetName());
		
		m_ServerGrid     = m_Root.FindAnyWidget("RespawnServers_Grid");
		m_VanillaRespawn = ButtonWidget.Cast(m_Root.FindAnyWidget("VanillaRespawn"));
		m_Heading        = TextWidget.Cast(m_Root.FindAnyWidget("Heading"));
		
		MLLog.Info("RespawnServerMenu: Grid=" + (m_ServerGrid != null).ToString() + " Btn=" + (m_VanillaRespawn != null).ToString() + " Heading=" + (m_Heading != null).ToString());
		
		m_Root.SetHandler(this);
		
		if (m_Heading)
			m_Heading.SetText("SELECT SERVER");
		
		PopulateServers();
		
		m_Root.Show(true);
		MLLog.Info("RespawnServerMenu: Visible=" + m_Root.IsVisible().ToString());
	}
	
	void ~RespawnServerMenu()
	{
		MLLog.Info("RespawnServerMenu: Destructor");
		if (m_Root)
			m_Root.Unlink();
	}
	
	protected void PopulateServers()
	{
		m_ServerEntries = BuildEligibleServers();
		m_ServerWidgets = new array<ref RespawnServerWidget>;
		
		MLLog.Info("RespawnServerMenu: " + m_ServerEntries.Count().ToString() + " eligible servers");
		
		int created = 0;
		for (int i = 0; i < m_ServerEntries.Count(); i++)
		{
			RespawnServerEntry entry = m_ServerEntries.Get(i);
			MLLog.Info("RespawnServerMenu:   [" + i.ToString() + "] " + entry.Name + " current=" + entry.IsCurrentServer.ToString());
			
			if (entry.IsCurrentServer)
				continue;
			
			m_ServerWidgets.Insert(new RespawnServerWidget(m_ServerGrid, this, entry));
			created++;
		}
		MLLog.Info("RespawnServerMenu: Created " + created.ToString() + " server widgets");
	}
	
	// --- Static helpers ---
	
	static UServerData FindServerByName(TMapLinkServerDataArray servers, string name)
	{
		for (int i = 0; i < servers.Count(); i++)
		{
			if (servers.Get(i) && servers.Get(i).Name == name)
				return servers.Get(i);
		}
		return NULL;
	}
	
	static array<ref RespawnServerEntry> BuildEligibleServers()
	{
		autoptr array<ref RespawnServerEntry> entries = new array<ref RespawnServerEntry>;
		MapLinkConfig config = GetMapLinkConfig();
		if (!config || !config.Servers || config.Servers.Count() == 0) return entries;
		
		string currentServerName = UFConfig().ServerID;
		UServerData currentServerData = FindServerByName(config.Servers, currentServerName);
		
		bool hasRespawnOverride = (currentServerData && currentServerData.RespawnServer != "" && currentServerData.RespawnServer != currentServerName);
		
		if (hasRespawnOverride)
		{
			UServerData respawnServer = FindServerByName(config.Servers, currentServerData.RespawnServer);
			if (respawnServer)
				entries.Insert(new RespawnServerEntry(respawnServer.Name, respawnServer.Map, respawnServer.IP, respawnServer.Port, respawnServer.Password, respawnServer.QueryPort, false));
		}
		else
		{
			for (int i = 0; i < config.Servers.Count(); i++)
			{
				UServerData srv = config.Servers.Get(i);
				if (!srv || srv.IP == "") continue;
				
				bool isCurrent = (srv.Name == currentServerName);
				
				if (!isCurrent && srv.RespawnServer != "" && srv.RespawnServer != srv.Name)
					continue;
				
				entries.Insert(new RespawnServerEntry(srv.Name, srv.Map, srv.IP, srv.Port, srv.Password, srv.QueryPort, isCurrent));
			}
		}
		
		bool hasCurrentServer = false;
		for (int j = 0; j < entries.Count(); j++)
		{
			if (entries.Get(j).IsCurrentServer)
			{
				hasCurrentServer = true;
				break;
			}
		}
		if (!hasCurrentServer && currentServerData)
			entries.Insert(new RespawnServerEntry(currentServerData.Name, currentServerData.Map, currentServerData.IP, currentServerData.Port, currentServerData.Password, currentServerData.QueryPort, true));
		
		return entries;
	}
	
	static bool HasOtherServers()
	{
		autoptr array<ref RespawnServerEntry> entries = BuildEligibleServers();
		int otherCount = 0;
		for (int i = 0; i < entries.Count(); i++)
		{
			if (!entries.Get(i).IsCurrentServer)
				otherCount++;
		}
		return otherCount > 0;
	}
	
	// --- Click handling ---
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (w == m_VanillaRespawn)
		{
			MLLog.Info("RespawnServerMenu: RESPAWN HERE clicked - continuing vanilla flow");
			// Defer destruction to avoid destroying ourselves while inside event handler
			if (m_InGameMenu)
				GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(m_InGameMenu.MapLink_ContinueVanillaRespawn);
			return true;
		}
		return false;
	}
	
	// --- Hover handling (DayZ-style: white text → red on hover) ---
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (w == m_VanillaRespawn)
		{
			ColorHighlight(m_VanillaRespawn);
			return true;
		}
		return false;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (w == m_VanillaRespawn)
		{
			ColorNormal(m_VanillaRespawn);
			return true;
		}
		return false;
	}
	
	// --- DayZ-style color helpers ---
	
	static void ColorHighlight(Widget w)
	{
		if (!w) return;
		Widget panel = w.FindAnyWidget(w.GetName() + "_panel");
		if (panel) panel.SetColor(ARGB(255, 0, 0, 0));
		TextWidget label = TextWidget.Cast(w.FindAnyWidget(w.GetName() + "_label"));
		if (label) label.SetColor(ARGB(255, 255, 0, 0));
	}
	
	static void ColorNormal(Widget w)
	{
		if (!w) return;
		Widget panel = w.FindAnyWidget(w.GetName() + "_panel");
		if (panel) panel.SetColor(ARGB(0, 0, 0, 0));
		TextWidget label = TextWidget.Cast(w.FindAnyWidget(w.GetName() + "_label"));
		if (label) label.SetColor(ARGB(255, 255, 255, 255));
	}
	
	static void ColorDisable(Widget w)
	{
		if (!w) return;
		Widget panel = w.FindAnyWidget(w.GetName() + "_panel");
		if (panel) panel.SetColor(ARGB(0, 0, 0, 0));
		TextWidget label = TextWidget.Cast(w.FindAnyWidget(w.GetName() + "_label"));
		if (label) label.SetColor(ARGB(120, 120, 120, 120));
	}
	
	// --- Server selection ---
	
	void OnServerSelected(RespawnServerEntry entry)
	{
		if (!entry) return;
		
		if (entry.IsCurrentServer)
		{
			MLLog.Info("RespawnServerMenu: Current server selected - vanilla flow");
			if (m_InGameMenu)
				GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(m_InGameMenu.MapLink_ContinueVanillaRespawn);
			return;
		}
		
		MLLog.Info("RespawnServerMenu: Redirecting to " + entry.Name + " @ " + entry.IP + ":" + entry.Port.ToString());
		UServerData serverData = new UServerData(entry.IP, entry.Port, entry.Password);
		serverData.Name = entry.Name;
		
		GetDayZGame().HiveSetReconnectTo(serverData);
		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Call(g_Game.DisconnectSessionForce);
	}
}
