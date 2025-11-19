modded class PlayerBase extends ManBase{
	
	protected string m_MapLinkGUIDCache = "";
	protected string m_MapLinkNameCache = "";
	protected bool m_MapLink_UnderProtection = false;
	
	protected string m_TransferPoint = "";
	
	protected bool m_MapLink_ShouldDelete = false;
	
	
	override void Init()
	{
		super.Init();
		RegisterNetSyncVariableBool("m_MapLink_UnderProtection");
	}
	
	override void OnPlayerLoaded()
	{
		super.OnPlayerLoaded();
		if (GetIdentity()){
			m_MapLinkGUIDCache = GetIdentity().GetId();
			m_MapLinkNameCache = GetIdentity().GetName();
		}
	}
	
	
	bool IsUnderMapLinkProtection(){
		return (m_MapLink_UnderProtection);
	}
	
	
	bool UFSaveTransferPoint(string point = ""){
		m_TransferPoint = point;
		return true;
	}
	
	
	bool IsBeingTransfered(){
		return (m_TransferPoint != "");
	}
	
	bool GetCachedIdentity(out string guid, out string name){
		if (!m_MapLinkGUIDCache || !m_MapLinkNameCache){
			return false;
		}
		guid = m_MapLinkGUIDCache;
		name = m_MapLinkNameCache;
		return true;
	}
		
	
	void OnUFSave( PlayerDataStore data){
	}
	
	void OnUFLoad( PlayerDataStore data){
		
	}
	
	bool MapLinkShoudDelete(){
		return m_MapLink_ShouldDelete;
	}	

}
