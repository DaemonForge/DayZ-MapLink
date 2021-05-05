class MapLinkOpenOnAny: ActionInteractBase
{
	void ActionOpenDoors()
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
		if ( !target ) 
			return false;
		
		EntityAI theTarget;
		
		//Hive_Terminals Use there own action, to be easier for modders to do Might make a moddable array to allow for modders to add there own objects
		if ( Class.CastTo(theTarget, target.GetObject()) && theTarget.GetType() != "Hive_Terminal" && GetMapLinkConfig().IsDepaturePoint(theTarget.GetType(), theTarget.GetPosition()) ) {
			return true;
		}
		return false;
	}
	
	
	override void OnExecuteClient( ActionData action_data ){
		super.OnExecuteClient( action_data );
		
		EntityAI theTarget;
		
		if (GetMapLinkConfig().IsDepaturePoint(theTarget.GetType(), theTarget.GetPosition()) && action_data && action_data.m_Target && Class.CastTo(theTarget, action_data.m_Target.GetObject()) ){
			if (!m_DeparturePointMenu){
				m_DeparturePointMenu = DeparturePointMenu.Cast(GetGame().GetUIManager().EnterScriptedMenu(MAPLINK_DEPARTUREPOINTMENU, NULL));
			}
			m_DeparturePointMenu.SetDeparturePoint(GetMapLinkConfig().GetDepaturePointAny(theTarget.GetType(), theTarget.GetPosition()));
		}
	}
}