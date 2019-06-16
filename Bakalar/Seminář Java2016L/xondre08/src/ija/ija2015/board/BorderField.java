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
public class BorderField implements Field{
    private Disk disk;
    /**
     * Vracia null, pretože na borderField sa nemôže ukladať.
     */
    public BorderField() 
    {
        disk=null;
    }
    /**
     *
     * @param dirs Smer v ktorom sa uloží nový field, v tomto prípade nič nespraví.
     * @param field Poličko.
     */
    @Override
    public void addNextField(Field.Direction dirs, Field field)
    {
        
    }
    /**
     *
     * @return Vracia položený disk. V tomto prípade null.
     */
    @Override
    public Disk getDisk()
    {
        return this.disk;
    }
    /**
     *
     * @param dirs Smery
     * @return Vracia null.
     */
    @Override
    public Field nextField(Field.Direction dirs)
    {
        return null;
    }
    /**
     *
     * @param disk Vloženie disku na políčko.
     * @return Vracia vždy false.
     */
    @Override
    public boolean putDisk(Disk disk)
    {
            return false;
    }

    /**
     *
     * @return Vracia 0.
     */
    @Override
    public int getCol() {
        return 0;
    }

    /**
     *
     * @return Vracia 0.
     */
    @Override
    public int getRow(){
        return 0;
    }
}
