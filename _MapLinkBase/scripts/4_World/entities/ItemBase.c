modded class ItemBase {
	//returns remaining
    int MLAddQuantity(float amount) {	
        if (!IsMagazine()) {
            int remainingQty = GetQuantityMax() - GetQuantity();
			if (remainingQty == 0){
                return amount;
			}
            if ( amount >= remainingQty ) {
                AddQuantity(remainingQty);
                return amount - remainingQty;
            } else {
                AddQuantity(amount);
                return 0;
            }
		}        
        return amount;
	}

	//returns remaining
    int MLSetQuantity(float amount) {	
        if (!IsMagazine()) {
            int maxQty = GetQuantityMax();			
            if ( amount >= maxQty ) {
                SetQuantity(maxQty);
                return amount - maxQty;
            } else {
                SetQuantity(amount);
                return 0;
            }
		}        
        return amount;
	}
    
    bool MLHasQuantityBar() {
        return this.ConfigGetBool("quantityBar");
    }
};