modded class MissionGameplay
{

	void MissionGameplay()
    {   
		GetRPCManager().AddRPC( "TheHive", "RPCRedirectedKicked", this, SingeplayerExecutionType.Both );
    }

	void ~MissionGameplay()
	{
	}

	override void OnMissionStart()
    {
        super.OnMissionStart();
		PrintLang();
    }


	void PrintLang(){
		string Lang = "N/A";
		ListOptionsAccess	language_option;
		GameOptions options = new GameOptions();
		language_option = ListOptionsAccess.Cast( options.GetOptionByType( AT_OPTIONS_LANGUAGE ) );
		if (language_option){
			language_option.GetItemText(language_option.GetIndex(),Lang);
		}
		Print("Language is set to " + Lang);
	}
	
	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);

		if (GetGame().GetInput().LocalPress("THive_ToggleDebug", true))
		{
			GetRPCManager().SendRPC("TheHive", "RPCDisableLoadFromApi", NULL, false, NULL);
		}

		
	}
	
	void RPCRedirectedKicked( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target ) {
		
		Param1< UApiServerData > data; 
		if ( !ctx.Read( data ) ) return;
		Print("Kicked from Game");
		UApiServerData serverData = UApiServerData.Cast(data.param1);
		if (serverData){
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetGame().DisconnectSessionForce, 10);
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetDayZGame().HiveReconnectTo, 500, false, serverData);
		}
	}
	
}