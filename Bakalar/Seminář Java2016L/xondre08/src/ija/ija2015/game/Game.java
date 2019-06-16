/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package ija.ija2015.game;

import ija.ija2015.board.Board;
import ija.ija2015.board.Disk;

/**
 *
 * @author xondre08
 * @author xkeryj00
 */
public class Game {
    private Board Board;
    private Player playerWhite;
    private Player playerBlack;
    private Player activePlayer;
    
    /**
     * Inicializuje hru.
     * @param board - Hracia doska, ktorá je súčasťou hry.
     */
    public Game(Board board){
        this.Board=board;
    }
    /**
     * Pridá hráča a súčasne vyvolá jeho inicializáciu.
     * Ak hráč danej faby už existuje, neurobí nič a vráti false.
     * @param player - Pridávaný hráč.
     * @return Úspech pridania.
     */
    public boolean addPlayer(Player player){
        player.init( Board);
        if(player.isWhite())
            if(playerWhite==null)
            {
                playerWhite=player;
                activePlayer=player;
                return true;
            }   
            else
            {
                return false;
            }
        else
        {
            if(playerBlack==null)
            {
                playerBlack=player;
                return true;
            }   
            else
            {
                return false;
            }
        }
    }
    
    /**
     * Vráti aktuálneho hráča, ktorý je na ťahu.
     * @return Aktuálny hráč.
     */
    public Player currentPlayer(){
        return activePlayer;

    }
    /**
     * Vráti hraciu dosku.
     * @return Hracia doska.
     */
    public Board getBoard(){
        return Board;

    }
    /**
     * Otestuje či je možné vložiť aspoň na jedno poličko.
     * @param Player - Hrač, ktorý sa bude testovat.
     * @return Vracia úspech testu.
     */
    public boolean testPlayerCanPutAnyDisc(Player Player)
    {
        for(int i=0;i<Board.getSize();i++)
        {
            for(int j=0;j<Board.getSize();j++)
            {
                if(Player.canPutDisk(Board.getField(i, j)))
                {
                    return true;
                }
            }
        }
        return false;
    }
    /**
     * Zmena aktuálneho hráča.
     * @return Aktuálne nastavený hráč.
     */
    public Player nextPlayer(){
        if(activePlayer==null){
            return playerWhite;
        }
        else{
            if(activePlayer==playerWhite){
                return activePlayer=playerBlack;
            }
            else
            {
                return activePlayer=playerWhite;
            }
        }
    }
    /**
     * Aktívny hráč si "zoberie" disk, zatiaľ čo protivník položí svoj disk.
     */
    public void anyPlayerCantPut(){
        if(activePlayer==playerWhite){
            Disk disk = playerWhite.getDisk();
            playerBlack.addDisk(disk);
        }
        else
        {
            Disk disk = playerBlack.getDisk();
            playerWhite.addDisk(disk);
        }
        
    }
    @Override
    public String toString() {
        return "Game{\n" + ", activePlayer=" + activePlayer + "\nBoard:\n" + Board +  "\n}";
    }
    
}
