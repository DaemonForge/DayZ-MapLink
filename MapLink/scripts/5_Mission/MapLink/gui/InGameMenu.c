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
