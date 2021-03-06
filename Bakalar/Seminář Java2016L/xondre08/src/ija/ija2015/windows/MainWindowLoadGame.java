/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package ija.ija2015.windows;

import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import javax.swing.DefaultListModel;

/**
 *
 * @author xkeryj00
 * @author xondre08
 */
public class MainWindowLoadGame extends javax.swing.JPanel {
    private final String FileContent;

    DefaultListModel model = new DefaultListModel();
    DefaultListModel model1 = new DefaultListModel();
    /**
     * Creates new form MainWindowLoadGame
     * @param FileContent Obsah súboru.
     */
    public MainWindowLoadGame(String FileContent) {
        initComponents();
        this.FileContent = FileContent;

         List<String> allMatches = new ArrayList<>();
        Matcher m = Pattern.compile(",[^#]+#")
            .matcher(FileContent);
        List<String> allMatches1 = new ArrayList<>();
        Matcher m1 = Pattern.compile("#[^,]+(,|$)")
            .matcher(FileContent);
        while (m.find()) {
          allMatches.add(m.group());
        }
        while (m1.find()) {
          allMatches1.add(m1.group());
        }
        for (int i = 0; i < allMatches.size(); i++) {
            String l=allMatches.get(i);
            model.addElement(l.substring(1, l.length()-1));
            String l1=allMatches1.get(i);
            model1.addElement(l1.substring(1, l1.length()-1));

        }
        jList1.setModel(model);
        jList1.setSelectedIndex(0);
        
    }

    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        jLabelSelectGame = new javax.swing.JLabel();
        jScrollPaneSelectGame = new javax.swing.JScrollPane();
        jList1 = new javax.swing.JList();
        jButtonStartGame = new javax.swing.JButton();

        setPreferredSize(new java.awt.Dimension(350, 200));

        jLabelSelectGame.setText("Vyberte hru:");

        jScrollPaneSelectGame.setViewportView(jList1);

        jButtonStartGame.setText("Začať hru");
        jButtonStartGame.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButtonStartGameActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(this);
        this.setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGap(47, 47, 47)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addComponent(jLabelSelectGame)
                        .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                    .addGroup(layout.createSequentialGroup()
                        .addComponent(jScrollPaneSelectGame, javax.swing.GroupLayout.PREFERRED_SIZE, 111, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 70, Short.MAX_VALUE)
                        .addComponent(jButtonStartGame, javax.swing.GroupLayout.PREFERRED_SIZE, 91, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(31, 31, 31))))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGap(16, 16, 16)
                .addComponent(jLabelSelectGame)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addGap(0, 58, Short.MAX_VALUE)
                        .addComponent(jButtonStartGame, javax.swing.GroupLayout.PREFERRED_SIZE, 68, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addComponent(jScrollPaneSelectGame, javax.swing.GroupLayout.PREFERRED_SIZE, 0, Short.MAX_VALUE))
                .addGap(38, 38, 38))
        );
    }// </editor-fold>//GEN-END:initComponents
    /**
     * Akcia načítania hry.
     */
    private void jButtonStartGameActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButtonStartGameActionPerformed
        int index=jList1.getSelectedIndex();

        String s=model1.get(index).toString();
        GameWindow g=new GameWindow(s);
        g.setVisible(true);
    }//GEN-LAST:event_jButtonStartGameActionPerformed

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton jButtonStartGame;
    private javax.swing.JLabel jLabelSelectGame;
    private javax.swing.JList jList1;
    private javax.swing.JScrollPane jScrollPaneSelectGame;
    // End of variables declaration//GEN-END:variables

    /*private GameWindow GameWindow(String toString) {
     * throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
     * }*/
}
