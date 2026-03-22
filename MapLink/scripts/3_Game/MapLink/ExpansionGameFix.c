/**
 * ExpansionGameFix.c
 *
 * Fixes ExpansionGame.OnFinish() not cleaning up state between server transfers.
 * - Resets m_IsLoaded flag so the new mission properly re-initializes
 * - Closes all ExpansionUIManager menus to destroy stale widget references
 */
#ifdef EXPANSIONMODCORE
modded class ExpansionGame
{
	override void OnFinish()
	{
		m_IsLoaded = false;

	#ifdef EXPANSIONUI
		if (!GetGame().IsDedicatedServer() && m_ExUIManager)
			m_ExUIManager.CloseAll();
	#endif

		super.OnFinish();
	}
}
#endif
