
package hs;

import java.util.Objects;
import org.json.simple.JSONObject;

/**
 * SIN - Inteligentná budova
 * @author Marcel Kiss (xkissm01)
 * @author Michal Ondrejo (xondre08)
 */
public class Device {
    
    
    enum Type {
        Unknown, LightSwitch, Temp, Humidity, Lux, General;
        
        public static Type parse(String type) {
            switch (type) {
                case "Light/Switch": return LightSwitch;
                case "Temp": return Temp;
                case "Humidity": return Humidity;
                case "Lux": return Lux;
                case "General": return General;
            }
            return Unknown;
        }
    }
    
    Type type;
    
    long idx;
    String name;
    long planID;
    
    boolean status = false;
    int updateCount = 0;
    
    double temp = 0.0;
    double humid = 0.0;
    double lux = 0;
    
    boolean blinds = false;
    
    public Device() {
        
    }
    
    public static Device parse(JSONObject jsonDevice) {
        Device device = new Device();
        
        device.type = Type.parse((String) jsonDevice.get("Type"));
        device.idx = Long.parseLong((String) jsonDevice.get("idx"));
        device.name = (String) jsonDevice.get("Name");
        device.planID = Long.parseLong((String) jsonDevice.get("PlanID"));
        
        String data = (String)jsonDevice.get("Data");
        switch (device.type) {
            case LightSwitch: {
                String type = (String)jsonDevice.get("SwitchType");
                device.blinds = (type.compareTo("Blinds") == 0);
                if (device.blinds) {
                    device.status = (data.compareTo("Open") == 0);
                } else {
                    device.status = (data.compareTo("On") == 0);
                }
            } break;
            case Temp: { device.temp = (Double)jsonDevice.get("Temp"); } break;
            case Humidity: { device.humid = (Long)jsonDevice.get("Humidity"); } break;
            case Lux: {
                device.lux = Long.parseLong(data.substring(0, data.indexOf(" Lux")));
            } break;
        }
        
        return device;
    }

    @Override
    public String toString() {
        String str = "[" + planID + ":" + idx +  "] " + name + " ";
        switch (type) {
            case LightSwitch: {
                str += "(" + (status ? "ON" : "OFF") + ")";
            } break;
            case Temp: {
                str += "(" + String.format("%.1f", temp) + " °C)";
            } break;
            case Humidity: {
                str += "(" + String.format("%.1f", humid) + " %)";
            } break;
            case Lux: {
                str += "(" + lux + " Lux)";
            } break;
        }
        return str;
    }

    @Override
    public int hashCode() {
        int hash = 7;
        hash = 29 * hash + (int) (this.idx ^ (this.idx >>> 32));
        return hash;
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        }
        if (obj == null) {
            return false;
        }
        if (getClass() != obj.getClass()) {
            return false;
        }
        final Device other = (Device) obj;
        if (this.idx != other.idx) {
            return false;
        }
        if (!Objects.equals(this.name, other.name)) {
            return false;
        }
        return true;
    }
    
    void update(Device other) {
        status = other.status;
        temp = other.temp;
        humid = other.humid;
        lux = other.lux;
        planID = other.planID;
        updateCount++;
    }
}
