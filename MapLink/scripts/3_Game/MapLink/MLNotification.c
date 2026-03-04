class MLNotification {
	
	// Send a notification from server to a specific client
	static void Send(PlayerIdentity identity, string title, string message, string icon = "", float duration = 6){
		if (!identity) return;
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(title);
		rpc.Write(message);
		rpc.Write(icon);
		rpc.Write(duration);
		rpc.Send(null, MAPLINK_NOTIFICATION, true, identity);
	}
	
	// Called on the client side to receive and display the notification
	static void Receive(ParamsReadContext ctx){
		string title, message, icon;
		float duration;
		if (ctx.Read(title) && ctx.Read(message) && ctx.Read(icon) && ctx.Read(duration)){
			Show(duration, title, message, icon);
		}
	}
	
	// Show a notification locally on the client
	static void Show(float duration, string title, string message, string icon = ""){
		NotificationSystem.AddNotificationExtended(duration, title, message, icon);
	}
}
