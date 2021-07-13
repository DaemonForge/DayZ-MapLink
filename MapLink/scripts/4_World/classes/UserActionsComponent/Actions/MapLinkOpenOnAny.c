class ActionMapLinkOpenOnAny: ActionInteractBase
{
	void ActionMapLinkOpenOnAny()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_HUDCursorIcon = CursorIcons.OpenDoors;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTCursor;
	}
	
	
	override string GetText()
	{
		return "Open Departure Terminal";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( !target || !player) 
			return false;
		
		EntityAI theTarget;
		
		MapLinkDepaturePoint depaturePoint;
		//Hive_Terminals Use there own action, to be easier for modders to do Might make a moddable array to allow for modders to add there own objects
		if ( Class.CastTo(theTarget, target.GetObject()) && theTarget.GetType() != "Hive_Terminal" && player.FindDepaturePointForEntity(theTarget, depaturePoint) ) {
			return true;
		}
		return false;
	}
	
	override void OnExecuteServer( ActionData action_data ){
		super.OnExecuteServer( action_data );
		
		//Something should go here for better validation server side? Right now it just checks for the nearest depaturePoint to the player within 50 meters
	}
	
	override void OnExecuteClient( ActionData action_data ){
		super.OnExecuteClient( action_data );

		if ( !action_data )
			return;
		
		EntityAI theTarget;
		PlayerBase thePlayer;
		if (Class.CastTo(theTarget, action_data.m_Target.GetObject()) && Class.CastTo(thePlayer, action_data.m_Player)){
			MapLinkDepaturePoint depaturePoint;
			if (thePlayer.FindDepaturePointForEntity(theTarget, depaturePoint)){
				thePlayer.MapLinkUpdateClientSettingsToServer();
				if (!m_DeparturePointMenu){
					m_DeparturePointMenu = DeparturePointMenu.Cast(GetGame().GetUIManager().EnterScriptedMenu(MAPLINK_DEPARTUREPOINTMENU, NULL));
				}
				m_DeparturePointMenu.SetDeparturePoint(depaturePoint);
			}
		}
	}
	
}