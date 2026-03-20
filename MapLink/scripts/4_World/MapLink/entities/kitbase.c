modded class ShelterKit extends ItemBase
{

	override void DisassembleKit(ItemBase item) {
		PlayerBase player = PlayerBase.Cast(GetHierarchyRootPlayer());
		if ( player && player.IsBeingTransferred() ){
			return;
			
		}
		super.DisassembleKit(item);
	}
	
}

modded class FenceKit extends ItemBase
{

	override void DisassembleKit(ItemBase item) {
		PlayerBase player = PlayerBase.Cast(GetHierarchyRootPlayer());
		if ( player && player.IsBeingTransferred() ){
			return;
			
		}
		super.DisassembleKit(item);
	}
	
}

modded class WatchtowerKit extends ItemBase
{

	override void DisassembleKit(ItemBase item) {
		PlayerBase player = PlayerBase.Cast(GetHierarchyRootPlayer());
		if ( player && player.IsBeingTransferred() ){
			return;
			
		}
		super.DisassembleKit(item);
	}
	
}

modded class TerritoryFlagKit extends ItemBase
{

	override void DisassembleKit(ItemBase item) {
		PlayerBase player = PlayerBase.Cast(GetHierarchyRootPlayer());
		if ( player && player.IsBeingTransferred() ){
			return;
			
		}
		super.DisassembleKit(item);
	}
	
}