
package hs;

import java.awt.Color;
import java.awt.Component;
import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.ListCellRenderer;

/**
 * SIN - Inteligentná budova
 * @author Marcel Kiss (xkissm01)
 * @author Michal Ondrejo (xondre08)
 */
public class DeviceCellRenderer extends JLabel implements ListCellRenderer<Device> {
    
    ImageIcon switchOnIcon = null;
    ImageIcon switchOffIcon = null;
    ImageIcon humidIcon = null;
    ImageIcon tempIcon = null;
    ImageIcon luxIcon = null;
    ImageIcon generalIcon = null;
    
    public DeviceCellRenderer() {
        switchOnIcon = new ImageIcon(getClass().getResource("/res/" + "switch_on" + ".jpg"));
        switchOffIcon = new ImageIcon(getClass().getResource("/res/" + "switch_off" + ".jpg"));
        humidIcon = new ImageIcon(getClass().getResource("/res/" + "humid" + ".jpg"));
        tempIcon = new ImageIcon(getClass().getResource("/res/" + "temp" + ".jpg"));
        luxIcon = new ImageIcon(getClass().getResource("/res/" + "lux" + ".jpg"));
        generalIcon = new ImageIcon(getClass().getResource("/res/" + "dev" + ".jpg"));
    }
    
    ImageIcon getIcon(Device.Type type, boolean status) {
        switch (type) {
            case LightSwitch: return status ? switchOnIcon : switchOffIcon;
            case Humidity: return humidIcon;
            case Temp: return tempIcon;
            case Lux: return luxIcon;
            case General: return generalIcon;
        }
        return null;
    }
    
    @Override
    public Component getListCellRendererComponent(JList<? extends Device> list, Device device, int index, boolean isSelected, boolean cellHasFocus) {
        ImageIcon imageIcon = getIcon(device.type, device.status);
        setIcon(imageIcon);
        setText(device.toString());
        
        setOpaque(true);
        setBackground(isSelected ? new Color(128, 128, 255) : Color.white);
        
        return this;
    }
}
