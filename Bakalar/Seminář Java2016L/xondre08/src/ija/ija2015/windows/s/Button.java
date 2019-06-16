/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package ija.ija2015.windows.s;

/**
 *
 * @author xondre08
 * @author xkeryj00
 */
public class Button extends javax.swing.JButton{
    private final int i;
    private final int j;

    /**
     *
     * @param i súradnica
     * @param j súradnica
     */
    public Button(int i,int j) {
        this.i = i;
        this.j = j;
    }

    /**
     *
     * @return Vracia súradnicu i.
     */
    public int getI() {
        return i;
    }

    /**
     *
     * @return Vracia súradnicu j.
     */
    public int getJ() {
        return j;
    }
    
}
