
package hs;

import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics;
import javax.swing.JPanel;

/**
 * SIN - Inteligentná budova
 * @author Marcel Kiss (xkissm01)
 * @author Michal Ondrejo (xondre08)
 */
public class Graph extends JPanel {
    
    int count = 0;
    double min = 0;
    double max = 0;
    double values[] = null;
    String suffix = "";
    
    int index = 0;
    long samples = 0;
    
    void setupValues(int count, double min, double max, String suffix) {
        this.count = count;
        values = new double[count];
        this.min = min;
        this.max = max;
        this.suffix = suffix;
    }
    
    void addValue(double val) {
        values[index] = val;
        index = (index + 1) % count;
        samples++;
        repaint();
    }
    
    double norm(double v)  {
        return (v - min) / (max - min);
    }
    
    static final Font font = new Font("Courier", 0, 10);

    @Override
    protected void paintComponent(Graphics g) {
        g.setColor(Color.white);
        g.fillRect(0, 0, getWidth(), getHeight());
        
        int offset = 5;
        double step = (double)(getWidth() - offset) / (double)(count - 1);
        g.setColor(Color.red);
        for (int i = 1; i < count; i++) {
            if (i <= (count - samples)) continue;
            int ii1 = (count + index + i - 1) % count;
            int ii2 = (count + index + i) % count;
            double v1 = norm(values[ii1]);
            double v2 = norm(values[ii2]);
            
            int y1 = getHeight() - (int)Math.round(v1 * getHeight());
            int y2 = getHeight() - (int)Math.round(v2 * getHeight());
            
            int x1 = (int)Math.round((i - 1) * step);
            int x2 = (int)Math.round((i) * step);
            g.drawLine(x1, y1, x2, y2);
        }
        
        g.setColor(Color.black);
        
        if (count > 0) {
            double v = norm(values[(count + index - 1) % count]);
            int y = getHeight() - (int)Math.round(v * getHeight());
            g.drawLine(0, y, getWidth(), y);
            g.fillArc(getWidth() - offset - 3, y - 3, 6, 6, 0, 360);
        }
        
        
        g.setFont(font);
        int h = g.getFontMetrics().getHeight();
        
        g.drawLine(0, 0 , 2, 0);
        g.drawLine(0, getHeight() / 2 , 2, getHeight() / 2);
        g.drawLine(0, getHeight() , 2, getHeight());
        
        g.drawString("" + min + suffix, 2, getHeight() - 1);
        g.drawString("" + max + suffix, 2, h - 2);
        g.drawString("" + (min + (max - min) / 2) + suffix, 2, getHeight() / 2 + h / 2 - 1);
    }
    
}
