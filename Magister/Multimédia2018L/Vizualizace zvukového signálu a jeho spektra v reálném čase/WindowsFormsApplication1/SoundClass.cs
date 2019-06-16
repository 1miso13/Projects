using OpenTK.Audio.OpenAL;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WindowsFormsApplication1
{
    public class SoundClass
    {
        public  int channels, bits_per_sample, sample_rate; 
        public ALFormat sound_format;
        public  byte[] sound_data;
        public RecordClass recordClass;
        public LinkedList<double> actual_data;
        public int actualDataLength;
        public float multiplier=1;
        public void AddDataToQueue(double s)
        {
            //Debug.WriteLine("p");
            if (actual_data == null)
            {
                actual_data = new LinkedList<double>();
                for (int i = 0; i < actualDataLength; i++)
                {

                    actual_data.AddLast(0);
                }
            }
            while (actual_data.Count >=actualDataLength)
            {
                actual_data.RemoveFirst();
            }
            actual_data.AddLast(s);
        }
        public SoundClass(int channels, int bits_per_sample, int sample_rate)
        {
            this.channels = channels;
            this.bits_per_sample = bits_per_sample;
            this.sample_rate = sample_rate;
            this.sound_format = GetSoundFormat(channels, bits_per_sample);
            len();
        }

        public SoundClass(RecordClass recordClass, int channels, int bits_per_sample, int sample_rate)
        {
            this.recordClass = recordClass;
            this.channels = channels;
            this.bits_per_sample = bits_per_sample;
            this.sample_rate = sample_rate;
            this.sound_format = GetSoundFormat(channels, bits_per_sample);
            len();
        }

        public void len()
        {

            actualDataLength = (int)(sample_rate/this.bits_per_sample* multiplier);
        }

        public static ALFormat GetSoundFormat(int channels, int bits)
        {
            switch (channels)
            {
                case 1: return bits == 8 ? ALFormat.Mono8 : ALFormat.Mono16;
                case 2: return bits == 8 ? ALFormat.Stereo8 : ALFormat.Stereo16;
                default: throw new NotSupportedException("The specified sound format is not supported.");
            }
        }
    }
}
