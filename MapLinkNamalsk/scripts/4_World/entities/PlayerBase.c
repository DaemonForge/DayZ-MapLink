modded class PlayerBase extends ManBase{
	
	
	override void  OnUApiSave(ref PlayerDataStore data){
		super.OnUApiSave(data);
		data.Write("StatStomachFreezing", GetStatStomachFreezing().Get());
		data.Write("StatStomachFrozen", GetStatStomachFrozen().Get());
		data.Write("StatStomachWarm", GetStatStomachWarm().Get());
		data.Write("StatColdResistance", GetStatColdResistance().Get());
		data.Write("StatFrostbiteEffect", GetStatFrostbiteEffect().Get());
		data.Write("StatFrostbiteZones", GetStatFrostbiteZones().Get());
	}
	
	override void OnUApiLoad(ref PlayerDataStore data){
		super.OnUApiLoad(data);
		float StatStomachFreezing;
		if (data.Read("StatStomachFreezing", StatStomachFreezing)){
			GetStatFrostbiteEffect().Set(StatStomachFreezing);
		}
		float StatStomachFrozen;
		if (data.Read("StatStomachFrozen", StatStomachFrozen)){
			GetStatFrostbiteEffect().Set(StatStomachFrozen);
		}
		float StatStomachWarm;
		if (data.Read("StatStomachWarm", StatStomachWarm)){
			GetStatFrostbiteEffect().Set(StatStomachWarm);
		}
		float StatColdResistance;
		if (data.Read("StatColdResistance", StatColdResistance)){
			GetStatFrostbiteEffect().Set(StatColdResistance);
		}
		float StatFrostbiteEffect;
		if (data.Read("StatFrostbiteEffect", StatFrostbiteEffect)){
			GetStatFrostbiteEffect().Set(StatFrostbiteEffect);
		}
		int StatFrostbiteZones;
		if (data.Read("StatFrostbiteZones", StatFrostbiteZones)){
			GetStatFrostbiteZones().Set(StatFrostbiteZones);
		}
	}
}