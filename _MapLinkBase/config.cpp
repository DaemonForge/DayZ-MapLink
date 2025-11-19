class CfgPatches
{
	class MapLinkBase
	{
		requiredVersion=0.1;
		requiredAddons[]={
		};
	};
};

class CfgMods
{
	class MapLinkBase
	{
		dir="MapLinkBase";
        name="MapLink";
        credits="DaemonForge, Iceblade, Dumpgrah";
        author="DaemonForge";
        authorID="0";
        version="0.1";
        extra=0;
        type="mod";
	    dependencies[]={ "Game", "World", "Mission"};
		defines[]={
			"MAPLINK"
		};
	    class defs
	    {
			class gameScriptModule
            {
				value = "";
                files[]={
					"_MapLinkBase/scripts/3_Game"
				};
            };
			class worldScriptModule
            {
                value="";
                files[]={ 
					"_MapLinkBase/scripts/4_World"
				};
            };
        };
    };
};