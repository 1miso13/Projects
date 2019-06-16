/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package ija.ija2015.windows;

import javax.swing.JOptionPane;

/**
 *
 * @author xondre08
 * @author xkeryj00
 */
public class MainWindowNewGame extends javax.swing.JPanel {

    /**
     * Creates new form MainWindowNewGame
     */
    public MainWindowNewGame() {
        initComponents();
        checkboxFreezing.setVisible(true);
    }

    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        buttonGroup1 = new javax.swing.ButtonGroup();
        jButtonStartGame = new javax.swing.JButton();
        checkboxFreezing = new java.awt.Checkbox();
        jScrollPaneSize = new javax.swing.JScrollPane();
        jList1 = new javax.swing.JList();
        jRadioButtonPlayer = new javax.swing.JRadioButton();
        jRadioButtonComputer = new javax.swing.JRadioButton();
        jCheckBoxRandom = new javax.swing.JCheckBox();
        jSpinnerI = new javax.swing.JSpinner();
        jSpinnerB = new javax.swing.JSpinner();
        jSpinnerC = new javax.swing.JSpinner();
        jLabelI = new javax.swing.JLabel();
        jLabelB = new javax.swing.JLabel();
        jLabelC = new javax.swing.JLabel();

        setPreferredSize(new java.awt.Dimension(350, 200));
        addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyReleased(java.awt.event.KeyEvent evt) {
                formKeyReleased(evt);
            }
        });

        jButtonStartGame.setText("Začať hru");
        jButtonStartGame.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButtonStartGameActionPerformed(evt);
            }
        });

        checkboxFreezing.setLabel("Zamŕzanie kameňov");
        checkboxFreezing.setVisible(false);
        checkboxFreezing.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                checkboxFreezingMouseClicked(evt);
            }
            public void mouseReleased(java.awt.event.MouseEvent evt) {
                checkboxFreezingMouseReleased(evt);
            }
        });

        jList1.setModel(new javax.swing.AbstractListModel() {
            String[] strings = { "6 x 6", "8 x 8", "10 x 10", "12 x 12" };
            public int getSize() { return strings.length; }
            public Object getElementAt(int i) { return strings[i]; }
        });
        jList1.setSelectedIndex(1);
        jList1.setVisibleRowCount(4);
        jList1.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jList1MouseClicked(evt);
            }
        });
        jScrollPaneSize.setViewportView(jList1);

        buttonGroup1.add(jRadioButtonPlayer);
        jRadioButtonPlayer.setSelected(true);
        jRadioButtonPlayer.setText("Proti hráčovi");
        jRadioButtonPlayer.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jRadioButtonPlayerActionPerformed(evt);
            }
        });

        buttonGroup1.add(jRadioButtonComputer);
        jRadioButtonComputer.setText("Proti počítaču");
        jRadioButtonComputer.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jRadioButtonComputerActionPerformed(evt);
            }
        });

        jCheckBoxRandom.setText("Random mode");
        jCheckBoxRandom.setEnabled(jRadioButtonComputer.isSelected());

        jSpinnerI.setCursor(new java.awt.Cursor(java.awt.Cursor.DEFAULT_CURSOR));
        jSpinnerI.setEnabled(checkboxFreezing.getState());
        jSpinnerI.setFocusable(false);
        jSpinnerI.setName(""); // NOI18N
        jSpinnerI.setRequestFocusEnabled(false);
        jSpinnerI.setValue(1);

        jSpinnerB.setEnabled(checkboxFreezing.getState());
        jSpinnerB.setValue(1);

        jSpinnerC.setEnabled(checkboxFreezing.getState());
        jSpinnerC.setValue(1);

        jLabelI.setText("I:");
        jLabelI.setEnabled(checkboxFreezing.getState());

        jLabelB.setText("B:");
        jLabelB.setEnabled(checkboxFreezing.getState());

        jLabelC.setText("C:");
        jLabelC.setEnabled(checkboxFreezing.getState());

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(this);
        this.setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                .addGap(31, 31, 31)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                    .addGroup(layout.createSequentialGroup()
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING, false)
                            .addComponent(jScrollPaneSize, javax.swing.GroupLayout.PREFERRED_SIZE, 126, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addGroup(javax.swing.GroupLayout.Alignment.LEADING, layout.createSequentialGroup()
                                .addGap(25, 25, 25)
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                                    .addComponent(jLabelI, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                                    .addComponent(jLabelB, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                                    .addComponent(jLabelC, javax.swing.GroupLayout.PREFERRED_SIZE, 20, javax.swing.GroupLayout.PREFERRED_SIZE))
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                    .addGroup(layout.createSequentialGroup()
                                        .addGap(18, 18, 18)
                                        .addComponent(jSpinnerC))
                                    .addGroup(layout.createSequentialGroup()
                                        .addGap(18, 18, 18)
                                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                            .addComponent(jSpinnerB)
                                            .addComponent(jSpinnerI))))))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 62, Short.MAX_VALUE)
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                    .addComponent(jRadioButtonPlayer)
                                    .addComponent(jRadioButtonComputer))
                                .addGap(15, 15, 15))
                            .addComponent(jButtonStartGame, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.PREFERRED_SIZE, 88, javax.swing.GroupLayout.PREFERRED_SIZE)))
                    .addGroup(layout.createSequentialGroup()
                        .addComponent(checkboxFreezing, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addComponent(jCheckBoxRandom)))
                .addGap(25, 25, 25))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addGap(24, 24, 24)
                        .addComponent(jRadioButtonPlayer)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jRadioButtonComputer)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jCheckBoxRandom))
                    .addGroup(layout.createSequentialGroup()
                        .addContainerGap()
                        .addComponent(jScrollPaneSize, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(checkboxFreezing, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jButtonStartGame, javax.swing.GroupLayout.PREFERRED_SIZE, 58, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addGroup(layout.createSequentialGroup()
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(jSpinnerI, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(jLabelI))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(jSpinnerB, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(jLabelB))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(jSpinnerC, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(jLabelC))))
                .addGap(0, 35, Short.MAX_VALUE))
        );
    }// </editor-fold>//GEN-END:initComponents

    private void jList1MouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jList1MouseClicked
        if (evt.getClickCount() == 2 && !evt.isConsumed()) {
            evt.consume();
            jButtonStartGameActionPerformed(null);
        }
    }//GEN-LAST:event_jList1MouseClicked
    /**
     * Vykonaná akcia Start Game. 
     */
    private void jButtonStartGameActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButtonStartGameActionPerformed
        //zacneme hru
        //spustime nove okno s danými parametrami
        if(checkboxFreezing.getState()&&((Integer)jSpinnerB.getValue()<1||(Integer)jSpinnerC.getValue()<1||(Integer)jSpinnerI.getValue()<1)){
            JOptionPane.showMessageDialog(null, "Pri zamŕzani musia byt hodnoty B, I a C väčšie ako 0");
            return;
        }
        int size;
        switch(jList1.getSelectedIndex()){
            //velkost hracej plochy
            case 0:
                size = 6;
                break;
            case 1:
                size = 8;
                break;
            case 2:
                size = 10;
                break;
            case 3:
                size = 12;
                break;
            default:
                size=0;
                break;
        }
        int type;
        if(jRadioButtonPlayer.isSelected()){
            //proti hračovy
           type=0; 
        }
        else
        if(!jCheckBoxRandom.isSelected()){
            //proti počítaču
            type=1;
        }
        else
            //proti počítaču random
            type=2;
        
        GameWindow s=new GameWindow(size, 
                type,
                checkboxFreezing.getState()?(Integer)jSpinnerI.getValue():0,
                checkboxFreezing.getState()?(Integer)jSpinnerB.getValue():0,
                checkboxFreezing.getState()?(Integer)jSpinnerC.getValue():0);
        s.setVisible(true);
    }//GEN-LAST:event_jButtonStartGameActionPerformed

    private void jRadioButtonComputerActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jRadioButtonComputerActionPerformed
        jCheckBoxRandom.setEnabled(true);
    }//GEN-LAST:event_jRadioButtonComputerActionPerformed

    private void jRadioButtonPlayerActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jRadioButtonPlayerActionPerformed
        jCheckBoxRandom.setEnabled(false);
    }//GEN-LAST:event_jRadioButtonPlayerActionPerformed

    private void formKeyReleased(java.awt.event.KeyEvent evt) {//GEN-FIRST:event_formKeyReleased
        if(evt.getKeyChar()== '\n'){
            jButtonStartGameActionPerformed(null);
        }
    }//GEN-LAST:event_formKeyReleased

    private void checkboxFreezingMouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_checkboxFreezingMouseClicked
         
    }//GEN-LAST:event_checkboxFreezingMouseClicked

    private void checkboxFreezingMouseReleased(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_checkboxFreezingMouseReleased
        boolean type=!checkboxFreezing.getState();

        jSpinnerB.setEnabled(type);
        jSpinnerC.setEnabled(type);
        jSpinnerI.setEnabled(type);
        jLabelB.setEnabled(type);
        jLabelC.setEnabled(type);
        jLabelI.setEnabled(type);
    }//GEN-LAST:event_checkboxFreezingMouseReleased

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.ButtonGroup buttonGroup1;
    private java.awt.Checkbox checkboxFreezing;
    private javax.swing.JButton jButtonStartGame;
    private javax.swing.JCheckBox jCheckBoxRandom;
    private javax.swing.JLabel jLabelB;
    private javax.swing.JLabel jLabelC;
    private javax.swing.JLabel jLabelI;
    private javax.swing.JList jList1;
    private javax.swing.JRadioButton jRadioButtonComputer;
    private javax.swing.JRadioButton jRadioButtonPlayer;
    private javax.swing.JScrollPane jScrollPaneSize;
    private javax.swing.JSpinner jSpinnerB;
    private javax.swing.JSpinner jSpinnerC;
    private javax.swing.JSpinner jSpinnerI;
    // End of variables declaration//GEN-END:variables
}
