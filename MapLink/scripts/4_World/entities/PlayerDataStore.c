modded class PlayerDataStore extends Managed{
	
	void PlayerDataStore(PlayerBase player = NULL){
		if (!player){return;}
		this.SavePlayer(player);
		
	}	
	
	void SavePlayer(PlayerBase player){
		if (!player){ return; }
		if (!player.GetCachedIdentity(GUID, m_Name) && !player.GetIdentity()){return;}
		int i = 0;
		
		if (player.GetIdentity()){
			GUID = player.GetIdentity().GetId();
			m_Name = player.GetIdentity().GetName();
			//MLLog.Debug("Savinging Player " + m_Name + " ("+ GUID +")");
		}
		m_Type = player.GetType();
		m_Health = player.GetHealth("", "Health");
		if (m_Health > 0){
			m_IsDead = false;
		} else {
			m_IsDead = true;
		}
		m_Blood = player.GetHealth("", "Blood");
		m_Shock = player.GetHealth("GlobalHealth", "Shock");
		m_Position = player.GetPosition();
		m_Orientaion = player.GetOrientation();
		
		m_TimeSurvivedValue = player.StatGet(AnalyticsManagerServer.STAT_PLAYTIME);
		m_PlayersKilledValue = player.StatGet(AnalyticsManagerServer.STAT_PLAYERS_KILLED);
		m_InfectedKilledValue = player.StatGet(AnalyticsManagerServer.STAT_INFECTED_KILLED);
		m_DistanceTraveledValue = player.StatGet(AnalyticsManagerServer.STAT_DISTANCE);
		m_LongRangeShotValue = player.StatGet(AnalyticsManagerServer.STAT_LONGEST_SURVIVOR_HIT );
		m_LifeSpanState = player.GetLifeSpanState();
		m_LastShavedSeconds = player.GetLastShavedSeconds();
		m_BloodType = player.GetStatBloodType().Get();
		m_HasBloodTypeVisible = player.HasBloodTypeVisible();
		m_HasBloodyHandsVisible = player.HasBloodyHands();
		m_IsUnconscious = player.IsUnconscious();
		m_IsRestrained = player.IsRestrained();
		
		// Damage System
		DamageZoneMap zones = new DamageZoneMap;
		DamageSystem.GetDamageZoneMap(player,zones);
		for( i = 0; i < zones.Count(); i++ ){
			string zone = zones.GetKey(i);
			SaveZoneHealth(zone, player.GetHealth(zone, "Health"), player.GetHealth(zone, "Blood"), player.GetHealth(zone, "Shock"));
		}
		
		array<EntityAI> items = new array<EntityAI>;
		player.GetInventory().EnumerateInventory(InventoryTraversalType.LEVELORDER, items);
		if (items && items.Count() > 0){
			for (i = 0; i < items.Count(); i++){
				EntityAI attachment_item = EntityAI.Cast(items.Get(i));
				if (!m_Attachments){m_Attachments = new array<autoptr UApiEntityStore>;}
				if (attachment_item && (player.GetInventory().HasAttachment(attachment_item) || attachment_item == player.GetItemInHands())){
					UApiEntityStore att_itemstore = new UApiEntityStore(attachment_item);
					m_Attachments.Insert(att_itemstore);
				} else {
					break;
				}
			}
		}
		player.OnUApiSave(this);
		GetGame().GetWorldName(m_Map);
		m_Server = UApiConfig().ServerID;
	}
	
	
	
	PlayerBase CreateWithIdentity(PlayerIdentity identity, vector Pos = vector.Zero){
		if (vector.Distance(Pos, vector.Zero) < 1){
			Pos = m_Position;
		}
		MLLog.Debug("CreateWithIdentity at " + Pos);
		Entity playerEnt = GetGame().CreatePlayer(PlayerIdentity.Cast(identity), m_Type, Pos, 0, "NONE");
		PlayerBase player;
		Class.CastTo(player, playerEnt);
		return player;
	}
	
	void SetupPlayer(PlayerBase player, vector Pos = vector.Zero, vector Ori =  vector.Zero){
		if (!player){ return; }
		int i = 0;
		
		if (vector.Distance(Pos, vector.Zero) < 1){
			Pos = m_Position;
		}
		if (vector.Distance(Ori, vector.Zero) < 0.1){
			Ori = m_Orientaion;
		}
		
		
		player.SetHealth("", "Health", m_Health);
		player.SetHealth("", "Blood", m_Blood);
		player.SetHealth("GlobalHealth", "Shock", m_Shock);
		
		// Damage System
		DamageZoneMap zones = new DamageZoneMap;
		DamageSystem.GetDamageZoneMap(player,zones);
		for( i = 0; i < zones.Count(); i++ ){
			string zone = zones.GetKey(i);
			float health, blood, shock;
			if (ReadZoneHealth(zone, health, blood, shock)){
				player.SetHealth(zone, "Health", health);
				player.SetHealth(zone, "Blood", blood);
				player.SetHealth(zone, "Shock", shock);
			}
		}
		
		
		MLLog.Debug("SetupPlayer at " + Pos);
		player.SetPosition(Pos);
		player.SetOrientation(Ori);
		
		player.SetBloodType(m_BloodType);
		player.SetBloodTypeVisible(m_HasBloodTypeVisible);
		
		
		if (m_Attachments && m_Attachments.Count() > 0){
			for(i = 0; i < m_Attachments.Count(); i++){
				if (m_Attachments.Get(i)){
					m_Attachments.Get(i).Create(player);				
				}
			}
		}
		player.StatUpdate(AnalyticsManagerServer.STAT_PLAYTIME, m_TimeSurvivedValue );
		player.StatUpdate(AnalyticsManagerServer.STAT_PLAYERS_KILLED, m_PlayersKilledValue);
		player.StatUpdate(AnalyticsManagerServer.STAT_INFECTED_KILLED, m_InfectedKilledValue);
		player.StatUpdate(AnalyticsManagerServer.STAT_DISTANCE, m_DistanceTraveledValue);
		player.StatUpdate(AnalyticsManagerServer.STAT_LONGEST_SURVIVOR_HIT, m_LongRangeShotValue );
		player.SetLifeSpanStateVisible(m_LifeSpanState);
		player.SetLastShavedSeconds(m_LastShavedSeconds);
		player.SetBloodyHands(m_HasBloodyHandsVisible);
		
		player.OnUApiLoad(this);
		
		player.GetStatBloodType().Set(m_BloodType);
	}
}