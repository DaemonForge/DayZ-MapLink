class ActionHiveMoveToNewServer extends ActionInteractBase
{
	string mapName = "";
	void ExpansionActionOpenTraderMenu(){
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_ATTACHITEM;
	}
	
	override string GetText(){
		return "Open Departure Terminal";
	}

	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item ){
		if (GetMapLinkConfig().GetDepaturePoint(player.GetPosition())){
			return true;
		}
		return false;
	}
	
	override void OnExecuteServer( ActionData action_data )
	{
		super.OnExecuteServer( action_data );
	}

	override void OnExecuteClient( ActionData action_data ){
		super.OnExecuteClient( action_data );
		
		PlayerBase player = PlayerBase.Cast(action_data.m_Player);
		
		if (player){
			if (!m_DeparturePointMenu){
				m_DeparturePointMenu = DeparturePointMenu.Cast(GetGame().GetUIManager().EnterScriptedMenu(MAPLINK_DEPARTUREPOINTMENU, NULL));
			}
			m_DeparturePointMenu.SetDeparturePoint(GetMapLinkConfig().GetDepaturePoint(player.GetPosition()));
		}
	}
	
};