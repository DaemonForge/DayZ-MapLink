class CfgPatches
{
	class TheHive
	{
		requiredVersion=0.1;
		requiredAddons[]={
			"UniversalApi"
		};
	};
};

class CfgMods
{
	class TheHive
	{
		dir="TheHive";
        name="TheHive";
        credits="DaemonForge, Iceblade, Dumpgrah";
        author="DaemonForge";
        authorID="0";
        version="0.1";
        extra=0;
        type="mod";
		inputs = "TheHive/inputs/inputs.xml";
	    dependencies[]={ "Game", "World", "Mission"};
	    class defs
	    {
			class gameScriptModule
            {
				value = "";
                files[]={
					"TheHive/scripts/3_Game"
				};
            };
			class worldScriptModule
            {
                value="";
                files[]={ 
					"TheHive/scripts/4_World" 
				};
            };
			
	        class missionScriptModule
            {
                value="";
                files[]={
					"TheHive/scripts/5_Mission" 
				};
            };
        };
    };
};
class CfgVehicles
{
	class HouseNoDestruct;
	class Hive_Terminal : HouseNoDestruct{
		scope = 1;
		displayName  = "Hive Terminal";
		model = "TheHive\data\hive_terminal.p3d";
	};
};