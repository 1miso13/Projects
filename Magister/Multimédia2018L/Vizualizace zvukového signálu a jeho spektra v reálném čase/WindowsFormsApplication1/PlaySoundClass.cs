using OpenTK;
using OpenTK.Audio;
using OpenTK.Audio.OpenAL;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace WindowsFormsApplication1
{
    static class PlaySoundClass
    {
        static List<int> buffers = new List<int>();
        static int buffer;
        static int source = AL.GenSource();
        static int state;
        public static int buffersize = 1600;
        public static SoundClass sound_class;
        private static AudioContext audio_context;
        static public int ticks = 0;
        private static int playingType;
        static private int tick_playing= 1;
        public static ALSourceState ALState = ALSourceState.Stopped;
        public static void Play(SoundClass sound_Class,bool Recording)
        {
            if (Recording)
            {
                playingType = 2;
            }
            else
            {
                playingType = 1;
            }
            sound_class = sound_Class;
            audio_context = new AudioContext();
            source = AL.GenSource();
            ticks = tick_playing;
            ALState = ALSourceState.Playing;
        }
        public static void Play1(SoundClass sound_Class)
        {
            sound_class = sound_Class;
            using (AudioContext context = new AudioContext())
            {
                source = AL.GenSource();
                
                //for (int i = 0; i < 21462; i++)
                //{//fill array test
                //    sound_class.sound_data[i] = (byte)i;
                //}
                int offset = sound_class.sound_data.Length % buffersize;
                //first buffer
                //var bufferArray = sound_class.sound_data.SubArray(0, buffersize > offset ? buffersize : offset);

                //AL.BufferData(buffer, sound_class.sound_format, bufferArray, bufferArray.Length, sound_class.sample_rate);

                //AL.Source(source, ALSourcei.Buffer, buffer);
                //AL.SourcePlay(source);
                AL.SpeedOfSound((float)ALGetFloat.SpeedOfSound);
                for (int i = 0; i < sound_class.sound_data.Length / buffersize+(sound_class.sound_data.Length % buffersize ==0? 0:1); i++)
                {
                    buffer = AL.GenBuffer();
                    int processed = 0;
                    do
                    {
                        AL.GetSource(source, ALGetSourcei.BuffersProcessed, out processed);
                    }
                    while (processed == 0 && AL.GetSourceState(source) == ALSourceState.Playing);
                    //switch buffers
                    //for (; processed > 0; processed--,i++)
                    {

                        Trace.WriteLine("soundData processed - " + i);
                        byte[] bufferArray1 = sound_class.sound_data.SubArray(buffersize * i, i != (sound_class.sound_data.Length / buffersize)  ? buffersize : offset-1);
                        AL.BufferData(buffer, sound_class.sound_format, bufferArray1, bufferArray1.Length, sound_class.sample_rate);
                        AL.SourceQueueBuffer(source, buffer);
                        Trace.WriteLine("B1");

                    }
                    //source = AL.GenSource();
                    //var bufferArray = sound_data.SubArray(buffersize * i, i==(sound_data.Length / buffersize)-1 ? buffersize: offset);//.Skip(buffersize * i).Take(buffersize).ToArray();
                    //AL.BufferData(buffer, GetSoundFormat(channels, bits_per_sample), sound_data, sound_data.Length, sample_rate);
                    //AL.BufferData(buffer, GetSoundFormat(channels, bits_per_sample), bufferArray, buffersize>bufferArray.Length?buffersize:bufferArray.Length, sample_rate);
                    if (AL.GetSourceState(source) != ALSourceState.Playing)
                        AL.SourcePlay(source);
                    int[] freedbuffers;
                    
                    int BuffersProcessed;
                    AL.GetSource(source, ALGetSourcei.BuffersProcessed, out BuffersProcessed);
                    if (BuffersProcessed == 0)
                        continue;
                    freedbuffers = AL.SourceUnqueueBuffers(source, BuffersProcessed);
                    AL.DeleteBuffers(freedbuffers);

                }
                Trace.Write("Playing");

                // Query the source to find out when it stops playing.
                do
                {

                    //Trace.Write(".");
                    AL.GetSource(source, ALGetSourcei.SourceState, out state);
                }
                while ((ALSourceState)state == ALSourceState.Playing);

                //Trace.WriteLine("");

                //AL.DeleteSource(source);
            }
        }
        static public void StopPlaying()
        {
            ALState = ALSourceState.Stopped;
            part = 0;
            ticks = 0;
            if (audio_context!=null)
            {
                audio_context.Dispose();
                audio_context = null;
            }
            AL.SourceStop(source);
            AL.DeleteSource(source);
            AL.DeleteBuffer(buffer);
        }
        static public void PauseMusic()
        {
            if (ALState == ALSourceState.Paused)
            {
                ticks = tick_playing;
                AL.SourcePlay(source);
                ALState = ALSourceState.Playing;
            }
            else
            {
                ALState = ALSourceState.Paused;
                ticks = 0;
                AL.SourcePause(source);
            }
        }
        static DateTime lastUpdateWave;
        static byte[] bufferArray;
        public static int partIndex;
        static int size;
        internal static void Update()
        {
            if (ALState != ALSourceState.Playing)
            {
                return;
            }
            int processed;
            if (bufferArray!=null || lastUpdateWave==null)
            {
                TimeSpan elapsed = DateTime.Now - lastUpdateWave;
                size = sound_class.sample_rate*elapsed.Milliseconds/1000;
                addSample(partIndex, size, bufferArray);
                if (size>0)
                {
                    lastUpdateWave = DateTime.Now;
                    partIndex += size;
                }
            }
            AL.GetSource(source, ALGetSourcei.BuffersProcessed, out processed);
            
            if (processed == 0 && part !=0)
            {
                return;
            }
            if (part == 0)
            {//pociatocne naplnenie bufferu
                processed = 3;//= pocet bufferov vo fronte
            }
             for (int i = processed; i > 0; i--)
            {
                if (bufferArray!=null)
                {
                    addSample(partIndex, bufferArray.Length - partIndex, bufferArray);
                }
                switch (playingType)
                {
                    case 1://file
                        bufferArray=UpdateAudioBuffer();
                        //stop if over
                        if (part == sound_class.sound_data.Length / buffersize /*+ (sound_class.sound_data.Length % buffersize == 0 ? -1 : 0)*/)
                        {
                            //prahrala sa aj posledna cast - koncime
                            part = 0;
                            StopPlaying();
                            return;
                        }
                        break;
                    case 2://record
                        bufferArray=UpdateRecordBuffer();
                        break;
                    default://never used
                        bufferArray = new byte[1];
                        break;
                }
                //AL.SpeedOfSound((float)ALGetFloat.SpeedOfSound * 50.0f);
                

                AL.SourceUnqueueBuffer(source);
                buffer = AL.GenBuffer();
                AL.BufferData(buffer, sound_class.sound_format, bufferArray, bufferArray.Length, sound_class.sample_rate);
                AL.SourceQueueBuffer(source, buffer);
                Trace.WriteLine("B1");

                if (AL.GetSourceState(source) != ALSourceState.Playing)
                {
                    AL.SourcePlay(source);
                }
                //Thread.Sleep(1/sound_class.sample_rate*1000000);
                freeBuffers();
                nextPart();
            }
        }

        private static void nextPart()
        {
            lastUpdateWave = DateTime.Now;
            partIndex = 0;
        }

        private static int addSample(int fromPart,int size, byte[] array)
        {//if size = 0 then do nothing
            for (int i = 0; i < size; i++)
            {
                if ((fromPart+i) * sound_class.bits_per_sample >= array.Length)
                {
                    return size - i;
                }
                sound_class.AddDataToQueue(getSampleAt(array,fromPart+i,sound_class.bits_per_sample));
            }
            return 0;
        }

        private static int getSampleAt(byte[] array, int i, int bits_per_sample)
        {
            int d;
            switch (bits_per_sample)
            {
                case 8:
                    d = array[i*8];
                    break;
                case 16:
                    d = BitConverter.ToInt16(array, i*16);
                    break;
                case 24:
                    byte[] arrayb = { 0, array[i*24], array[(i + 1) * 24], array[(i + 2)*24] };

                    d = BitConverter.ToInt32(arrayb, 0);
                    break;
                case 32:
                    d = BitConverter.ToInt32(array, i * 32);
                    break;
                default:
                    throw new NotSupportedException("unsupported bits per sample");
            }
            return d;
        }

        private static byte[] UpdateRecordBuffer()
        {
            Trace.WriteLine("Record part - " + part);
            part++;
            return sound_class.recordClass.GetBufferSample();
        }
        public static int part;
        internal static byte[] UpdateAudioBuffer()
        {
            Trace.WriteLine("Audio part - " + part);

            int offset = sound_class.sound_data.Length % buffersize;
            byte[] bufferArray = sound_class.sound_data.SubArray(buffersize * part, part != (sound_class.sound_data.Length / buffersize) ? buffersize : offset - 1);
            part++;
            return bufferArray;
        }
        static void freeBuffers()
        {
            if (AL.GetSourceState(source) != ALSourceState.Playing)
                AL.SourcePlay(source);
            int[] freedbuffers;

            int BuffersProcessed;
            AL.GetSource(source, ALGetSourcei.BuffersProcessed, out BuffersProcessed);
            if (BuffersProcessed == 0)
                return;
            freedbuffers = AL.SourceUnqueueBuffers(source, BuffersProcessed);
            AL.DeleteBuffers(freedbuffers);
        }
        
    }
}
