package com.ppioo;

import java.util.Random;

/**
 * Created by Bruno on 21/01/2017.
 */
public abstract class Army {
    private int player;
    private int quant;

    public void setQuant(int quant) {
        this.quant = quant;
    }

    public int getQuant() {
        return this.quant;
    }

    public void setPlayer(int player) {
        this.player = player;
    }

    public abstract int attack();
}
