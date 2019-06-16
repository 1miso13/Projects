using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using FFTWSharp;
using System.Runtime.InteropServices;

namespace WindowsFormsApplication1
{
    public class DFTClass
    {
       static int DOUBLESIZE = sizeof(double);
        IntPtr ptr;
        int Length;
        IntPtr plan;
        public DFTClass(int Length)
        {
            this.Length = Length;
            ptr = fftw.malloc(Length * DOUBLESIZE);
            plan = fftw.dft_1d(Length / 2, ptr, ptr, fftw_direction.Forward, fftw_flags.Estimate);
        }
        void changeLength(int Length)
        {
            fftw.destroy_plan(plan);
            fftw.free(ptr);
            ptr = fftw.malloc(Length * DOUBLESIZE);
            plan = fftw.dft_1d(Length / 2, ptr, ptr, fftw_direction.Forward, fftw_flags.Estimate);
        }
        public double[] FFT(double []data)
        {
            data = windowFunction_Hamming(data);
            int n = data.Length*2;
            if (n!=this.Length)
            {
                changeLength(n);
            }
            var comp = new double[n];
            for (int i = 0; i < n/2; i++)
            {
                comp[2 * i] = data[i];
            }
            Marshal.Copy(comp, 0, ptr, n);
            fftw.execute(plan);
            var fftComplex = new double[n];
            var fft = new double[n/4];
            Marshal.Copy(ptr, fftComplex, 0, n);
            for (int i = 0; i < n / 4; i++)
            {
                fft[i] =Math.Sqrt(Math.Pow(fftComplex[i * 2], 2) + Math.Pow(fftComplex[i * 2 + 1], 2));
                if (fft[i] < 0) {
                    fft[i]=-fft[i];
                }
            }

            return fft;
        }
        /// <summary>
        /// clean
        /// </summary>
        ~DFTClass()
        {
            fftw.destroy_plan(plan);
            fftw.free(ptr);
            fftw.cleanup();
        }
        double[] windowFunction_Hamming(double[] data)
        {
            double[] data_new = new double[data.Length];
            float alfa = 0.54f;
            float beta = 0.46f;
            int lenght = data.Length - 1;
            for (int n = 0; n <= lenght; n++)
            {
                data_new[n] = data[n] *(alfa - beta * Math.Cos(2 * Math.PI * n / lenght));
            }
            return data_new;
        }
    }
}
