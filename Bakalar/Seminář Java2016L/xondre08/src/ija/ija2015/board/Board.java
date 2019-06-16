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
public class Board {
    private int size; 
    private Field[][] pole;
    private Rules Rules;

    /**
     *
     * @param rules Objekt pravidla, pre vytvorenie hracej dosky.
     */
    public Board(Rules rules) {
        this.Rules=rules;
        this.size = rules.getSize();
        pole=new Field[size+2][size+2];
        for(int i=0;i<size+2;i++)
        {
            for(int j=0;j<size+2;j++)
            {
                    pole[i][j]=rules.createField(i, j);   
            }
        }
        for(int i=1;i<size+1;i++)
        {
            for(int j=1;j<size+1;j++)
            { 
                    pole[i][j].addNextField(Field.Direction.RD, pole[i+1][j+1]);
                    pole[i][j].addNextField(Field.Direction.D, pole[i+1][j]);
                    pole[i][j].addNextField(Field.Direction.LD, pole[i+1][j-1]);
                    pole[i][j].addNextField(Field.Direction.RU, pole[i-1][j+1]);
                    pole[i][j].addNextField(Field.Direction.U, pole[i-1][j]);
                    pole[i][j].addNextField(Field.Direction.LU, pole[i-1][j-1]);
                    pole[i][j].addNextField(Field.Direction.R, pole[i][j+1]);
                    pole[i][j].addNextField(Field.Direction.L, pole[i][j-1]); 
            }
        }
    }
    
    /**
     * Vráti pole umiestnené na uvedených indexoch row a col.
     * @param row Riadok
     * @param col Stĺpec
     * @return Pole na pozícii [row][col].
     */
    public Field getField(int row,int col)
    {
        if(row>=0&&row<=size+1&&col>=0&&col<=size+1)
            return this.pole[row][col];
        else
            return null;
    }
    
    /**
     * Vracia objekt pravidiel.
     * @return Pravidla inicializácie hry.
     */
    public Rules getRules()
    {
        return this.Rules;
    }
    
    /**
     * Vracia veľkosť (rozmer) dosky.
     * @return Veľkosť dosky.
     */
    public int getSize()
    {
        return size;
    }

    @Override
    public String toString() {
        String str="";
        for(int j =0;j<getSize()+2;j++)
        {
            for(int i =0;i<getSize()+2;i++)
            {
                if(pole[i][j].getDisk()==null){
                    if(i==0||j==0||i==getSize()+1||j==getSize()+1)
                    {
                    }
                    else
                    {
                        str+="-";
                    }
                }
                else{
                    if(pole[i][j].getDisk().isWhite()){
                        str+="O";
                    }
                    else {
                        str+="X";
                    }
                }
            }
        }
         return str;
    }
    
    /**
     *
     * @return String str.
     */
    public String toString2() {
        String str="";
        for(int j =0;j<getSize()+2;j++)
        {
            for(int i =0;i<getSize()+2;i++)
            {
                if(pole[i][j].getDisk()==null){
                    if(i==0||j==0||i==getSize()+1||j==getSize()+1)
                    {
                    }
                    else
                    {
                        str+="-";
                    }
                }
                else{
                    if(pole[i][j].getDisk().isWhite()){
                        str+="O";
                    }
                    else {
                        str+="X";
                    }
                }
            }
            str+="\n";
        }
         return str;
    }
    
    private String okraj()
    {
        String str = "";
        str+="\n";
                for(int h =0; h<(getSize()+2)*2+1;h++){
                    if(h%2!=0){
                        str+="---";
                    }
                    else{
                        str+="+";
                    }
                }
                str+="\n";
        return str;
    }

    /**
     *
     * @param fields Polia fields(X || O).
     */
    public void setField(String fields) {
        int j,i;
        for(int k = 0;k<fields.length();k++){
            j = (k/(int)Math.sqrt((double)fields.length()));
            i = (k%(int)Math.sqrt((double)fields.length()));
                switch(fields.charAt(k)){
                    case 'X':
                        this.pole[i+1][j+1].putDisk(new Disk(false));
                        break;
                    case 'O':
                        this.pole[i+1][j+1].putDisk(new Disk(true));
                        break;
                    default:
                        this.pole[i+1][j+1].putDisk(null);
                        break;
                }
        }
    }
}