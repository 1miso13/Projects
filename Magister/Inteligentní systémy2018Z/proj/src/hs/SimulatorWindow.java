
package hs;

import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.Timer;
import java.util.TimerTask;
import javax.swing.DefaultListModel;
import javax.swing.JList;
import javax.swing.JToggleButton;
import javax.swing.SwingUtilities;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

/**
 * SIN - Inteligentná budova
 * @author Marcel Kiss (xkissm01)
 * @author Michal Ondrejo (xondre08)
 */
public class SimulatorWindow extends javax.swing.JFrame {
    
    Client client = new Client();
    
    DefaultListModel<Device> deviceListModel = new DefaultListModel<>();
    
    double homeTemp = 24;
    double gardenTemp = 20;
    double gardenHumid = 50;
    double outdoorTemp = 10;
    double outdoorLux = 20;
    
    Timer simulationTimer;
    Timer refreshTimer;

    /**
     * Creates new form SimulatorWindow
     */
    public SimulatorWindow() {
        initComponents();
        
        deviceJList.setModel(deviceListModel);
        deviceJList.setCellRenderer(deviceCellRenderer);
        
        int values = 100;
        homeTempGraph.setupValues(values, 10, 35, "°C");
        gardenTempGraph.setupValues(values, -2, 30, "°C");
        gardenHumidGraph.setupValues(values, 0, 100, "%");
        outdoorHumidGraph.setupValues(values, 0, 100, "%");
        outdoorLuxGraph.setupValues(values, 0, 600, " Lux");
        
        deviceJList.addMouseListener(new MouseAdapter() {
            @Override
            public void mousePressed(MouseEvent evt) {
                JList list = (JList) evt.getSource();
                if (evt.getButton() != MouseEvent.BUTTON1) {
                    int index = list.locationToIndex(evt.getPoint());
                    if (index >= 0) {
                        Device device = deviceJList.getModel().getElementAt(index);
                        if (device.type == Device.Type.LightSwitch) {
                            postStatus((int) device.idx, !device.status);
                        }
                    }
                }
            }
        });
    }
    
    void start() {
        if (simulationTimer != null) return;
        simulationTimer = new Timer();
        simulationTimer.schedule(new TimerTask() {
            long counter = 0;
            @Override
            public void run() {
                double v1 = ((Math.sin((counter) / 200.0 * (Math.PI * 2)) + 1.0) / 2.0) * (homeTempGraph.max - homeTempGraph.min) + homeTempGraph.min;
                double v2 = ((Math.sin((counter+10) / 100.0 * (Math.PI * 2)) + 1.0) / 2.0) * (gardenTempGraph.max - gardenTempGraph.min) + gardenTempGraph.min;
                double v3 = (Math.sin((counter+20) / 70.0 * (Math.PI * 2)) + 1.0) / 2.0 * (gardenHumidGraph.max - gardenHumidGraph.min);
                double v4 = (Math.sin((counter-40) / 90.0 * (Math.PI * 2)) + 1.0) / 2.0 * (outdoorHumidGraph.max - outdoorHumidGraph.min);
                double v5 = (Math.sin((counter+35) / 400.0 * (Math.PI * 2)) + 1.0) / 2.0 * (outdoorLuxGraph.max - outdoorLuxGraph.min);
                
                homeTempGraph.addValue(v1);
                gardenTempGraph.addValue(v2);
                gardenHumidGraph.addValue(v3);
                outdoorHumidGraph.addValue(v4);
                outdoorLuxGraph.addValue(v5);
                
                homeTempLabel.setText(String.format("%.1f °C", v1));
                gardenTempLabel.setText(String.format("%.1f °C", v2));
                gardenHumidLabel.setText(String.format("%.1f %%", v3));
                outdoorHumidLabel.setText(String.format("%.1f %%", v4));
                outdoorLuxLabel.setText(String.format("%.1f Lux", v5));
                
                if (idx1 > 0) postTemperature(idx1, v1);
                if (idx2 > 0) postTemperature(idx2, v2);
                if (idx3 > 0) postHumidity(idx3, v3);
                if (idx4 > 0) postHumidity(idx4, v4);
                if (idx5 > 0) postLux(idx5, v5);
                
                counter++;
            }
        }, 0, 150);
    }
    
    void stop() {
        if (simulationTimer != null) {
            simulationTimer.cancel();
            simulationTimer.purge();
            simulationTimer = null;
        }
    }
    
    DeviceList list = null;
    
    void updateDeviceList() {
        String ip = ipField.getText();
        if (ip == null || ip.isEmpty()) return;
        JSONObject response = client.get("http://" + ip + ":8080/json.htm?type=devices");
        JSONArray result = (JSONArray)response.get("result");
        DeviceList deviceList = DeviceList.parse(result);
        
        int updateCount = 0;
        int addCount = 0;
        if (list != null) {
            for (Device otherDevice : deviceList.devices) {
                Device device = list.find(otherDevice.idx);
                if (device != null) {
                    device.update(otherDevice);
                    updateCount++;
                } else {
                    deviceListModel.addElement(otherDevice);
                    addCount = 0;
                }
            }
        } else {
            list = deviceList;
            addCount = deviceList.devices.size();
        }
        
        if (addCount > 0) {
            deviceListModel.clear();
            for (Device device : deviceList.devices) {
                deviceListModel.addElement(device);
            }
            deviceJList.repaint();
        }
        
        if (updateCount > 0) {
            deviceJList.repaint();
        }
    }
    
    void postTemperature(int idx, double temp) {
        String ip = ipField.getText();
        if (ip == null || ip.isEmpty()) return;
        String strVal = String.format("%.1f", temp);
        JSONObject response = client.get("http://" + ip + ":8080/json.htm?type=command&param=udevice&idx=" + idx + "&nvalue=0&svalue=" + strVal + "");
    }
    
    void postHumidity(int idx, double humid) {
        String ip = ipField.getText();
        if (ip == null || ip.isEmpty()) return;
        String strVal = String.format("%.1f", humid);
        JSONObject response = client.get("http://" + ip + ":8080/json.htm?type=command&param=udevice&idx=" + idx + "&nvalue=" + strVal + "&svalue=0");
    }
    
    void postLux(int idx, double lux) {
        String ip = ipField.getText();
        if (ip == null || ip.isEmpty()) return;
        String strVal = String.format("%.1f", lux);
        JSONObject response = client.get("http://" + ip + ":8080/json.htm?type=command&param=udevice&idx=" + idx + "&svalue=" + strVal);
    }
    
    void postStatus(int idx, boolean status) {
        String ip = ipField.getText();
        if (ip == null || ip.isEmpty()) return;
        String strVal = (status ? "On" : "Off");
        
        JSONObject response = client.get("http://" + ip + ":8080/json.htm?type=command&param=switchlight&idx=" + idx + "&switchcmd=" + strVal);
    }

    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        deviceCellRenderer = new hs.DeviceCellRenderer();
        jScrollPane1 = new javax.swing.JScrollPane();
        deviceJList = new javax.swing.JList<>();
        jPanel1 = new javax.swing.JPanel();
        homeTempGraph = new hs.Graph();
        homeTempLabel = new javax.swing.JLabel();
        jLabel1 = new javax.swing.JLabel();
        attach1 = new javax.swing.JButton();
        jPanel2 = new javax.swing.JPanel();
        gardenTempGraph = new hs.Graph();
        gardenTempLabel = new javax.swing.JLabel();
        jLabel4 = new javax.swing.JLabel();
        attach2 = new javax.swing.JButton();
        jPanel3 = new javax.swing.JPanel();
        outdoorLuxGraph = new hs.Graph();
        outdoorLuxLabel = new javax.swing.JLabel();
        jLabel5 = new javax.swing.JLabel();
        attach5 = new javax.swing.JButton();
        jPanel4 = new javax.swing.JPanel();
        gardenHumidGraph = new hs.Graph();
        gardenHumidLabel = new javax.swing.JLabel();
        jLabel6 = new javax.swing.JLabel();
        attach3 = new javax.swing.JButton();
        jPanel5 = new javax.swing.JPanel();
        outdoorHumidGraph = new hs.Graph();
        outdoorHumidLabel = new javax.swing.JLabel();
        jLabel7 = new javax.swing.JLabel();
        attach4 = new javax.swing.JButton();
        startButton = new javax.swing.JButton();
        stopButton = new javax.swing.JButton();
        ipField = new javax.swing.JTextField();
        jLabel2 = new javax.swing.JLabel();
        updateButton = new javax.swing.JToggleButton();

        deviceCellRenderer.setText("deviceCellRenderer1");

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        setTitle("HouseSimulator");

        jScrollPane1.setViewportView(deviceJList);

        javax.swing.GroupLayout homeTempGraphLayout = new javax.swing.GroupLayout(homeTempGraph);
        homeTempGraph.setLayout(homeTempGraphLayout);
        homeTempGraphLayout.setHorizontalGroup(
            homeTempGraphLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 0, Short.MAX_VALUE)
        );
        homeTempGraphLayout.setVerticalGroup(
            homeTempGraphLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 71, Short.MAX_VALUE)
        );

        homeTempLabel.setText("50°C");

        jLabel1.setText("Home Temperature:");

        attach1.setText("Attach (*)");
        attach1.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                attach1ActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout jPanel1Layout = new javax.swing.GroupLayout(jPanel1);
        jPanel1.setLayout(jPanel1Layout);
        jPanel1Layout.setHorizontalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel1Layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(homeTempGraph, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addGroup(jPanel1Layout.createSequentialGroup()
                        .addComponent(jLabel1, javax.swing.GroupLayout.PREFERRED_SIZE, 150, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(attach1)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addComponent(homeTempLabel)))
                .addContainerGap())
        );
        jPanel1Layout.setVerticalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel1Layout.createSequentialGroup()
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(homeTempLabel)
                    .addComponent(jLabel1)
                    .addComponent(attach1))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(homeTempGraph, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap())
        );

        javax.swing.GroupLayout gardenTempGraphLayout = new javax.swing.GroupLayout(gardenTempGraph);
        gardenTempGraph.setLayout(gardenTempGraphLayout);
        gardenTempGraphLayout.setHorizontalGroup(
            gardenTempGraphLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 0, Short.MAX_VALUE)
        );
        gardenTempGraphLayout.setVerticalGroup(
            gardenTempGraphLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 71, Short.MAX_VALUE)
        );

        gardenTempLabel.setText("0");

        jLabel4.setText("Garden Temperature:");

        attach2.setText("Attach (*)");
        attach2.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                attach2ActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout jPanel2Layout = new javax.swing.GroupLayout(jPanel2);
        jPanel2.setLayout(jPanel2Layout);
        jPanel2Layout.setHorizontalGroup(
            jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel2Layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(gardenTempGraph, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addGroup(jPanel2Layout.createSequentialGroup()
                        .addComponent(jLabel4, javax.swing.GroupLayout.PREFERRED_SIZE, 148, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(attach2)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addComponent(gardenTempLabel)))
                .addContainerGap())
        );
        jPanel2Layout.setVerticalGroup(
            jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel2Layout.createSequentialGroup()
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(gardenTempLabel)
                    .addComponent(jLabel4)
                    .addComponent(attach2))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(gardenTempGraph, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap())
        );

        javax.swing.GroupLayout outdoorLuxGraphLayout = new javax.swing.GroupLayout(outdoorLuxGraph);
        outdoorLuxGraph.setLayout(outdoorLuxGraphLayout);
        outdoorLuxGraphLayout.setHorizontalGroup(
            outdoorLuxGraphLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 0, Short.MAX_VALUE)
        );
        outdoorLuxGraphLayout.setVerticalGroup(
            outdoorLuxGraphLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 71, Short.MAX_VALUE)
        );

        outdoorLuxLabel.setText("0");

        jLabel5.setText("Outdoor Lux:");

        attach5.setText("Attach (*)");
        attach5.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                attach5ActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout jPanel3Layout = new javax.swing.GroupLayout(jPanel3);
        jPanel3.setLayout(jPanel3Layout);
        jPanel3Layout.setHorizontalGroup(
            jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel3Layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(outdoorLuxGraph, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addGroup(jPanel3Layout.createSequentialGroup()
                        .addComponent(jLabel5, javax.swing.GroupLayout.PREFERRED_SIZE, 157, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(attach5)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addComponent(outdoorLuxLabel)))
                .addContainerGap())
        );
        jPanel3Layout.setVerticalGroup(
            jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel3Layout.createSequentialGroup()
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addGroup(jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(outdoorLuxLabel)
                    .addComponent(jLabel5)
                    .addComponent(attach5))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(outdoorLuxGraph, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap())
        );

        javax.swing.GroupLayout gardenHumidGraphLayout = new javax.swing.GroupLayout(gardenHumidGraph);
        gardenHumidGraph.setLayout(gardenHumidGraphLayout);
        gardenHumidGraphLayout.setHorizontalGroup(
            gardenHumidGraphLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 0, Short.MAX_VALUE)
        );
        gardenHumidGraphLayout.setVerticalGroup(
            gardenHumidGraphLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 71, Short.MAX_VALUE)
        );

        gardenHumidLabel.setText("0");

        jLabel6.setText("Garden Humid:");

        attach3.setText("Attach (*)");
        attach3.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                attach3ActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout jPanel4Layout = new javax.swing.GroupLayout(jPanel4);
        jPanel4.setLayout(jPanel4Layout);
        jPanel4Layout.setHorizontalGroup(
            jPanel4Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel4Layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(jPanel4Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(gardenHumidGraph, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addGroup(jPanel4Layout.createSequentialGroup()
                        .addComponent(jLabel6, javax.swing.GroupLayout.PREFERRED_SIZE, 151, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(attach3)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addComponent(gardenHumidLabel)))
                .addContainerGap())
        );
        jPanel4Layout.setVerticalGroup(
            jPanel4Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel4Layout.createSequentialGroup()
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addGroup(jPanel4Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(gardenHumidLabel)
                    .addComponent(jLabel6)
                    .addComponent(attach3))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(gardenHumidGraph, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap())
        );

        javax.swing.GroupLayout outdoorHumidGraphLayout = new javax.swing.GroupLayout(outdoorHumidGraph);
        outdoorHumidGraph.setLayout(outdoorHumidGraphLayout);
        outdoorHumidGraphLayout.setHorizontalGroup(
            outdoorHumidGraphLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 0, Short.MAX_VALUE)
        );
        outdoorHumidGraphLayout.setVerticalGroup(
            outdoorHumidGraphLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 71, Short.MAX_VALUE)
        );

        outdoorHumidLabel.setText("0");

        jLabel7.setText("Outdoor Humid:");

        attach4.setText("Attach (*)");
        attach4.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                attach4ActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout jPanel5Layout = new javax.swing.GroupLayout(jPanel5);
        jPanel5.setLayout(jPanel5Layout);
        jPanel5Layout.setHorizontalGroup(
            jPanel5Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel5Layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(jPanel5Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(outdoorHumidGraph, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addGroup(jPanel5Layout.createSequentialGroup()
                        .addComponent(jLabel7, javax.swing.GroupLayout.PREFERRED_SIZE, 154, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(attach4)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addComponent(outdoorHumidLabel)))
                .addContainerGap())
        );
        jPanel5Layout.setVerticalGroup(
            jPanel5Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel5Layout.createSequentialGroup()
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addGroup(jPanel5Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(outdoorHumidLabel)
                    .addComponent(jLabel7)
                    .addComponent(attach4))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(outdoorHumidGraph, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap())
        );

        startButton.setText("Start");
        startButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                startButtonActionPerformed(evt);
            }
        });

        stopButton.setText("Stop");
        stopButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                stopButtonActionPerformed(evt);
            }
        });

        ipField.setText("localhost");

        jLabel2.setText("IP:");

        updateButton.setText("Start Updating");
        updateButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                updateButtonActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jPanel1, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(jPanel2, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(jPanel3, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(jPanel4, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(jPanel5, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addGroup(layout.createSequentialGroup()
                        .addComponent(startButton, javax.swing.GroupLayout.PREFERRED_SIZE, 63, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(stopButton, javax.swing.GroupLayout.PREFERRED_SIZE, 63, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(updateButton, javax.swing.GroupLayout.PREFERRED_SIZE, 128, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jLabel2)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(ipField, javax.swing.GroupLayout.DEFAULT_SIZE, 142, Short.MAX_VALUE)))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jScrollPane1, javax.swing.GroupLayout.PREFERRED_SIZE, 308, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap())
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jScrollPane1)
                    .addGroup(layout.createSequentialGroup()
                        .addComponent(jPanel1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jPanel2, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jPanel4, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jPanel5, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jPanel3, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(startButton)
                            .addComponent(stopButton)
                            .addComponent(ipField, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(jLabel2)
                            .addComponent(updateButton))
                        .addGap(0, 0, Short.MAX_VALUE)))
                .addContainerGap())
        );

        pack();
        setLocationRelativeTo(null);
    }// </editor-fold>//GEN-END:initComponents

    int idx1, idx2, idx3, idx4, idx5;
    
    private void attach1ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_attach1ActionPerformed
        Device selected = deviceJList.getSelectedValue();
        if (selected != null) {
            idx1 = (int) selected.idx;
            attach1.setText("Attach (" + idx1 + ")");
        }
    }//GEN-LAST:event_attach1ActionPerformed

    private void attach2ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_attach2ActionPerformed
        Device selected = deviceJList.getSelectedValue();
        if (selected != null) {
            idx2 = (int) selected.idx;
            attach2.setText("Attach (" + idx2 + ")");
        }
    }//GEN-LAST:event_attach2ActionPerformed

    private void attach3ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_attach3ActionPerformed
        Device selected = deviceJList.getSelectedValue();
        if (selected != null) {
            idx3 = (int) selected.idx;
            attach3.setText("Attach (" + idx3 + ")");
        }
    }//GEN-LAST:event_attach3ActionPerformed

    private void attach4ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_attach4ActionPerformed
        Device selected = deviceJList.getSelectedValue();
        if (selected != null) {
            idx4 = (int) selected.idx;
            attach4.setText("Attach (" + idx4 + ")");
        }
    }//GEN-LAST:event_attach4ActionPerformed

    private void attach5ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_attach5ActionPerformed
        Device selected = deviceJList.getSelectedValue();
        if (selected != null) {
            idx5 = (int) selected.idx;
            attach5.setText("Attach (" + idx5 + ")");
        }
    }//GEN-LAST:event_attach5ActionPerformed

    private void startButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_startButtonActionPerformed
        start();
    }//GEN-LAST:event_startButtonActionPerformed

    private void stopButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_stopButtonActionPerformed
        stop();
    }//GEN-LAST:event_stopButtonActionPerformed

    private void updateButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_updateButtonActionPerformed
        JToggleButton t = (JToggleButton)evt.getSource();
        if (t.isSelected()) {
            refreshTimer = new Timer();
            refreshTimer.schedule(new TimerTask() {
                @Override
                public void run() {
                    SwingUtilities.invokeLater(() -> {
                        updateDeviceList();
                    });
                }
            }, 0, 500);
            t.setText("Stop Updating");
        } else {
            refreshTimer.cancel();
            refreshTimer.purge();
            t.setText("Start Updating");
        }
        
    }//GEN-LAST:event_updateButtonActionPerformed

    /**
     * @param args the command line arguments
     */
    public static void main(String args[]) {
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
        } catch (ClassNotFoundException ex) {
            java.util.logging.Logger.getLogger(SimulatorWindow.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (InstantiationException ex) {
            java.util.logging.Logger.getLogger(SimulatorWindow.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (IllegalAccessException ex) {
            java.util.logging.Logger.getLogger(SimulatorWindow.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (javax.swing.UnsupportedLookAndFeelException ex) {
            java.util.logging.Logger.getLogger(SimulatorWindow.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        //</editor-fold>

        /* Create and display the form */
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                new SimulatorWindow().setVisible(true);
            }
        });
    }

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton attach1;
    private javax.swing.JButton attach2;
    private javax.swing.JButton attach3;
    private javax.swing.JButton attach4;
    private javax.swing.JButton attach5;
    private hs.DeviceCellRenderer deviceCellRenderer;
    private javax.swing.JList<Device> deviceJList;
    private hs.Graph gardenHumidGraph;
    private javax.swing.JLabel gardenHumidLabel;
    private hs.Graph gardenTempGraph;
    private javax.swing.JLabel gardenTempLabel;
    private hs.Graph homeTempGraph;
    private javax.swing.JLabel homeTempLabel;
    private javax.swing.JTextField ipField;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JLabel jLabel2;
    private javax.swing.JLabel jLabel4;
    private javax.swing.JLabel jLabel5;
    private javax.swing.JLabel jLabel6;
    private javax.swing.JLabel jLabel7;
    private javax.swing.JPanel jPanel1;
    private javax.swing.JPanel jPanel2;
    private javax.swing.JPanel jPanel3;
    private javax.swing.JPanel jPanel4;
    private javax.swing.JPanel jPanel5;
    private javax.swing.JScrollPane jScrollPane1;
    private hs.Graph outdoorHumidGraph;
    private javax.swing.JLabel outdoorHumidLabel;
    private hs.Graph outdoorLuxGraph;
    private javax.swing.JLabel outdoorLuxLabel;
    private javax.swing.JButton startButton;
    private javax.swing.JButton stopButton;
    private javax.swing.JToggleButton updateButton;
    // End of variables declaration//GEN-END:variables
}
