modded class ActionConstructor
{
	override void RegisterActions( TTypenameArray actions )
	{
		super.RegisterActions( actions );	
		actions.Insert( ActionHiveMoveToNewServer );
		actions.Insert( MapLinkOpenOnAny );

	}
};