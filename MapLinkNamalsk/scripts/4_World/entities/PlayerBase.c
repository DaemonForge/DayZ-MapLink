modded class PlayerBase extends ManBase{
	
	
	override void  OnUApiSave(ref PlayerDataStore data){
		super.OnUApiSave(data);
		data.WriteFloat("namalsk", "StatStomachFreezing", GetStatStomachFreezing().Get());
		data.WriteFloat("namalsk", "StatStomachFrozen", GetStatStomachFrozen().Get());
		data.WriteFloat("namalsk", "StatStomachWarm", GetStatStomachWarm().Get());
		data.WriteFloat("namalsk", "StatColdResistance", GetStatColdResistance().Get());
		data.WriteFloat("namalsk", "StatFrostbiteEffect", GetStatFrostbiteEffect().Get());
		data.WriteInt("namalsk", "StatFrostbiteZones", GetStatFrostbiteZones().Get());
	}
	
	override void OnUApiLoad(ref PlayerDataStore data){
		super.OnUApiLoad(data);
		float StatStomachFreezing;
		if (data.ReadFloat("namalsk", "StatStomachFreezing", StatStomachFreezing)){
			GetStatFrostbiteEffect().Set(StatStomachFreezing);
		}
		float StatStomachFrozen;
		if (data.ReadFloat("namalsk", "StatStomachFrozen", StatStomachFrozen)){
			GetStatFrostbiteEffect().Set(StatStomachFrozen);
		}
		float StatStomachWarm;
		if (data.ReadFloat("namalsk", "StatStomachWarm", StatStomachWarm)){
			GetStatFrostbiteEffect().Set(StatStomachWarm);
		}
		float StatColdResistance;
		if (data.ReadFloat("namalsk", "StatColdResistance", StatColdResistance)){
			GetStatFrostbiteEffect().Set(StatColdResistance);
		}
		float StatFrostbiteEffect;
		if (data.ReadFloat("namalsk", "StatFrostbiteEffect", StatFrostbiteEffect)){
			GetStatFrostbiteEffect().Set(StatFrostbiteEffect);
		}
		int StatFrostbiteZones;
		if (data.ReadInt("namalsk", "StatFrostbiteZones", StatFrostbiteZones)){
			GetStatFrostbiteZones().Set(StatFrostbiteZones);
		}
	}
}