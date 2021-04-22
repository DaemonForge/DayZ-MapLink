class ActionHiveMoveToNewServer extends ActionInteractBase
{
	string mapName = "";
	void ExpansionActionOpenTraderMenu(){
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_ATTACHITEM;
	}
	
	override string GetText(){
		if (mapName == "enoch"){
			return "Transfer To Chenarus";
		}
		return "Transfer To Enoch";
	}

	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item ){	
		GetGame().GetWorldName(mapName);
		
		return true;
	}
	
	override void OnExecuteServer( ActionData action_data )
	{
		super.OnExecuteServer( action_data );
		PlayerBase player = PlayerBase.Cast(action_data.m_Player);
		if (player && player.GetIdentity()){
			player.UApiSaveTransferPoint("first");
			player.SavePlayerToUApi();
			UApiServerData serverData;
			if (mapName == "enoch"){
				serverData = new UApiServerData("192.95.50.50", 2602);
			}
			if (mapName == "chernarusplus"){
				serverData = new UApiServerData("192.95.50.50", 2632);
			}
			GetRPCManager().SendRPC("TheHive", "RPCRedirectedKicked", new Param1<UApiServerData>(serverData), true, player.GetIdentity());
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(player.UApiKillAndDeletePlayer, 400, false);
		}
	}

	override void OnExecuteClient( ActionData action_data ){
		super.OnExecuteClient( action_data );
	}
	
};