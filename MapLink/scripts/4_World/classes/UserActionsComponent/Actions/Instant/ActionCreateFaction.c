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
		return "Transfer To Livonia";
	}

	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item ){	
		GetGame().GetWorldName(mapName);
		if (mapName == "enoch" || mapName ==  "chernarusplus"){
			return true;
		}
		return false;
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
				serverData = new UApiServerData("192.95.50.50", 2602, "");
			}
			if (mapName == "chernarusplus"){
				serverData = new UApiServerData("192.95.50.50", 2632, "daemon");
			}
			GetRPCManager().SendRPC("TheHive", "RPCRedirectedKicked", new Param1<UApiServerData>(serverData), true, player.GetIdentity());
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(player.UApiKillAndDeletePlayer, 400, false);
		}
	}

	override void OnExecuteClient( ActionData action_data ){
		super.OnExecuteClient( action_data );
	}
	
};

class ActionHiveMoveToNewServerNamalsk extends ActionInteractBase
{
	string mapName = "";
	void ExpansionActionOpenTraderMenu(){
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_ATTACHITEM;
	}
	
	override string GetText(){
		if (mapName == "namalsk"){
			return "Transfer To Chenarus";
		}
		return "Transfer To Namalsk";
	}

	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item ){	
		GetGame().GetWorldName(mapName);
		if (mapName == "namalsk" || mapName == "chernarusplus"){
			return true;
		}
		return false;
	}
	
	override void OnExecuteServer( ActionData action_data )
	{
		super.OnExecuteServer( action_data );
		PlayerBase player = PlayerBase.Cast(action_data.m_Player);
		if (player && player.GetIdentity()){
			player.UApiSaveTransferPoint("first");
			player.SavePlayerToUApi();
			UApiServerData serverData;
			if (mapName == "namalsk"){
				serverData = new UApiServerData("192.95.50.50", 2602, "");
			}
			if (mapName == "chernarusplus"){
				serverData = new UApiServerData("192.95.50.50", 2662, "daemon");
			}
			GetRPCManager().SendRPC("TheHive", "RPCRedirectedKicked", new Param1<UApiServerData>(serverData), true, player.GetIdentity());
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(player.UApiKillAndDeletePlayer, 400, false);
		}
	}

	override void OnExecuteClient( ActionData action_data ){
		super.OnExecuteClient( action_data );
	}
	
};