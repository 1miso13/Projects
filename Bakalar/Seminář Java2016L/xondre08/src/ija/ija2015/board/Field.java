/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package ija.ija2015.board;

/**
 *
 * @author xondre08
 * @author xkeryj00
 */
public interface Field {

    /**
     *
     * @return Daný stĺpec.
     */
    public int getCol();

    /**
     *
     * @return Daný riadok.
     */
    public int getRow();
    /**
     * Pridá susedné pole field v danom smere dirs.
     * @param dirs Smer v ktorom sa pridáva pole.
     * @param field Pridávané pole.
     */
    public void addNextField(Field.Direction dirs,Field field);
    /**
     * Vracia žetón, ktorý je vložený na pole.
     * @return Vložený žetón. Ak je pole prázdne, vráti null.
     */
    public Disk getDisk();
    /**
     * Vráti susedné pole v danom smere dirs.
     * @param dirs Smer v ktorom sa pridáva pole.
     * @return Susedné pole v danom smere dirs.
     */
    public Field nextField(Field.Direction dirs);
    /**
     * Vloží na pole žetón. Raz vložený žetón sa už nedá odobrať.
     * @param disk Vkládaný žetón.
     * @return Vracia úspešnosť akcie. Ak je už pole obsadené, vráti false.
     */
    public boolean putDisk(Disk disk);
    /**
     * Výčtový typ reprezentujúcí okolie (smery) jednotlivých polí.
     */
    public static enum Direction
    {

        /**
         * Down
         */
        D,

        /**
         * Left
         */
        L,

        /**
         * Left-Down
         */
        LD,

        /**
         * Left-Up
         */
        LU,

        /**
         * Right
         */
        R,

        /**
         * Right-Down
         */
        RD,

        /**
         * Right-Up
         */
        RU,

        /**
         * Up
         */
        U
    }
}
