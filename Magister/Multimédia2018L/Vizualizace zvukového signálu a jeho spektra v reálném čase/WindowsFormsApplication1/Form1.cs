using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;


namespace WindowsFormsApplication1
{
    public enum ApplicationState
    {
        UNLOADED,LOADED,PLAY,PAUSE,MIC,
        NULL,
        STOP
    }
    public partial class Form1 : Form
    {
        public ApplicationState GetState()
        {
            return ActualState;
        }

        // Create points that define curve.
        Point[] frequencyPoints;
        Point[] wavePoints;
        
        public Form1()
        {

            DoubleBuffered = true;
            InitializeComponent();
            Redraw();
        }
        
        
        

        private void tableLayoutPanel1_Paint(object sender, PaintEventArgs e)
        {
            //e.Graphics.Clear(Color.LightCyan);
            // Create pens.
            Pen redPen = new Pen(Color.Red, 1);
            Pen greenPen = new Pen(Color.Green, 3);
            Pen blackPen = new Pen(Color.Black, 1);

            Point lineBeg = new Point(0, (int)(panel1.Height/2.0));
            Point lineEnd = new Point(panel1.Width, (int)(panel1.Height / 2.0));
            Point[] linePoints = { lineBeg, lineEnd };
            e.Graphics.DrawLines(blackPen, linePoints);


            

            //wave
            if (wavePoints != null)
            {
                e.Graphics.DrawLines(blackPen, wavePoints);
            }
            //freq
            if (freqPoints != null)
            {
                e.Graphics.DrawLines(blackPen, freqPoints);
            }
        }
        static DFTClass DFTClass;
        private Point[] recalculatepoints(double [] Array,int maxheight,int drawHeight)/*panel1.Height / 2*/
        {
            float span= panel1.Width / (float)Array.Length;
            List<Point> PointList = new List<Point>();
            float heightSpan = panel1.Height / (float)maxheight;

            int oldX = 0;
            for (int i = 0; i < Array.Length; i+=(int)(1/**sound_class.multiplier*/))
            {
                if (waveShift > 0)
                {
                    Point p = new Point((int)(span / 2) + (int)(span * i), (int)(drawHeight - Array[i] * heightSpan));
                    int Ymax = p.Y,
                        Ymin = p.Y;
                    for (; i < Array.Length; i++)
                    {
                        p = new Point((int)(span / 2) + (int)(span * i), (int)(drawHeight - Array[i] * heightSpan));
                        if (p.X != oldX)
                        {
                            oldX = p.X;
                            break;
                        }
                        if (p.Y > Ymax)
                        {
                            Ymax = p.Y;
                        }
                        if (p.Y < Ymin)
                        {
                            Ymin= p.Y;
                        }
                        i++;
                    }
                    PointList.Add(new Point(oldX, Ymax));
                    if (Ymax != Ymin)
                    {
                        PointList.Add(new Point(oldX, Ymin));
                    }
                }
                else
                {
                    PointList.Add(new Point((int)(span / 2) + (int)(span * i), (int)(drawHeight - Array[i] * heightSpan)));
                }
                
            }
            return PointList.ToArray();


        }
        double[] waveUnmodified;
        SoundClass sound_class;
        private void drawPanel_Resize(object sender, EventArgs e)
        {
            
            Redraw();
        }
        private void Redraw()
        {
            if (sound_class!=null && sound_class.sound_data != null)
            {
                if (ActualState == ApplicationState.STOP || ActualState == ApplicationState.LOADED)
                {
                    if (WindowsFormsApplication1.LoadSoundClass.isModified && sound_class.sound_data != null)
                    {

                        WindowsFormsApplication1.LoadSoundClass.isModified = false;
                        List<double> arrayL = new List<double>();
                        for (int i = 0; i < sound_class.sound_data.Length; i += sound_class.bits_per_sample*(sound_class.channels == 2?2:1))
                        {
                            int d;
                            switch (sound_class.bits_per_sample)
                            {
                                case 8:
                                    d = sound_class.sound_data[i];
                                    break;
                                case 16:
                                    d = BitConverter.ToInt16(sound_class.sound_data, i);
                                    break;
                                case 24:
                                    byte[] arrayb = { 0, sound_class.sound_data[i + 0], sound_class.sound_data[i + 1], sound_class.sound_data[i + 2] };
                            
                                    d = BitConverter.ToInt32(arrayb, 0);
                                    break;
                                case 32:
                                    d = BitConverter.ToInt32(sound_class.sound_data, i);
                                    break;
                                default:
                                    throw new NotSupportedException("unsupported bits per sample");
                            
                            }
                            arrayL.Add(d);

                        }
                        waveUnmodified = arrayL.ToArray();
                        if (DFTClass == null)
                        {
                            DFTClass = new DFTClass(waveUnmodified.Length);
                        }

                        freq = DFTClass.FFT(waveUnmodified);
                    }
                }
                else
                {

                    if (ActualState == ApplicationState.MIC)
                    {
                        if (sound_class.actual_data != null)
                        {
                            waveUnmodified = sound_class.actual_data.ToArray();
                            freq = DFTClass.FFT(waveUnmodified);
                        }
                    }
                    else
                    {
                        if (sound_class.actual_data!=null)
                        {
                            waveUnmodified = sound_class.actual_data.ToArray();
                            freq = DFTClass.FFT(waveUnmodified);
                        }
                    }
                }
                wavePoints = recalculatepoints(waveUnmodified, (int)Math.Pow(2.0,(double)sound_class.bits_per_sample+1)/*(int)maxNumInArray(waveUnmodified) * 4*/, panel1.Height / 4);
                if (freq != null)
                {
                    freqPoints = recalculatepoints(freq, (int)maxNumInArray(freq)*6, (int)(panel1.Height * (5.0/6.0)));
                }

                label1.Text = sound_class.actualDataLength + "\n" + (sound_class.sample_rate ).ToString()+"Hz";
                SetStyle(ControlStyles.OptimizedDoubleBuffer, true);
                panel1.Refresh();
            }
        }
        ApplicationState ActualState = ApplicationState.UNLOADED;
        private Point[] freqPoints;
        private double[] freq;
        RecordClass record_class;
        private void changeApplicationState(ApplicationState newState)
        {
            switch (newState)
            {
                case ApplicationState.UNLOADED:
                    buttonLoadSound.Enabled = true;
                    buttonPause.Enabled = false;
                    buttonPlay.Enabled = false;
                    buttonRecord.Enabled = true;
                    buttonStop.Enabled = false;
                    break;
                case ApplicationState.LOADED:
                    try
                    {
                        LoadSoundClass.LoadWav(out sound_class);
                    }
                    catch(Exception e)
                    {
                        MessageBox.Show(e.Message);
                        return;
                    }
                    buttonLoadSound.Enabled = true;
                    buttonPause.Enabled = false;
                    buttonPlay.Enabled = true;
                    buttonRecord.Enabled = true;
                    buttonStop.Enabled = false;
                    Trace.Write(sound_class.sound_data.Length);
                    ActualState = newState;
                    Redraw();
                    break;
                case ApplicationState.PLAY:
                    PlaySoundClass.Play(sound_class,false);
                    buttonLoadSound.Enabled = true;
                    buttonPause.Enabled = true;
                    buttonPlay.Enabled = false;
                    buttonRecord.Enabled = true;
                    buttonStop.Enabled = true;

                    break;
                case ApplicationState.PAUSE:
                    PlaySoundClass.PauseMusic();
                    buttonLoadSound.Enabled = true;
                    buttonPause.Enabled = true;
                    buttonPlay.Enabled = false;
                    buttonRecord.Enabled = true;
                    buttonStop.Enabled = true;
                    break;
                case ApplicationState.MIC:
                    record_class = new RecordClass();
                    record_class.RecordButton();
                    //start microphone
                    if (ActualState == ApplicationState.MIC)
                    {
                        buttonLoadSound.Enabled = false;
                        buttonPause.Enabled = false;
                        buttonPlay.Enabled = false;
                        buttonRecord.Enabled = true;
                        buttonStop.Enabled = false;
                    }
                    //end microphone
                    else
                    {
                        record_class = null;
                        changeApplicationState(ApplicationState.UNLOADED);
                            return;
                        buttonLoadSound.Enabled = true;
                        buttonPause.Enabled = false;
                        buttonPlay.Enabled = false;
                        buttonRecord.Enabled = true;
                        buttonStop.Enabled = false;
                    }
                    break;
                case ApplicationState.STOP:
                    PlaySoundClass.StopPlaying();
                    buttonLoadSound.Enabled = true;
                    buttonPause.Enabled = false;
                    buttonPlay.Enabled = true;
                    buttonRecord.Enabled = true;
                    buttonStop.Enabled = false;
                    break;
                default:
                    break;
            }
            ActualState = newState;
        }

        private void button_Click(object sender, EventArgs e)
        {
            string b = (sender as Button).ToString();
            switch (b)
            {
                case "System.Windows.Forms.Button, Text: Load":
                   
                    if (!LoadSound())
                    {
                        return;
                    }
                    //load Successfully
                    changeApplicationState(ApplicationState.LOADED);
                    buttonPlay.Focus();
                    break;
                case "System.Windows.Forms.Button, Text: Record":
                    changeApplicationState(ApplicationState.MIC);
                    buttonRecord.Focus();
                    break;
                case "System.Windows.Forms.Button, Text: Stop":
                    changeApplicationState(ApplicationState.STOP);
                    buttonPlay.Focus();
                    break;
                case "System.Windows.Forms.Button, Text: Play":
                    changeApplicationState(ApplicationState.PLAY);
                    buttonPause.Focus();
                    break;
                case "System.Windows.Forms.Button, Text: Pause":
                    changeApplicationState(ApplicationState.PAUSE);
                    buttonPlay.Focus();
                    break;
                default:
                    break;
            }
        }
        
        private bool LoadSound()
        {
            OpenFileDialog openFileDialog1 = new OpenFileDialog();

            openFileDialog1.InitialDirectory = "c:\\";
            openFileDialog1.Filter = "Sound files (*.wav)|*.wav|All files (*.*)|*.*";
            openFileDialog1.FilterIndex = 1;
            openFileDialog1.RestoreDirectory = true;
            
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                WindowsFormsApplication1.LoadSoundClass.filename= openFileDialog1.FileName;
                return true;
            } 
            return false;
        }

        private void backgroundWorker1_DoWork(object sender, DoWorkEventArgs e)
        {

        }

        private void Form1_Resize(object sender, EventArgs e)
        {

        }
        private double maxNumInArray(double[] array)
        {
            double max = array[0];
            for (int i = 1; i < array.Length; i++)
            {
                if (max<array[i])
                {
                    max = array[i];
                }
            }
            return max;
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            if (PlaySoundClass.ticks!=0)
            {
                timer1.Interval = PlaySoundClass.ticks;
                PlaySoundClass.Update();
                Redraw();
            }
            else
            {
                if (ActualState== ApplicationState.PLAY && PlaySoundClass.ALState==OpenTK.Audio.OpenAL.ALSourceState.Stopped)
                {
                    changeApplicationState(ApplicationState.STOP);
                }
                timer1.Interval = 100;
            }
            //progress bar

            //buffer size 
            try
            {
                progressBar1.Value = (int)(((float)progressBar1.Maximum / ((float)sound_class.sound_data.Length/ PlaySoundClass.buffersize)) * PlaySoundClass.part);
            }
            catch
            {

            }
        }

        private void splitContainer1_Panel1_Paint(object sender, PaintEventArgs e)
        {

        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            float num;
            if (float.TryParse(textBox1.Text, out num)&& num>0.5 && num <1000)
            {
                sound_class.multiplier = num;
                sound_class.len();
            }
        }
        float waveShift=1;
        private void textBox2_TextChanged(object sender, EventArgs e)
        {
            float num;
            if (float.TryParse(textBox2.Text, out num) && num >= 1)
            {
                waveShift = num;
            }
        }
    }
}
