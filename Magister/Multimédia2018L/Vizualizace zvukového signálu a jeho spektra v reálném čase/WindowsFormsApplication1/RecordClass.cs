using OpenTK;
using OpenTK.Audio;
using OpenTK.Audio.OpenAL;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WindowsFormsApplication1
{
    public class RecordClass
    {
        public int sampling_rate { get;  set; } 
        public AudioCapture audio_capture;
        private int buffer_Size;

        public  bool playing { get; private set; }

        public void RecordButton()
        {
            if (playing)
            {//stop
                playing = false;
                audio_capture.Stop();
                PlaySoundClass.StopPlaying();
                //audio_capture.
            }
            else
            {
                //play
                playing = true;
                audio_capture.Start();
                SoundClass sound_class = new SoundClass(this,1/*channels*/, 16/*bits_per_sample*/, sampling_rate);
                PlaySoundClass.Play(sound_class,true);
            }

        }
        
        public RecordClass()
        {
            buffer_Size = 2000;
            playing = false;
            sampling_rate = 44100;
            audio_capture = new AudioCapture(AudioCapture.DefaultDevice, sampling_rate, ALFormat.Mono16, buffer_Size);
        }
        ~RecordClass()
        {
            audio_capture.Dispose();
            audio_capture = null;
        }
        byte[] buffer = new byte[512];
        public byte[] GetBufferSample()
        {
            int available_samples = audio_capture.AvailableSamples;
            if (available_samples * 2 > buffer.Length * BlittableValueType.StrideOf(buffer))
            {
                buffer = new byte[MathHelper.NextPowerOfTwo(
                    (int)(available_samples * 2 / (double)BlittableValueType.StrideOf(buffer) + 0.5))];
            }
            audio_capture.ReadSamples(buffer, available_samples);
            return buffer;
        }
    }
}
