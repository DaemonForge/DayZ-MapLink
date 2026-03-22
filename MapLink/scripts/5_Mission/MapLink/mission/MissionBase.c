modded class MissionBase
{
	
	override void UFrameworkReady(){
		super.UFrameworkReady();
		MLLog.Info("[MAPLINK] Init");
		GetMapLinkConfig().Load();

		// Register MapLink settings template with UFramework
		string html = "";
		FileHandle file = OpenFile("MapLink/data/mod-settings.html", FileMode.READ);
		if (file)
		{
			string line;
			while (FGets(file, line) >= 0)
			{
				html += line + "\n";
			}
			CloseFile(file);
		}

		if (html != "")
		{
			autoptr TStringArray globals = new TStringArray;
			globals.Insert("MapLink");
			UF().Settings().Register("maplink", "MapLink", "DaemonForge", html, globals);
		}
	}

    override UIScriptedMenu CreateScriptedMenu(int id) {
        UIScriptedMenu menu = NULL;
        menu = super.CreateScriptedMenu(id);
        if (!menu) {
            switch (id) {
                case MAPLINK_DEPARTUREPOINTMENU:
                    menu = new DeparturePointMenu;
                    break;
            }
            if (menu) {
                menu.SetID(id);
            }
        }
        return menu;
    }
	
}