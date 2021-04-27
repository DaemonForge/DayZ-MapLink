modded class MissionBase
{
	
	override void UniversalApiReady(){
		super.UniversalApiReady();
		Print("[MAPLINK] Init");
		GetMapLinkConfig();
	}

}