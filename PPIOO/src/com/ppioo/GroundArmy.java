package com.ppioo;

import java.util.Random;

/**
 * Created by Bruno on 21/01/2017.
 */
public class GroundArmy extends Army{
    public GroundArmy(int quant) {
        this.setQuant(quant);
    }

    @Override
    public int attack() {
        Random dice = new Random();
        return dice.nextInt(6) + 1;
    }
}
