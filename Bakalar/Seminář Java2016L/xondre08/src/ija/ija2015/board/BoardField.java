/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package ija.ija2015.board;

import java.util.Objects;

/**
 *
 * @author xondre08
 * @author xkeryj00
 */
public class BoardField implements Field{
    private int col,row;
    private Disk disk;
    private Field[][] Field;

    /**
     *
     * @param row Daný počet riadkov.
     * @param col Daný počet stĺpcov.
     */
    public BoardField(int row, int col)
    {
        Field=new Field[3][3];
        this.row=row;
        this.col=col;
        disk=null;
    }
    /**
     * 
     * @return Vracia stĺpec(Col). 
     */
    @Override
    public int getCol() {
        return col;
    }
    /**
     * 
     * @return Vracia riadok(Row). 
     */
    @Override
    public int getRow() {
        return row;
    }
    
    @Override
    public String toString() {
        return "BoardField{" + "col=" + col + ", row=" + row +'}';
    }
    /**
     * 
     * @param dirs Všetky smery od daného field.
     * @param field Field, od ktorého sa vytvorí pole vo všetkých smeroch.
     */
    @Override
    public void addNextField(Field.Direction dirs, Field field)
    {
        switch(dirs)
        {
            case D:
            //Dole (Down) od pole.
                Field[2][1]=field;
                break;
            case L:
            //Vlevo (Left) od pole.
                Field[1][0]=field;
                break;
            case LD:
            //Vlevo dole po diagonále (Left-Down) od pole.
                Field[2][0]=field;
                break;
            case LU:
            //Vlevo nahoru po diagonále (Left-Up) od pole.
                Field[0][0]=field;
                break;
            case R:
            //Vpravo (Right) od pole.
                Field[1][2]=field;
                break;
            case RD:
            //Vpravo dole po diagonále (Right-Down) od pole.
                Field[2][2]=field;
                break;
            case RU:
            //Vpravo nahoru po diagonále (Right-Up) od pole.
                Field[0][2]=field;
                break;
            case U:
            //Nahoru (Up) od pole.
                Field[0][1]=field;
                break;
        }
    }

    @Override
    public int hashCode() {
        int hash = 3;
        hash = 29 * hash + this.col;
        hash = 29 * hash + this.row;
        hash = 29 * hash + Objects.hashCode(this.disk);
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
        final BoardField other = (BoardField) obj;
        if (this.col != other.col) {
            return false;
        }
        if (this.row != other.row) {
            return false;
        }
        if (!Objects.equals(this.disk, other.disk)) {
            return false;
        }
        
        return true;
    }

    
    /**
     * 
     * @return Vracia disk na danom poli. 
     */
    @Override
    public Disk getDisk()
    {
        return this.disk;
    }
    /**
     * 
     * @param dirs Smery, v ktorých sa kontroluje ďalší field
     * @return Vracia dané pole pre kontrolu.
     */
    @Override
    public Field nextField(Field.Direction dirs){
        switch(dirs)
        {
            case D:
            //Dole (Down) od pole.
                return Field[2][1];
            case L:
            //Vlevo (Left) od pole.
                return Field[1][0];
            case LD:
            //Vlevo dole po diagonále (Left-Down) od pole.
                return Field[2][0];
            case LU:
            //Vlevo nahoru po diagonále (Left-Up) od pole.
                return Field[0][0];
            case R:
            //Vpravo (Right) od pole.
                return Field[1][2];
            case RD:
            //Vpravo dole po diagonále (Right-Down) od pole.
                return Field[2][2];
            case RU:
            //Vpravo nahoru po diagonále (Right-Up) od pole.
                return Field[0][2];
            case U:
            //Nahoru (Up) od pole.
                return Field[0][1];
        }
        return null;
    }
    /**
     * 
     * @param disk Daný disk, ktorý sa vkladá.
     * @return Vracia úspech uloženia disku.
     */
    @Override
    public boolean putDisk(Disk disk)
    {
            this.disk=disk;
            return true;         
    }
    
}
