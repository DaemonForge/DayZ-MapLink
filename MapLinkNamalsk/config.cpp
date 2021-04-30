class CfgPatches
{
	class MapLinkNamalsk
	{
		requiredVersion=0.1;
		requiredAddons[]={
			"MapLink",
			"namalsk_survival"
		};
	};
};

class CfgMods
{
	class MapLinkNamalsk
	{
		dir="MapLinkNamalsk";
        name="MapLinkNamalsk";
        credits="DaemonForge, Iceblade, Dumpgrah";
        author="DaemonForge";
        authorID="0";
        version="0.1";
        extra=0;
        type="mod";
	    dependencies[]={ "Game", "World"};
	    class defs
	    {
			class gameScriptModule
            {
                value="";
                files[]={ 
					"MapLinkNamalsk/scripts/3_Game" 
				};
            };
			class worldScriptModule
            {
                value="";
                files[]={ 
					"MapLinkNamalsk/scripts/4_World" 
				};
            };
        };
    };
};
