modded class MissionBase
{
	
	override void UFrameworkReady(){
		super.UFrameworkReady();
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