/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package ija.ija2015.windows;

import ija.ija2015.board.Board;
import ija.ija2015.board.Disk;
import ija.ija2015.game.Game;
import ija.ija2015.game.Player;
import ija.ija2015.game.ReversiRules;
import ija.ija2015.windows.s.Button;
import java.awt.Color;
import java.awt.Image;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.StandardOpenOption;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import javax.swing.BorderFactory;
import javax.swing.ImageIcon;
import javax.swing.JOptionPane;
import javax.swing.Timer;
import javax.swing.border.BevelBorder;

/**
 * 
 * @author xondre08
 * @author xkeryj00
 */
public final class GameWindow extends javax.swing.JFrame {
    /**
     * Size Parameter o danej veľkosti hracej plochy.
     */
    private int size;
    /**
     * typeOfOponent Parameter udávajúci typ oponenta(AI || druhý hráč).
     */
    private int typeOfOponent;
    /**
     * I Parameter udávajúci čas, po ktorom nastane zamrznutie žetónov.
     */
    private float I;
    /**
     * B Parameter udávajúci čas, ako dlho budú žetóny zamrznuté.
     */
    private float B;
    /**
     * C Parameter udávajúci počet zamrznutých žetónov.
     */
    private int C;
    /**
     * PutDiskField Ikona zobrazujúca políčko vhodné 
     * na položenie daného žetónu.
     */
    private ImageIcon PutDiskField;
    /**
     * EmptyField Ikona zobrazujúca prázdne políčko.
     */
    private ImageIcon EmptyField;
    /**
     * blackField Ikona zobrazujúca políčko 
     * zabrané čierným žetónom.
     */
    private ImageIcon blackField;
    /**
     * whiteField Ikona zobrazujúca políčko
     * zabrané bielym žetónom.
     */
    private ImageIcon whiteField;
    /**
     * PutDiskField_Changed Ikona PutDiskField so 
     * zmenenou veľkosťou.
     */
    private ImageIcon PutDiskField_Changed;
    /**
     * EmptyField_Changed Ikona EmptyField so 
     * zmenenou veľkosťou.
     */
    private ImageIcon EmptyField_Changed;
    /**
     * blackField_Changed Ikona blackField so 
     * zmenenou veľkosťou. 
     */
    private ImageIcon blackField_Changed;
    /**
     * whiteField_Changed Ikona whiteField so 
     * zmenenou veľkosťou. 
     */
    private ImageIcon whiteField_Changed;
    /**
     * FieldButton Pole tlačidiel na hracej ploche.
     */
    private Button[][] FieldButton;
    /**
     * game Aktuálna hra. 
     */
    private Game game;
    /**
     * endOfGame Parameter označujúci koniec hry. 
     */
    private boolean endOfGame=false;
    /**
     * lastX Parameter označujúci súradnicu X 
     * posledného kliknutého políčka. 
     * lastY Parameter označujúci súradnicu Y
     * posledného kliknutého políčka.
     */
    private int lastX=0,lastY=0;
    /**
     * lastXBack Parameter označujúci súradnicu X
     * predposledného kliknutého políčka.
     * lastYBack Parameter označujúci súradnicu Y
     * predposledného kliknutého políčka.
     */
    private int lastXBack=0,lastYBack=0;
    /**
     * whiteDiskCount Parameter označujúci počet
     * bielých žetónov na hracej doske.
     * blackDiskCount Parameter označujúci počet
     * čierných žetónov na hracej doske.
     */
    private int whiteDiskCount=0,blackDiskCount=0;
    /**
     * fieldBack Parameter označujúci, či bol
     * na danom políčku čierný alebo bielý žetón.
     */
    private String fieldBack="";
    /**
     * timer Čas reprezentujúci dobu
     * medzi dvoma zamrznutiami.
     */
    private Timer timer;
    /**
     * GameTime Čas reprezentujúci dĺžku
     * rozohratej hry.
     */
    private Timer GameTime;
    /**
     * freezing Parameter určujúci, či bude dochádzať
     * k zamŕzaniu žetónov.
     */
    private boolean freezing=false;
    /**
     * listOfDIsabled Zoznam zamrznutých žetónov.
     */
    private final List<Button> listOfDIsabled=new  ArrayList();
    /**
     * time Pomocná premenná pre GameTime, určujúca
     * koľko sekund prešlo od začiatku hry.
     */
    private int time=0;

    /**
     * Vytvorenie Fields.
     */
    public void CreatedFields(){
        
        LoadFieldsIcons();
        generateFields();
        setFields();
    }

    /**
     * Generovanie Fields.
     */
    public void generateFields(){
        int sizeA=(jPanelFields.getSize().width/size);
        int sizeB=(jPanelFields.getSize().height/size);
        EmptyField=new ImageIcon(EmptyField.getImage().getScaledInstance(sizeA-2, sizeB-2, Image.SCALE_AREA_AVERAGING));
        whiteField=new ImageIcon(whiteField.getImage().getScaledInstance(sizeA-2, sizeB-2, Image.SCALE_AREA_AVERAGING));
        blackField=new ImageIcon(blackField.getImage().getScaledInstance(sizeA-2, sizeB-2, Image.SCALE_AREA_AVERAGING));
        PutDiskField=new ImageIcon(PutDiskField.getImage().getScaledInstance(sizeA-2, sizeB-2, Image.SCALE_AREA_AVERAGING));
        FieldButton = new Button[size][size];
        for(int i=0;i<size;i++){
            for(int j=0;j<size;j++){
               final Button d = new Button(i+1, j+1);
               d.addMouseListener(new MouseAdapter() {
                    
                    @Override
                    public void mouseClicked(MouseEvent e) {
                        if(e.isPopupTrigger() || e.getButton() == MouseEvent.BUTTON1){
                            jButtonFieldMouseClicked(d);
                        }
                    }
                });
                d.setIcon(EmptyField);
                d.setSize(sizeA, sizeB);
                d.setBounds(i*sizeA, j*sizeB, sizeA, sizeB);
                d.setBorder(BorderFactory.createBevelBorder(2));
                d.validate();
                FieldButton[i][j]=d;
            }
        }
            
    }

    /**
     * Zmena veľkosti Fields.
     */
    public void resizeFields(){
        int sizeA=(jPanelFields.getSize().width/size);
        int sizeB=(jPanelFields.getSize().height/size);
        EmptyField_Changed=new ImageIcon(EmptyField.getImage().getScaledInstance(sizeA-2, sizeB-2, Image.SCALE_SMOOTH));
        whiteField_Changed=new ImageIcon(whiteField.getImage().getScaledInstance(sizeA-2, sizeB-2, Image.SCALE_SMOOTH));
        blackField_Changed=new ImageIcon(blackField.getImage().getScaledInstance(sizeA-2, sizeB-2, Image.SCALE_SMOOTH));
        PutDiskField_Changed=new ImageIcon(PutDiskField.getImage().getScaledInstance(sizeA-2, sizeB-2, Image.SCALE_SMOOTH));
        //arrayFields = new Button[size][size];
        for(int i=0;i<size;i++){
            for(int j=0;j<size;j++){
                FieldButton[i][j].setSize(sizeA, sizeB);
                FieldButton[i][j].setBounds(i*sizeA, j*sizeB, sizeA, sizeB);
                FieldButton[i][j].setBorder(BorderFactory.createBevelBorder(2));
                FieldButton[i][j].validate();
            }
        }           
    }

    /**
     * Nastavenie Fields.
     */
    public void setFields(){
        jPanelFields.removeAll();
        for(int i=0;i<size;i++){
           for(int j=0;j<size;j++){
                if(i+1==lastX&&j+1==lastY){
                    FieldButton[i][j].setBorder(BorderFactory.createBevelBorder(BevelBorder.LOWERED, Color.RED, Color.RED));
                    
                }
                else
                {
                    FieldButton[i][j].setBorder(BorderFactory.createEmptyBorder());
                }
                if(game.getBoard().getField(i+1, j+1).getDisk()==null){
                    if(game.currentPlayer().canPutDisk(game.getBoard().getField(i+1, j+1))){
                        FieldButton[i][j].setIcon(PutDiskField_Changed);
                    }
                    else{
                        FieldButton[i][j].setIcon(EmptyField_Changed);
                    }
                }
                else
                {
                    if(game.getBoard().getField(i+1, j+1).getDisk().isWhite()){
                        
                        FieldButton[i][j].setIcon(whiteField_Changed);

                    }
                    else
                    {
                        FieldButton[i][j].setIcon(blackField_Changed);
                    }
                
                } 
                jPanelFields.add(FieldButton[i][j]);
           }
        }
        this.repaint();
    }

    /**
     * Načitanie obrázkov.
     */
    public void LoadFieldsIcons(){
            PutDiskField = new ImageIcon("lib/backgroundFree.jpg");
            EmptyField = new ImageIcon("lib/background.jpg");
            blackField = new ImageIcon("lib/backgroundBlack.jpg");
            whiteField = new ImageIcon("lib/backgroundWhite.jpg");
            if(PutDiskField.getImage() == null || EmptyField.getImage() == null || blackField.getImage() == null || whiteField.getImage() == null) {
                JOptionPane.showMessageDialog(null, "Neboli nájdené súbory.");
                this.dispose();
            }
    }
    /**
     * Inicializácia hry.
     */
    private void init() {
        initComponents();
        whiteDiskCount=2;
        blackDiskCount=2;
        game=new Game(new Board(new ReversiRules(size)));
        game.addPlayer(new Player(false));
        game.addPlayer(new Player(true));
        game.nextPlayer();
        CreatedFields();
        GameTime=new Timer(1000, new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                time+=1;
                int seconds=time%60;
                int minutes=(time/60)%60;
                int hours=((time/60)/60);
                jLabelTime.setText((hours>9?hours:"0"+hours)+":"+(minutes>9?minutes:"0"+minutes)+":"+(seconds>9?seconds:"0"+seconds));
                
            }
        });
        GameTime.start();
        
        
    }
    /**
     * Creates new form GameWindow
     * @param size Veľkosť hracej plochy.
     * @param typeOfOponent Typ oponenta(AI || druhý hráč).
     */
    public GameWindow(int size,int typeOfOponent) {
        this.size = size;
        this.typeOfOponent = typeOfOponent;
        init();
    }
    /**
     *
     * @param size Veľkosť hracej plochy.
     * @param B Časový interval, počas ktorej sú zamrznuté žetóny.
     * @param C Počet žetónov.
     * @param typeOfOponent Typ oponenta(AI || druhý hráč).
     * @param I Časovy interval, po uplynutí sa zmrazia žetóny.
     */
    public GameWindow(final int size,int typeOfOponent,float I,final float B,final int C) {
        this(size,typeOfOponent);
        this.I = I;
        this.B = B;
        this.C = C;
        nastavZamrzanie();
    }
    /**
     * Nastavovanie zamrzania
     */
    public void nastavZamrzanie(){
        if(I>0){
            freezing=true;
            timer=new Timer((int)(I+B)*1000, new ActionListener() {
                private int pocetOpacnych;
                private Timer timer2;
                @Override
                public void actionPerformed(ActionEvent e) {
                    //zistime si pocet diskov ktore ma nepriatel, a do premennej d ulozime danu hodnotu alebo hodnotu C
                    pocetOpacnych=(game.currentPlayer().isWhite()?whiteDiskCount:blackDiskCount);
                    int d=(C<pocetOpacnych?C:pocetOpacnych);
                    //pridam si do pola vsetky a potom nahodno vybereme 
                    List<Button> listOfButtons=new  ArrayList();
                    for(int j=0;j<size;j++){
                        for(int i=0;i<size;i++){
                            Disk disk;
                            //zitovanie ci nieje null
                            if((disk=game.getBoard().getField(i+1, j+1).getDisk())!=null)
                                //ci je opacne 
                                if(game.currentPlayer().isWhite()!=disk.isWhite())
                                    //ak vyhovuje, tak pridame do pola 
                                    listOfButtons.add(FieldButton[i][j]);
                        }
                    }
                    for(int i =0;i<d;i++){
                        //nahodne vybereme pocet d z pola listOfButtons
                        listOfDIsabled.add(listOfButtons.remove((int )(Math.random() * (listOfButtons.size()-1))));
                        try {
                            game.getBoard().getField(listOfDIsabled.get(i).getI(), listOfDIsabled.get(i).getJ()).getDisk().freezed=true;
                        } catch (Exception ee) {
                        }
                        listOfDIsabled.get(i).setEnabled(false);
                    }
                    setFields();
                    //2. timer na odmrznutie
                    timer2=new Timer((int)B*1000, new ActionListener() {
                        @Override
                        public void actionPerformed(ActionEvent e) {
                            try {
                                for(int i =listOfDIsabled.size()-1;i>=0;i--){
                                    Button button=listOfDIsabled.remove(i);
                                    button.setEnabled(true);
                                    try {
                                        game.getBoard().getField(button.getI(), button.getJ()).getDisk().freezed=false;
                                        } catch (Exception ee) {
                                        }
                                }
                                setFields();
                                timer2.stop();
                            } catch (Exception ea) {
                            }

                        }
                    });
                    timer2.start();         
                }
            });
        timer.start();
    }
    }
 
    /**
     *
     * @param s Parameter, kde sú uložené data z hry.
     */
    public GameWindow(String s){
             
        List<String> allMatches = new ArrayList<>();
        Matcher m = Pattern.compile("[^\n]+").matcher(s);
        while (m.find()) {
            String ssss=m.group();
          allMatches.add(ssss);
        }
        
        this.size = Integer.parseInt(allMatches.get(0));
        init();
        this.typeOfOponent = Integer.parseInt(allMatches.get(1));
        this.I = Float.parseFloat(allMatches.get(2));
        this.B = Float.parseFloat(allMatches.get(3));
        this.C = Integer.parseInt(allMatches.get(4));

        if(!"BLACK".equals(allMatches.get(5))){
            game.nextPlayer();
        }
        
        lastX=Integer.parseInt(allMatches.get(6));
        lastY=Integer.parseInt(allMatches.get(7));
        String fields="";
        for(int i =8;i<size+8;i++){
            fields+=allMatches.get(i);
        }
        game.getBoard().setField(fields);
        setFields();
        nastavZamrzanie();
         //nastavime skore
        blackDiskCount=0;
        whiteDiskCount=0;
        for(int o=1;o<size+1;o++){
            for(int n=1;n<size+1;n++){
                if(game.getBoard().getField(o, n).getDisk()!=null){
                    if(game.getBoard().getField(o, n).getDisk().isWhite()){
                        whiteDiskCount++;
                    }
                    if(!game.getBoard().getField(o, n).getDisk().isWhite()){
                        blackDiskCount++;
                    }
                }
            }
        }
        jLabelScorePlayer1.setText("Čierny:  "+blackDiskCount);
        jLabelScorePlayer2.setText("Biely:     "+whiteDiskCount);
    }
    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        jSeparator2 = new javax.swing.JSeparator();
        jPanel1 = new javax.swing.JPanel();
        jLabelScorePlayer1 = new javax.swing.JLabel();
        jLabelScorePlayer2 = new javax.swing.JLabel();
        jPanel2 = new javax.swing.JPanel();
        jLabelTime = new javax.swing.JLabel();
        jButtonBack = new javax.swing.JButton();
        jButtonEnd = new javax.swing.JButton();
        jPanelFields = new javax.swing.JPanel();
        jPanel3 = new javax.swing.JPanel();
        jLabel2 = new javax.swing.JLabel();
        jButtonSave = new javax.swing.JButton();

        setDefaultCloseOperation(javax.swing.WindowConstants.DISPOSE_ON_CLOSE);
        setTitle("Othello - Game");
        addComponentListener(new java.awt.event.ComponentAdapter() {
            public void componentResized(java.awt.event.ComponentEvent evt) {
                formComponentResized(evt);
            }
        });
        addWindowStateListener(new java.awt.event.WindowStateListener() {
            public void windowStateChanged(java.awt.event.WindowEvent evt) {
                formWindowStateChanged(evt);
            }
        });

        jSeparator2.setOrientation(javax.swing.SwingConstants.VERTICAL);

        jPanel1.setBorder(javax.swing.BorderFactory.createTitledBorder("Skóre:"));

        jLabelScorePlayer1.setText("Čierny:  2");

        jLabelScorePlayer2.setText("Biely:     2");

        javax.swing.GroupLayout jPanel1Layout = new javax.swing.GroupLayout(jPanel1);
        jPanel1.setLayout(jPanel1Layout);
        jPanel1Layout.setHorizontalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel1Layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jLabelScorePlayer2, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(jLabelScorePlayer1, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                .addContainerGap())
        );
        jPanel1Layout.setVerticalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel1Layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jLabelScorePlayer1)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addComponent(jLabelScorePlayer2)
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        jPanel2.setBorder(javax.swing.BorderFactory.createTitledBorder("Čas:"));

        jLabelTime.setText("00:00:00");

        javax.swing.GroupLayout jPanel2Layout = new javax.swing.GroupLayout(jPanel2);
        jPanel2.setLayout(jPanel2Layout);
        jPanel2Layout.setHorizontalGroup(
            jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel2Layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jLabelTime, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addContainerGap())
        );
        jPanel2Layout.setVerticalGroup(
            jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel2Layout.createSequentialGroup()
                .addComponent(jLabelTime)
                .addGap(0, 11, Short.MAX_VALUE))
        );

        jButtonBack.setText("Krok späť");
        jButtonBack.setEnabled(false);
        jButtonBack.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButtonBackActionPerformed(evt);
            }
        });

        jButtonEnd.setText("Koniec");
        jButtonEnd.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButtonEndActionPerformed(evt);
            }
        });

        jPanelFields.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jPanelFieldsMouseClicked(evt);
            }
        });

        javax.swing.GroupLayout jPanelFieldsLayout = new javax.swing.GroupLayout(jPanelFields);
        jPanelFields.setLayout(jPanelFieldsLayout);
        jPanelFieldsLayout.setHorizontalGroup(
            jPanelFieldsLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 439, Short.MAX_VALUE)
        );
        jPanelFieldsLayout.setVerticalGroup(
            jPanelFieldsLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 0, Short.MAX_VALUE)
        );

        jPanel3.setBorder(javax.swing.BorderFactory.createTitledBorder("Aktívny hráč:"));

        jLabel2.setText("Hráč:   Čierny    ");

        javax.swing.GroupLayout jPanel3Layout = new javax.swing.GroupLayout(jPanel3);
        jPanel3.setLayout(jPanel3Layout);
        jPanel3Layout.setHorizontalGroup(
            jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel3Layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jLabel2, javax.swing.GroupLayout.DEFAULT_SIZE, 88, Short.MAX_VALUE))
        );
        jPanel3Layout.setVerticalGroup(
            jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel3Layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jLabel2)
                .addContainerGap(18, Short.MAX_VALUE))
        );

        jButtonSave.setText("Uložiť hru");
        jButtonSave.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButtonSaveActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addComponent(jPanelFields, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jSeparator2, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addGap(27, 27, 27)
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                            .addComponent(jPanel1, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                            .addComponent(jPanel2, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                            .addComponent(jPanel3, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)))
                    .addGroup(layout.createSequentialGroup()
                        .addGap(18, 18, 18)
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(jButtonEnd, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.PREFERRED_SIZE, 184, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(jButtonBack, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.PREFERRED_SIZE, 184, javax.swing.GroupLayout.PREFERRED_SIZE)))
                    .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jButtonSave, javax.swing.GroupLayout.PREFERRED_SIZE, 184, javax.swing.GroupLayout.PREFERRED_SIZE)))
                .addContainerGap())
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(jSeparator2, javax.swing.GroupLayout.Alignment.TRAILING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jPanel2, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(18, 18, 18)
                .addComponent(jPanel1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jPanel3, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 128, Short.MAX_VALUE)
                .addComponent(jButtonBack, javax.swing.GroupLayout.PREFERRED_SIZE, 54, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(18, 18, 18)
                .addComponent(jButtonSave, javax.swing.GroupLayout.PREFERRED_SIZE, 62, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(22, 22, 22)
                .addComponent(jButtonEnd, javax.swing.GroupLayout.PREFERRED_SIZE, 51, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap())
            .addComponent(jPanelFields, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
        );

        jPanel3.getAccessibleContext().setAccessibleName("Actual Player\n");

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void formWindowStateChanged(java.awt.event.WindowEvent evt) {//GEN-FIRST:event_formWindowStateChanged
        
    }//GEN-LAST:event_formWindowStateChanged

    private void jPanelFieldsMouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jPanelFieldsMouseClicked
        
    }//GEN-LAST:event_jPanelFieldsMouseClicked
    /**
     * Akcia stlačenia tlačidla polička.
     * @param button Dane tlačidlo.
     */
    private void jButtonFieldMouseClicked(Button button) {    
        if(!endOfGame){
            whiteDiskCount=0;blackDiskCount=0;
            int i=button.getI();
            int j=button.getJ();
            boolean canPut=false,konec=true;
            String fieldBack1=game.getBoard().toString();
            if(game.currentPlayer().putDisk(game.getBoard().getField(i, j))){
                if(freezing){
                    listOfDIsabled.clear();
                    for(int m=0;m<size;m++){
                        for(int n=0;n<size;n++){
                            FieldButton[n][m].setEnabled(true);
                            Disk ddd;
                            if((ddd=game.getBoard().getField(n+1, m+1).getDisk())!=null)
                                ddd.freezed=false;
                        }
                    }
                    timer.restart();
                }
                fieldBack=fieldBack1;
                lastXBack=lastX;
                lastYBack=lastY;
                lastX=i;
                lastY=j;
                jButtonBack.setEnabled(true);
                game.nextPlayer();
                for(int m=1;m<size+1;m++){
                    for(int n=1;n<size+1;n++){
                        if(game.currentPlayer().canPutDisk(game.getBoard().getField(m, n))){
                            canPut=true;
                        }
                        if(game.getBoard().getField(m, n).getDisk()!=null){
                            if(game.getBoard().getField(m, n).getDisk().isWhite()){
                                whiteDiskCount++;
                            }
                            if(!game.getBoard().getField(m, n).getDisk().isWhite()){
                                blackDiskCount++;
                            }
                        }
                        else{
                            konec=false;
                        }
                    }
                }
                if(!canPut){
                    game.anyPlayerCantPut();
                    game.nextPlayer();
                    if(!game.testPlayerCanPutAnyDisc(game.currentPlayer())){
                        konec=true;
                    }
                }
                else
                if(typeOfOponent==2||typeOfOponent==1){
                    if(game.testPlayerCanPutAnyDisc(game.currentPlayer())){
                        konec=game.currentPlayer().autoAI(game.getBoard(),typeOfOponent);
                        lastX=game.currentPlayer().getLastX();
                        lastY=game.currentPlayer().getLastY();
                        if(konec!=true)
                            game.nextPlayer();//da sa hrac
                        while(!konec&&!game.testPlayerCanPutAnyDisc(game.currentPlayer())){
                            game.anyPlayerCantPut();//zobere hracovi 
                            game.nextPlayer();//da sa AI
                            if(!game.testPlayerCanPutAnyDisc(game.currentPlayer())){
                                konec=true;
                            }
                            else
                                konec=game.currentPlayer().autoAI(game.getBoard(),typeOfOponent);

                            if(konec!=true)
                                game.nextPlayer();//da sa hrac
                        }//pokym oponent nevie davat
                        whiteDiskCount = 0; blackDiskCount = 0;
                        for(int m=1;m<size+1;m++){
                            for(int n=1;n<size+1;n++){
                                if(game.getBoard().getField(m, n).getDisk()!=null){
                                    if(game.getBoard().getField(m, n).getDisk().isWhite()){
                                        whiteDiskCount++;
                                    }
                                    if(!game.getBoard().getField(m, n).getDisk().isWhite()){
                                        blackDiskCount++;
                                    }
                                }
                                else{
                                    konec=false;
                                }
                            }
                        }
                    }
                    else
                        game.nextPlayer();//nikdy sa nedostane lebo ho to vyhodi uz na canPut
                }
                
                
                jLabelScorePlayer1.setText("Čierny:  "+blackDiskCount);
                jLabelScorePlayer2.setText("Biely:     "+whiteDiskCount);
                setFields();
                jLabel2.setText(game.currentPlayer().isWhite()?"Hráč:   Biely    ":"Hráč:   Čierny    ");

                //ak niektory z hracov ma 0 tak je koniec hry
               if(blackDiskCount==0 || whiteDiskCount==0 || konec){
                    //nastal koniec hry
                    JOptionPane.showMessageDialog(null, "Koniec hry! "+(whiteDiskCount!=blackDiskCount? "Vyhral "+(whiteDiskCount<blackDiskCount?"čierny":"biely"):"Nastala remiza!"));
                    endOfGame=true;
                    jButtonBack.setEnabled(false);
                    jButtonSave.setEnabled(false);
               }

            }
        }
        else{
            JOptionPane.showMessageDialog(null, "Koniec hry! "+(whiteDiskCount!=blackDiskCount?"Vyhral "+(whiteDiskCount<blackDiskCount?"čierny":"biely"):"Nastala remiza!"));
            
        }
    }  
    /**
     * Akcia Koniec - po stlačení tlačidla Koniec.
     */
    private void jButtonEndActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButtonEndActionPerformed
        this.dispose();
    }//GEN-LAST:event_jButtonEndActionPerformed
    /**
     * Akcia Uložiť - po stlačení tlačidla Save 
     */
    private void jButtonSaveActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButtonSaveActionPerformed
        String saveName;
        do{
            saveName=JOptionPane.showInputDialog(this, "Zadajte meno: ");
        }while(saveName.matches(".*(,|#)+.*"));
        try {
            Files.write(Paths.get("lib/SavedGames.data"), (saveName+"#\n"+size+"\n"+typeOfOponent+"\n"+I+"\n"+B+"\n"+C+"\n"+(game.currentPlayer())+"\n"+lastX+"\n"+lastY+"\n"+game.getBoard().toString2()+",").getBytes(), StandardOpenOption.APPEND);
        }catch (IOException e) {
            JOptionPane.showMessageDialog(null, "Chýbajúci súbor SavedGames.");
        }
    }//GEN-LAST:event_jButtonSaveActionPerformed
    /**
     * Akcia Krok Späť - po stlačení tlačidla Krok Späť 
     */
    private void jButtonBackActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButtonBackActionPerformed
        game.getBoard().setField(fieldBack);
        game.currentPlayer().IncrasePool();
        if(freezing){
            listOfDIsabled.clear();
            for(int m=0;m<size;m++){
                for(int n=0;n<size;n++){
                    FieldButton[n][m].setEnabled(true);
                }
            }
            timer.restart();
        }
        if(typeOfOponent==0){
            game.nextPlayer();
            game.currentPlayer().IncrasePool();
        }
        else{
            game.currentPlayer().IncrasePool();
            game.currentPlayer().IncrasePool();
        }
        lastX=lastXBack;
        lastY=lastYBack;
        setFields();
        whiteDiskCount=0;
        blackDiskCount=0;
        for(int m=1;m<size+1;m++){
            for(int n=1;n<size+1;n++){
                if(game.getBoard().getField(m, n).getDisk()!=null){
                    if(game.getBoard().getField(m, n).getDisk().isWhite()){
                        whiteDiskCount++;
                    }
                    if(!game.getBoard().getField(m, n).getDisk().isWhite()){
                        blackDiskCount++;
                    }
                }
                else{
                }
            }
        }
        
        if(!game.testPlayerCanPutAnyDisc(game.currentPlayer()))
            game.nextPlayer();
        jLabelScorePlayer1.setText("Čierny:  "+blackDiskCount);
        jLabelScorePlayer2.setText("Biely:     "+whiteDiskCount);
        jLabel2.setText(game.currentPlayer().isWhite()?"Hráč:   Biely    ":"Hráč:   Čierny    ");
        jButtonBack.setEnabled(false);
        this.repaint();
    }//GEN-LAST:event_jButtonBackActionPerformed
    /**
     * Zmena veľkosti Fields.
     */
    private void formComponentResized(java.awt.event.ComponentEvent evt) {//GEN-FIRST:event_formComponentResized
        jPanelFields.setSize(this.getSize().width-230, this.getSize().height-30);
        resizeFields();
        setFields();
    }//GEN-LAST:event_formComponentResized

    /**
     * @param args the command line arguments
     */
    public static void main( String args[]) {
        /* Set the Nimbus look and feel */
        //<editor-fold defaultstate="collapsed" desc=" Look and feel setting code (optional) ">
        /* If Nimbus (introduced in Java SE 6) is not available, stay with the default look and feel.
         * For details see http://download.oracle.com/javase/tutorial/uiswing/lookandfeel/plaf.html 
         */
        try {
            for (javax.swing.UIManager.LookAndFeelInfo info : javax.swing.UIManager.getInstalledLookAndFeels()) {
                if ("Nimbus".equals(info.getName())) {
                    javax.swing.UIManager.setLookAndFeel(info.getClassName());
                    break;
                }
            }
        } catch (ClassNotFoundException | InstantiationException | IllegalAccessException | javax.swing.UnsupportedLookAndFeelException ex) {
            java.util.logging.Logger.getLogger(GameWindow.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        //</editor-fold>
        final int size=Integer.parseInt(args[0]);
        final int type=Integer.parseInt(args[1]);
        /* Create and display the form */
        java.awt.EventQueue.invokeLater(new Runnable() {
            @Override
            public void run() {
                new GameWindow(size,type).setVisible(true);
            }
        });
    }
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton jButtonBack;
    private javax.swing.JButton jButtonEnd;
    private javax.swing.JButton jButtonSave;
    private javax.swing.JLabel jLabel2;
    private javax.swing.JLabel jLabelScorePlayer1;
    private javax.swing.JLabel jLabelScorePlayer2;
    private javax.swing.JLabel jLabelTime;
    private javax.swing.JPanel jPanel1;
    private javax.swing.JPanel jPanel2;
    private javax.swing.JPanel jPanel3;
    private javax.swing.JPanel jPanelFields;
    private javax.swing.JSeparator jSeparator2;
    // End of variables declaration//GEN-END:variables

    
}
