/**
 * File: MyDayZModClass.c
 * Description: A template class for DayZ modding.
 */
ref MLPlayerStoreCache m_MLPlayerStoreCache;

class MLPlayerStoreCache
{
    
    autoptr map<string, autoptr PlayerDataStore> m_PlayerDBQue = new map<string, autoptr PlayerDataStore>;

	void Add(string oid, PlayerDataStore data){
		m_PlayerDBQue.Set(oid, data);
	}
	
	void Remove(string oid){
		m_PlayerDBQue.Remove(oid);
	}
 
	bool Check(string oid){
		return m_PlayerDBQue.Contains(oid);
	}
	
	bool Find(string oid, out PlayerDataStore data){
		return m_PlayerDBQue.Find(oid, data);
	}
}
