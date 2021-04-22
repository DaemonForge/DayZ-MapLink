modded class BleedingSourcesManagerServer extends BleedingSourcesManagerBase {
    
	void OnUApiLoad(PlayerDataStore data) {
		int activeBits = data.m_BleedingBits;
		int bitOffset = 0;
		int bleedsAdded = 0;

		for (int i = 0; i < BIT_INT_SIZE; i++) {
			int bit = 1 << bitOffset;
			
			if ((bit & activeBits) != 0 && CanAddBleedingSource(bit)) {
				AddBleedingSource(bit);
				
				if (bleedsAdded < data.m_BleedingActiveTime.Count()) {
					SetBleedingSourceActiveTime(bit, data.m_BleedingActiveTime.Get(bleedsAdded));
				}
				bleedsAdded++;
			}
			bitOffset++;
		}	
	}
	
	void OnUApiSave(PlayerDataStore data){
		int bits = data.m_BleedingBits;
		int bitOffset = 0;
		for (int i = 0; i < BIT_INT_SIZE; i++) {
			int bit = 1 << bitOffset;
			if ((bit & bits) != 0) {
				data.m_BleedingActiveTime.Insert(GetBleedingSourceActiveTime(bit));
			}
			bitOffset++;
		}
	}
	
	
}