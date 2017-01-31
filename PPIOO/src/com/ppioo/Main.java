package com.ppioo;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Random;

public class Main {
    private static final int MATRIX_HEIGHT = 5;
    private static final int MATRIX_WIDTH = 8;
    private static final int PLAYER_1 = 1;
    private static final int PLAYER_2 = 2;

    private static void printMap(Territory[][] gameMap) {
        System.out.println("World Map: ");
        for (int i = 0; i < MATRIX_HEIGHT; i++) {
            for (int j = 0; j < MATRIX_WIDTH; j++) {
                if (gameMap[i][j] != null)
                    System.out.format("%20s", gameMap[i][j].getName() + "(" + gameMap[i][j].getPlayer() + ")");
                else
                    System.out.format("%20s", "");
            }
            System.out.println();
        }
        System.out.println();
    }

    private static void printTerritories(ArrayList<Territory> territories, int player) {
        System.out.format("Player %s territories: \n", player);
        for (Territory t : territories) {
            System.out.format("%16s", t.getName());
        }

        System.out.println();

        for (Territory t : territories) {
            System.out.format("%16s", t.getGroundArmy().getQuant());
        }

        System.out.println();

        for (Territory t : territories) {
            System.out.format("%16s", t.getAirArmy().getQuant());
        }

        System.out.println();
    }

    public static void main(String[] args) {
        Territory gameWorld[][] = new Territory[MATRIX_HEIGHT][MATRIX_WIDTH];
        for (Territory[] aux : gameWorld) {
            Arrays.fill(aux, null);
        }

        //Setting world territories (33 territories)
        gameWorld[0][0] = new Territory("Alaska", Continents.NORTH_AMERICA);
        gameWorld[0][1] = new Territory("Vancouver", Continents.NORTH_AMERICA);
        gameWorld[0][2] = new Territory("Greenland", Continents.NORTH_AMERICA);
        gameWorld[1][1] = new Territory("California", Continents.NORTH_AMERICA);
        gameWorld[1][2] = new Territory("Ottawa", Continents.NORTH_AMERICA);
        gameWorld[2][1] = new Territory("Mexico", Continents.NORTH_AMERICA);
        gameWorld[2][2] = new Territory("New York", Continents.NORTH_AMERICA);

        gameWorld[3][0] = new Territory("Chile", Continents.SOUTH_AMERICA);
        gameWorld[3][1] = new Territory("Colombia", Continents.SOUTH_AMERICA);
        gameWorld[4][0] = new Territory("Argentina", Continents.SOUTH_AMERICA);
        gameWorld[4][1] = new Territory("Brazil", Continents.SOUTH_AMERICA);

        gameWorld[0][3] = new Territory("England", Continents.EUROPE);
        gameWorld[0][4] = new Territory("Italy", Continents.EUROPE);
        gameWorld[0][5] = new Territory("Sweden", Continents.EUROPE);
        gameWorld[1][4] = new Territory("Germany", Continents.EUROPE);
        gameWorld[1][5] = new Territory("Moscow", Continents.EUROPE);

        gameWorld[2][3] = new Territory("Nigeria", Continents.AFRICA);
        gameWorld[2][4] = new Territory("Egypt", Continents.AFRICA);
        gameWorld[3][3] = new Territory("Congo", Continents.AFRICA);
        gameWorld[3][4] = new Territory("Sudan", Continents.AFRICA);
        gameWorld[4][3] = new Territory("South Africa", Continents.AFRICA);
        gameWorld[4][4] = new Territory("Madagascar", Continents.AFRICA);

        gameWorld[0][7] = new Territory("Vladivotosk", Continents.ASIA);
        gameWorld[1][6] = new Territory("Omsk", Continents.ASIA);
        gameWorld[1][7] = new Territory("Siberia", Continents.ASIA);
        gameWorld[2][5] = new Territory("Middle East", Continents.ASIA);
        gameWorld[2][6] = new Territory("India", Continents.ASIA);
        gameWorld[2][7] = new Territory("China", Continents.ASIA);
        gameWorld[3][7] = new Territory("Japan", Continents.ASIA);

        gameWorld[3][5] = new Territory("Sumatra", Continents.OCEANIA);
        gameWorld[3][6] = new Territory("Borneo", Continents.OCEANIA);
        gameWorld[4][6] = new Territory("Australia", Continents.OCEANIA);
        gameWorld[4][7] = new Territory("New Guinea", Continents.OCEANIA);

        System.out.println("Done.");

        ArrayList<Territory> territoriesPlayer1, territoriesPlayer2;
        territoriesPlayer1 = new ArrayList<Territory>();
        territoriesPlayer2 = new ArrayList<Territory>();

        //Land distribution
        ArrayList<Territory> auxList = new ArrayList<Territory>();
        for (Territory[] arrayTerritories : gameWorld) {
            for (Territory territory : arrayTerritories) {
                if (territory != null)
                    auxList.add(territory);
            }
        }

        int auxCounter = 0;
        while (auxCounter < 16) {
            Random rand = new Random();
            int i = rand.nextInt(auxList.size());
            if (auxList.get(i).getPlayer() == -1) {
                auxList.get(i).setPlayer(PLAYER_1);
                territoriesPlayer1.add(auxList.get(i));
                auxList.remove(i);
                auxCounter++;
            }
        }

        for (Territory t : auxList) {
            t.setPlayer(PLAYER_2);
            territoriesPlayer2.add(t);
        }

        /*
        System.out.print("Player 1 territories: ");
        for (Territory t : territoriesPlayer1)
            System.out.print(t.getName() + " ");

        System.out.println();

        System.out.print("Player 2 territories: ");
        for (Territory t : territoriesPlayer2)
            System.out.print(t.getName() + " ");

        System.out.println("\n");
        */
    
        printMap(gameWorld);
        printTerritories(territoriesPlayer1, 1);
    }
}