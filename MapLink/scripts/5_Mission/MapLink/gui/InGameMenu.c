modded class InGameMenu
{
	protected ref RespawnServerMenu m_MapLinkRespawn;
	
	// Override OnClick_Respawn to show server selection BEFORE RespawnDialogue
	override void OnClick_Respawn()
	{
		MLLog.Info("InGameMenu.OnClick_Respawn called");
		
		// Guard against double-open
		if (m_MapLinkRespawn)
		{
			MLLog.Info("InGameMenu.OnClick_Respawn: Server selection already open");
			return;
		}
		
		// If player is alive+unconscious, let vanilla handle the confirmation dialog first
		Man player = GetGame().GetPlayer();
		if (player && player.IsUnconscious() && !player.IsDamageDestroyed())
		{
			MLLog.Info("InGameMenu.OnClick_Respawn: Player unconscious, vanilla dialog");
			super.OnClick_Respawn();
			return;
		}
		
		// Player is dead - check if we should show server selection
		if (GetMapLinkConfig() && GetMapLinkConfig().Servers && GetMapLinkConfig().Servers.Count() > 0)
		{
			// If current server has a specific RespawnServer (not ANYBLANK), auto-redirect immediately
			RespawnServerEntry directRedirect = RespawnServerMenu.GetDirectRespawnServer();
			if (directRedirect)
			{
				MLLog.Info("InGameMenu.OnClick_Respawn: Direct RespawnServer override '" + directRedirect.Name + "' - auto-redirecting");
				UServerData rData = new UServerData(directRedirect.IP, directRedirect.Port, directRedirect.Password);
				rData.Name = directRedirect.Name;
				GetDayZGame().HiveSetReconnectTo(rData);
				g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Call(g_Game.DisconnectSessionForce);
				return;
			}
			
			// Check if exactly one eligible server AND player can't stay here - auto-redirect without showing menu
			RespawnServerEntry singleServer = RespawnServerMenu.GetSingleEligibleServer();
			if (singleServer && RespawnServerMenu.HasRespawnOverride())
			{
				MLLog.Info("InGameMenu.OnClick_Respawn: Single eligible server '" + singleServer.Name + "' - auto-redirecting (respawn override)");
				UServerData serverData = new UServerData(singleServer.IP, singleServer.Port, singleServer.Password);
				serverData.Name = singleServer.Name;
				GetDayZGame().HiveSetReconnectTo(serverData);
				g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Call(g_Game.DisconnectSessionForce);
				return;
			}
			
			if (RespawnServerMenu.HasOtherServers())
			{
				MLLog.Info("InGameMenu.OnClick_Respawn: Showing server selection overlay");
				MapLink_HideMenuWidgets();
				m_MapLinkRespawn = new RespawnServerMenu(layoutRoot, this);
				return;
			}
			MLLog.Info("InGameMenu.OnClick_Respawn: No other servers, vanilla flow");
		}
		
		super.OnClick_Respawn();
	}
	
	// Called by RespawnServerMenu when "RESPAWN HERE" is picked (deferred via CallQueue)
	void MapLink_ContinueVanillaRespawn()
	{
		MLLog.Info("InGameMenu.MapLink_ContinueVanillaRespawn");
		m_MapLinkRespawn = NULL; // destroys overlay (Unlinks root widget in destructor)
		MapLink_ShowMenuWidgets();
		super.OnClick_Respawn(); // proceed with vanilla flow (RespawnDialogue or direct respawn)
	}
	
	// Hide all existing InGameMenu children so only our overlay is visible
	protected void MapLink_HideMenuWidgets()
	{
		Widget child = layoutRoot.GetChildren();
		while (child)
		{
			child.Show(false);
			child = child.GetSibling();
		}
	}
	
	// Restore InGameMenu children (UpdateGUI will fix conditional visibility on next frame)
	protected void MapLink_ShowMenuWidgets()
	{
		Widget child = layoutRoot.GetChildren();
		while (child)
		{
			child.Show(true);
			child = child.GetSibling();
		}
	}
}
