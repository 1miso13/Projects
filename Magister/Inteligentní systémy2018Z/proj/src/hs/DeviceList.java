
package hs;

import java.util.ArrayList;
import java.util.Collections;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

/**
 * SIN - Inteligentná budova
 * @author Marcel Kiss (xkissm01)
 * @author Michal Ondrejo (xondre08)
 */
public class DeviceList {
    
    ArrayList<Device> devices = new ArrayList<>();
    
    public static DeviceList parse(JSONArray jsonDeviceList) {
        DeviceList deviceList = new DeviceList();
        for (Object rawDevice : jsonDeviceList) {
            JSONObject jsonDevice = (JSONObject)rawDevice;
            
            Device device = Device.parse(jsonDevice);
            deviceList.devices.add(device);            
        }
        deviceList.sort();
        return deviceList;
    }
    
    Device find(long idx) {
        for (int i = 0; i < devices.size(); i++)
            if (devices.get(i).idx == idx)
                return devices.get(i);
        return null;
    }
    
    void sort() {
        Collections.sort(devices, (Device o1, Device o2) -> {
            return (o1.planID < o2.planID ? -1 : 1);
        });
    }
    
    
}
