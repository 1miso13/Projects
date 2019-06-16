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
public class Disk extends java.lang.Object{
    
    private boolean isWhite;

    /**
     *  Určuje, či je žetón zamrznutý alebo nie.
     */
    public boolean freezed;
    /**
     *
     * @param isWhite Hráč.
     */
    public Disk(boolean isWhite)
    {
        this.isWhite=isWhite;
        freezed=false;
    }

    @Override
    public int hashCode() {
        int hash = 5;
        hash = 79 * hash + (this.isWhite ? 1 : 0);
        return hash;
    }

    @Override
    public boolean equals(Object obj) {
        if (obj == null) {
            return false;
        }
        if (getClass() != obj.getClass()) {
            return false;
        }
        final Disk other = (Disk) obj;
        if (this.isWhite != other.isWhite) {
            return false;
        }
        return true;
    }

    /**
     * Test, či je žetón biely.
     * @return Vracia true, ak je kameň biely.
     */
    public boolean isWhite()
    {
        return isWhite;
    }
    /**
     * Otočenie (zmena farby) žetónu.
     */
    public void turn()
    {
        isWhite=!isWhite;
    }
}
