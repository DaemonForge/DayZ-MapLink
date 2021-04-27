class Hive_Terminal extends BuildingSuper {
	override void SetActions(){
		super.SetActions();
		AddAction( ActionHiveMoveToNewServer );
		AddAction( ActionHiveMoveToNewServerNamalsk );
	}
}