/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package ija.ija2015.game;

import ija.ija2015.board.BoardField;
import ija.ija2015.board.BorderField;
import ija.ija2015.board.Field;
import ija.ija2015.board.Rules;

/**
 *
 * @author xondre08
 * @author xkeryj00
 */
public class ReversiRules implements Rules{
    private int size;

    /**
     * Inicializácia pravidiel.
     * @param size - Veľkosť hracej dosky.
     */
    public ReversiRules(int size) {
        this.size = size;
    }
    
    /**
     * Vytvorí odpovedajúce pole na zadaných indexoch.
     * @param row - Riadok dosky.
     * @param col - Stĺpec dosky.
     * @return Vytvorené pole.
     */
    @Override
    public Field createField(int row, int col) {
        Field field;
        if(row==0||row==size+1||col==0||col==size+1)
            field=new BorderField();
        else
            field=new BoardField(row, col);
        return field;
    }
    /**
     * Vracia veľkosť dosky.
     * @return Veľkosť dosky.
     */
    @Override
    public int getSize() {
        return this.size;
        
    }
    /**
     * Vracia počet žetónov jednotlivých hráčov.
     * @return Počet žetónov.
     */
    @Override
    public int numberDisks() {
        return size*size/2;
    }
    
}
