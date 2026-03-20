modded class ManBase {
    
	protected string m_MapLinkGUIDCache = "";
	protected string m_MapLinkNameCache = "";
	protected bool m_MapLink_UnderProtection = false;
	
	
	protected string m_TransferPoint = "";

	protected bool m_MapLink_ShouldDelete = false;

    
	bool UFSaveTransferPoint(string point = ""){
		m_TransferPoint = point;
		return true;
	}
	
	
	bool GetCachedIdentity(out string guid, out string name){
		if (!m_MapLinkGUIDCache || !m_MapLinkNameCache){
			return false;
		}
		guid = m_MapLinkGUIDCache;
		name = m_MapLinkNameCache;
		return true;
	}
	
	bool MapLinkShouldDelete(){
		return m_MapLink_ShouldDelete;
	}

	bool IsBeingTransferred(){
		return (m_TransferPoint != "");
	}

	bool IsUnderMapLinkProtection(){
		return (m_MapLink_UnderProtection);
	}
}