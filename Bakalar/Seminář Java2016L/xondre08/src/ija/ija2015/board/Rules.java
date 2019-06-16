/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package ija.ija2015.board;

/**
 *
 * @author xondre08
 * @author xkery00
 */
public interface Rules {
    /**
     * Vytvorí odpovedajúce pole na zadaných indexoch.
     * @param row Riadok dosky.
     * @param col Stĺpec dosky.
     * @return Vytvorené pole.
     */
    Field createField(int row, int col);
    /**
     * Vracia veľkosť dosky.
     * @return Veľkosť dosky.
     */
    int getSize();
    /**
     * Vracia počet žetónov jednotlivých hráčov.
     * @return Počet žetónov.
     */
    int numberDisks();
}
