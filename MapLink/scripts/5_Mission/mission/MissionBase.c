modded class MissionBase
{
	
	override void UniversalApiReady(){
		super.UniversalApiReady();
		MLLog.Info("[MAPLINK] Init");
		GetMapLinkConfig();
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