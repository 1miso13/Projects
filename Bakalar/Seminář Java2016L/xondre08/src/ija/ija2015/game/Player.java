/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package ija.ija2015.game;

import ija.ija2015.board.Board;
import ija.ija2015.board.BoardField;
import ija.ija2015.board.Disk;
import ija.ija2015.board.Field;
import static ija.ija2015.board.Field.Direction;
import java.util.ArrayList;
import java.util.List;

/**
 *
 * @author xondre08
 * @author xkeryj00
 */
public class Player {
    private boolean isWhite;
    private ArrayList<Disk> Pool;
    private int lastX=0;
    private int lastY=0;
                private int helpX = 0;
            private int helpY = 0;
    @Override
    public String toString() {
        if(isWhite)
            return "WHITE"; 
        else
            return "BLACK";
    }

    /**
     * Inicializácia hráča.
     * @param isWhite - Určuje farbu hráča (true = biely, false = čierny)
     */
    public Player(boolean isWhite) {
        this.isWhite = isWhite;
        Pool= new ArrayList<>();
    }
    /**
     * Zisti či sú v danom smere žetóny opačnej farby 
     * až dokým nenatrafí na žetón rovnakej farby.
     * @param direction Smer, ktorým sa bude vyhľadávať.
     * @param field Pozícia, od ktorej sa začína vyhľadávanie.
     * @return Úspešnosť akcie.
     */
    public boolean FindToDirection(Direction direction ,Field field)
    {
        Field nextField;
        
        if((nextField = field.nextField(direction)).getDisk()==null || nextField.getDisk().isWhite()==isWhite)//najblizsie musi byt opacnej farby
        {
            return false;
        }
        while((nextField = nextField.nextField(direction)).getDisk()!=null)
        {
               
            if(nextField.getDisk().isWhite()==isWhite){
                
                return true;
            }
        }
        return false;
    }
    /**
     * Test, či je možné vložiť nový žetón hráča na dané pole. 
     * Žetón sa zo sady nevyberá a ani nevkladá na pole.
     * @param field Pole, na ktoré sa má vložiť žetón.
     * @return Možnosť vloženia žetóna na pole.
     */
    public boolean canPutDisk(Field field){
        if(field.getDisk()!=null || field.getClass()!=BoardField.class){
            return false;
        }
        
        boolean isPossible=false;
        for (Direction dir : Direction.values()) {
            if(FindToDirection(dir,field))
            {
                isPossible=true;
            }
        }
        return isPossible;
    }
    /**
     * Test prázdnosti sady žetónov, ktoré má hráč k dispozícií.
     * @return Vracia true/false, či je sada prázdna.
     */
    public boolean emptyPool(){
        return Pool.isEmpty();

    }
   /**
    * Highest-Value AI. 
    * Získane pole otestujeme na množstvo možných obrátených žetónov protívníka.
    * @param field Pole, ktoré testujeme pre možnosť uloženia žetónu.
    * @return Vracia počet možných obrátení žetónov.
    */
    public int HighestValue(Field field)
    {
        int solution = 0;
        for (Direction dir : Direction.values()) {
            if(FindToDirection(dir,field))
            {//na smer dir mozeme otacat 
                Field nextField=field.nextField(dir);
                do
                {
                   solution = solution + 1;
                }while(((nextField=nextField.nextField(dir)).getDisk()).isWhite()!=isWhite);
            }
        }
        return solution;
    }
    /**
     *
     * @param board Hracia doska.
     * @param typeOfAI  - 1 je zakladny (Highest-Value)
     *                  - 2 je Random
     * @return True ak je koniec hry.
     */
    public boolean autoAI(Board board,int typeOfAI){
        int size=board.getSize();
        boolean s=true; 
        if(typeOfAI==2){
            List <Field> fieldArray=new ArrayList();
            for(int i =1;i<=size;i++){
                for(int j =1;j<=size;j++){
                    if(this.canPutDisk(board.getField(i, j)))
                        fieldArray.add(board.getField(i, j));
                    if(board.getField(i, j).getDisk()==null)
                        s=false;
                }
            }
            Field f=fieldArray.get((int)(Math.random() * fieldArray.size()));
            lastX=f.getRow();
            lastY=f.getCol();
            putDisk(f);
        } 
        else if(typeOfAI == 1)
        {
              List <Field> fieldArray2=new ArrayList(); // tu sa ulozia vsetky mozne fields kde sa da polozit
                for(int i =1;i<=size;i++)
                {
                    for(int j =1;j<=size;j++)
                    {
                        if(this.canPutDisk(board.getField(i, j)))
                            fieldArray2.add(board.getField(i, j)); // postupne ich tam vkladame
                        if(board.getField(i, j).getDisk()==null)
                            s=false;
                    }
                }
                int yay = 0; // premenna pre vyslednu najvyssiu hodnotu
                int atta;
                Field zing = null;
                for (int i = 0; i < fieldArray2.size(); i++) 
                {
                    Field ff = fieldArray2.get(i); // zobrali sme si jeden z moznosti
                    atta = HighestValue(ff);
                    if (yay < atta)
                    {
                        yay = atta;
                        zing = ff;
                    }
                }
                lastX = zing.getRow();
                lastY = zing.getCol();
                putDisk(zing);
        }
        return s;
        
    }
    /**
     * Inicializácia hráča v rámci hracej dosky.
     * Vytvorí sadu žetónov o príslušnej veľkosti a
     * umiestni počiatočné žetóny na dosku.
     * @param board - Doska, v rámci kterej sa hráč inicializuje.
     */
    public void init(Board board){
        
        //board;
        for(int i=0;i<board.getRules().numberDisks();i++)
        {
            Pool.add(new Disk(isWhite));
        }
        if(isWhite)
        {
            board.getField(board.getSize()/2, board.getSize()/2).putDisk(Pool.remove(Pool.size()-1));
            board.getField(board.getSize()/2+1, board.getSize()/2+1).putDisk(Pool.remove(Pool.size()-1));
        }
        else
        {
            board.getField(board.getSize()/2, board.getSize()/2+1).putDisk(Pool.remove(Pool.size()-1));
            board.getField(board.getSize()/2+1, board.getSize()/2).putDisk(Pool.remove(Pool.size()-1));
        }
    }
    
    /**
     * V prípade, že hráč nemôže vložiť disk, odoberie sa mu 1 žetón,
     * zmení sa farba a pridá sa druhému hráčovi.
     * @return Vracia sa upravená sada žetónov.
     */
    public Disk getDisk() {
        if(Pool.isEmpty())
            return null;
        return Pool.remove(Pool.size()-1);
    }

    /**
     * V prípade, že hráč nemôže vložiť disk, odoberie sa mu 1 žetón,
     * zmení sa farba a pridá sa druhému hráčovi.
     * @param Disk Pridanie žetónu do sady.
     */
    public void addDisk(Disk Disk) {
        Pool.add(new Disk(isWhite));
    }
    /**
     * Test farby hráča.
     * @return Vracia, či je hráč biely.
     */
    public boolean isWhite(){
        return isWhite;
    }
    /**
     * Vloží nový žetón hráča na dané pole, ak to pravidla umožnujú.
     * Ak je možné vložiť, zoberie žetón zo sady a položí na dané pole.
     * @param field Pole, na ktoré sa ukladá žetón.
     * @return Úspech akcie.
     */
    public boolean putDisk(Field field){
        if(!this.canPutDisk(field)|| emptyPool())
            return false;
        //vieme ze mozeme vlozit, tak musime postupne otacat
        field.putDisk(Pool.remove(Pool.size()-1));
        for (Direction dir : Direction.values()) {
            if(FindToDirection(dir,field))
            {//na smer dir mozeme otacat 
                Field nextField=field.nextField(dir);
                Disk disc;
                do
                {
                    if(nextField.getDisk().freezed)
                        break;
                    else
                        nextField.getDisk().turn();
                }while((disc=(nextField=nextField.nextField(dir)).getDisk()).isWhite()!=isWhite && !disc.freezed);
            }
        }
        return  true;
    }

    /**
     *
     * @return Vracia súradnicu Y posledne položeného žetónu.
     */
    public int getLastY() {
        return lastY;
    }

    /**
     *
     * @return Vracia súradnicu X posledne pooloženého žetónu.
     */
    public int getLastX() {
        return lastX;
    }

    /**
     * Navyšenie sady žetónov.
     */
    public void IncrasePool() {
        Pool.add(new Disk(isWhite));
    }
}
