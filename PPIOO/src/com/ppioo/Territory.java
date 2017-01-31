package com.ppioo;

/**
 * Created by Bruno on 21/01/2017.
 */
public class Territory {
    private String name;
    private int player;
    private Continents continent;
    private Army groundArmy;
    private Army airArmy;

    public Territory(String name, Continents continent) {
        this.name = name;
        this.player = -1;
        this.continent = continent;
        this.groundArmy = new GroundArmy(0);
        this.airArmy = new AirArmy(0);
    }
    public String getName() {
        return name;
    }

    public int getPlayer() {
        return player;
    }

    public Continents getContinent() {
        return continent;
    }

    public void setGroundArmy(Army groundArmy) {
        this.groundArmy = groundArmy;
    }

    public void setAirArmy(Army airArmy) {
        this.airArmy = airArmy;
    }

    public Army getGroundArmy() {
        return groundArmy;
    }

    public Army getAirArmy() {
        return airArmy;
    }

    public void setPlayer(int player) {
        this.player = player;
    }
}