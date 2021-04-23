modded class BleedingSourcesManagerServer extends BleedingSourcesManagerBase {
    
	void OnUApiLoad(PlayerDataStore data) {
		Print("[UAPI] Loading Bleeding Manager");	
		int BleedActiveIndex = 0;
		for (int i = 0; i < BIT_INT_SIZE; i++) {
			int bit = 1 << i;
			if ((bit & data.m_BleedingBits) == bit && CanAddBleedingSource(bit)) {
				AddBleedingSource(bit);
				if (BleedActiveIndex < data.m_BleedingActiveTime.Count()) {
					SetBleedingSourceActiveTime(bit, data.m_BleedingActiveTime.Get(BleedActiveIndex));
				} else { break; }
				BleedActiveIndex++;
			}
		}	
	}
	
	void OnUApiSave(PlayerDataStore data){
		data.m_BleedingActiveTime = new TIntArray;
		Print("[UAPI] Saving Bleeding Manager");	
		for (int i = 0; i < BIT_INT_SIZE; i++) {
			int bit = 1 << i;
			if ((bit & data.m_BleedingBits) == bit) {
				data.m_BleedingActiveTime.Insert(GetBleedingSourceActiveTime(bit));
			}
		}
	}
	
}