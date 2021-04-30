modded class DayZGame
{	
	void DayZGame() {
	}

	override void DeferredInit() {
		super.DeferredInit();
	}
	
	void HiveReconnectTo(UApiServerData data){
		GetGame().Connect(NULL, data.IP, data.Port, data.Password);
	}	
};
