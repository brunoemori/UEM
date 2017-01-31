package com.ppioo;

/**
 * Created by Bruno on 21/01/2017.
 */
public class AirArmy extends Army {
    public AirArmy(int quant) {
        this.setQuant(quant);
    }

    @Override
    public int attack() {
        return 1;
    }

}
