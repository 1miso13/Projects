using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Net.Sockets;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.GamerServices;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Media;
using System.Threading;
using System.Threading.Tasks;
namespace MojEngine
{
    public class Connection
    {
        Engine engine;
        private byte[] data;
        Socket s,ss;
        public string prijate;
        public IPAddress IPAddres/* = IPAddress.Parse(/*"127.0.0.1""192.168.1.15")*/;
        public Connection(Engine engine,string ip)
        {
            engine.text = "connecting";
            this.engine=engine;
            s = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            ss = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            try // 1
            {
                IPAddres= /* IPAddress/*IPAddress.Broadcast/**//*IPAddress.Parse("192.168.0.15")*/ IPAddress.Parse(ip);
              //  ss.Connect(IPAddres, 6668); // 2
                s.Connect(IPAddres, 6666);
                string q = zistenieIP();              // 3
                data = Encoding.Default.GetBytes(q + "#");    // 3
                s.Send(data);
                
                byte[] datas = new byte[s.SendBufferSize];
                
                int j = s.Receive(datas); // 7
                byte[] adata = new byte[j];         // 7
                for (int i = 0; i < j; i++)         // 7
                    adata[i] = datas[i];             // 7
                string dat = Encoding.Default.GetString(adata);
                prijate = dat;
                
                engine.pád=false;

                if (dat == "potvrdenie")
                {
                    engine.pád = false;
                }
                else
                {
                    engine.pád = true;
                }
                odoslanie = new Thread(/*new ThreadStart*/(Update));
                prijatiedat = new Thread(/*new ThreadStart*/(prijatData));
                prijatiedat.IsBackground = odoslanie.IsBackground= true;
                
                odoslanie.Start();
                prijatiedat.Start();
                engine.text = "connected";
            }
            catch // 1
            {
                engine.pád = true;
                // nepripojeno
            }
            
        }Thread prijatiedat ,odoslanie;
        Vector3 MyPosition;/* bool d=true;*/
        public  void Update()
        {
            for (; !koniec; )
                if (!s.Connected)

                    engine.pád = true;
                else
                {
                    MyPosition = engine.pozice;
                    naOdoslanie(MyPosition.X.ToString());
                    naOdoslanie(MyPosition.Y.ToString());
                    naOdoslanie(MyPosition.Z.ToString());
                    naOdoslanie(engine.fPitch.ToString());
                    naOdoslanie(engine.fYaw.ToString());

                    odoslat(odoslanie_data);
                }
            //Thread odoslatdat = new Thread(new ThreadStart(odoslat));
            
            
        }
        public bool koniec=false;
        string SpoziciaX, SpoziciaY, SpoziciaZ, SYaw, SPitch;
        public void prijatData()
        {
            int pocetHracov = 0;
            for (;!koniec ; )
                try
                {
                    int dvdbfbg=0;
                   ///
                   /// 
                   /// 
                    do
                    {
                        int j;
                        byte[] data = new byte[s.SendBufferSize];
                        do
                        {
                            int d = j = s.Receive(data);
                        }
                        while (j == 0);// 7
                        byte[] adata = new byte[j];         // 7
                        for (int i = 0; i < j; i++)         // 7
                            adata[i] = data[i];             // 7
                        string dat = Encoding.Default.GetString(adata);
                        engine.data = dat;
                        char znakA='!';
                        string testttt="";
                        bool dsafsvAd = false;
                        for (int i = dat.Length - 1; i >= 0; i--)
                        {


                            if (dsafsvAd)
                            {
                                testttt += dat[i];
                                if (dat[i] == znakA)
                                    break;

                            }

                            if (dat[i] == znakA)
                            {
                                if (dat[i - 1] == znakA)
                                    i--;
                                dsafsvAd = true;
                            }

                        }
                        testttt = ReverseString(testttt);
                        if (testttt != "empty" || testttt != "!empty" || testttt != "empty!")
                        {
                            ///
                            /// 
                            /// "100#346,4152#270#-13,12959#-0,2599994#-1,356575^100#346,4152#270#-13,12959#-0,2599994#-1,356575^100#346,4152#270#-13,12959#-0,2599994#-1,356575^100#346,4152#270#-13,12959#-0,2599994#-1,356575^100#346,4152#270#-13,12959#-0,2599994#-1,356575^100#346,4152#270#-13,12959#-0,2599994#-1,356575^100#346,4152#270#-13,12959#-0,2599994#-1,356575^100#346,4152#270#-13,12959#-0,2599994#-1,356575^100#346,4152#270#-13,12959#-0,2599994#-1,356575^100#346,4152#270#-13,12959#-0,2599994#-1,356575^100#346,4152#270#-13,12959#-0,2599994#-1,356575^100#346,4152#270#-13,12959#-0,2599994#-1,356575^100#346,4152#270#-13,12959#-0,2599994#-1,356575^100#346,4152#270#-13,12959#-0,2599994#-1,356575^100#346,4152#270#-13,12959#-0,2599994#-1,356575^100#346,4152#270#-13,12959#-0,2599994#-1,356575^100#346,4152#270#-13,12959#-0,2599994#-1,356575^100#346,4152#270#-13,12959#-0,2599994#-1,356575^100#346,4152#270#-13,12959#-0,2599994#-1,356575^100#346,4152#270#-13,12959#-0,2599994#-1,356575^100#346,4152#270#-13,12959#-0,2599994#-1,356575^100#346,4152#270#-13,12959#-0,2599994#-1,356575^100#346,4152#270#-13,12959#-0,2599994#-1,356575^100#346,4152#270#-13,12959#-0,2599994#-1,356575^100#346,4152#270#-13,12959#-0,2599994#-1,356575^100#346,4152#270#-13,12959#-0,2599994#-1,356575^100#346,4152#270#-13,12959#-0,2599994#-1,356575^100#346,4152#270#-13,12959#-0,2599994#-1,356575^100#346,4152#270#-13,12959#-0,2599994#-1,356575^100#346,4152#270#-13,12959#-0,2599994#-1,356575^"

                            string zivot = "";
                            string dataPrijate = "";
                            int moznost = 0;
                            SpoziciaX = "";
                            SpoziciaY = "";
                            SpoziciaZ = "";
                            SYaw = "";
                            SPitch = "";
                            string pocetHracov1 = ""; bool dsafsvd = false;
                            char znak = '^';
                            #region Posledné celé prijaté data
                            for (int i = dat.Length - 1; i >= 0; i--)
                            {


                                if (dsafsvd)
                                {
                                    dataPrijate += dat[i];
                                    if (dat[i] == znak)
                                        break;

                                }

                                if (dat[i] == znak)
                                {
                                    if (dat[i - 1] == znak)
                                        i--;
                                    dsafsvd = true;
                                }

                            }
                            dataPrijate = ReverseString(dataPrijate);//otočenie stringu(kvoli tomu že to ide odzadu)
                            #endregion

                            try
                            {
                                for (int i = 1;
                                   i < dataPrijate.Length;
                                   i++)
                                {
                                    if (dataPrijate[i] == '#')
                                    {
                                        moznost++;
                                        i++;
                                    }
                                    switch (moznost)
                                    {
                                        case 0: zivot += dataPrijate[i];
                                            break;
                                        case 1: SpoziciaX += dataPrijate[i];
                                            break;
                                        case 2: SpoziciaY += dataPrijate[i];
                                            break;
                                        case 3: SpoziciaZ += dataPrijate[i];
                                            break;
                                        case 4: SPitch += dataPrijate[i];
                                            break;
                                        case 5: SYaw += dataPrijate[i];
                                            break;
                                        case 6: pocetHracov1 += dataPrijate[i];
                                            break;
                                    }
                                }
                                // engine.health = Convert.ToInt32(zivot);
                                Matrix rotace = Matrix.CreateFromYawPitchRoll(float.Parse(SYaw), float.Parse(SPitch), 0);

                                Vector3 vector = new Vector3(
                                            float.Parse(SpoziciaX),
                                            float.Parse(SpoziciaY),
                                            float.Parse(SpoziciaZ));

                                Model3D hrac = new Model3D(vector, rotace, new Vector3(1, 1, 1), "dude");
                                hrac.Rotace *= Matrix.CreateRotationY((float)Math.PI);
                                hrac.Pozice += new Vector3(0, -70, 0);
                                hraci.Add(hrac);

                                pocetHracov = Int32.Parse(pocetHracov1);
                            }
                            catch
                            {
                                int idcsdcsdcsvdfbfggf = 5;

                            }



                            dvdbfbg++;
                        }
                    }
                    ///
                    /// 
                    /// 
                    while (dvdbfbg < pocetHracov);
                   // engine.ostatnyHraci.Clear();
                 //   engine.aktalizovanieNepratelov = true;
                    engine.ostatnyHraci.Clear();
                 //   if(!engine.test)
                    foreach (Model3D hrac in hraci)
                    {
                        engine.ostatnyHraci.Add(hrac);
                    }// engine.aktalizovanieNepratelov = false;
                    //engine.ostatnyHraci=hraci;
                    hraci.Clear();
                }
                catch
                {
                    engine.pád = true;
                }
                        
        }
        List<Model3D> hraci =new List<Model3D>();
        public static string ReverseString(string s) //otočenie stringu
        {
            char[] arr = s.ToCharArray();
            Array.Reverse(arr);
            return new string(arr);
        }
        string odoslanie_data="";
        public void naOdoslanie (string dd)
        {

            odoslanie_data+= "#"+dd;
        }
        void odoslat(string q)
        {
            try
            {
                //s.Connect(IPAddres, 6666);
                data = Encoding.Default.GetBytes("$" + q + "$");
                // s.Connect(IPAddres, 6666);
                //s.Receive(new byte[55]);
                
                s.Send(data);
                
            }
            catch // 1
            {
                int i = 0;
                i++;
                //engine.pád = true;
                // nepripojeno
            }
            finally
            {
                odoslanie_data = "";
            }
        }
        public void disconnect()
        {
           // s.Close();
           // s = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            //if (IPAddres != null)
            /*this.odoslanie.Suspend();
            this.prijatiedat.Suspend();*/
            {
                try
                {
                    // s.Connect(IPAddres, 6666);
                    data = Encoding.Default.GetBytes(zistenieIP() + "#diconnected#");
                    s.Send(data);

                }
                catch
                {
                    //engine.pád = true;
                }
                finally{
                    try
                    {
                        prijatiedat.Abort();
                        odoslanie.Abort();
                       // s.Disconnect(false);
                      //  ss.Disconnect(false);
                        s.Close();
                        ss.Close();
                    }
                    catch
                    {
                        //engine.pád = true;
                    }
                }
            }
            
            
        }
        ~Connection()
        {
            koniec = true;
            disconnect();
           // Connection.
        }
        public string zistenieIP()
        {
            IPHostEntry host;
            string localIP = "?";
            host = Dns.GetHostEntry(Dns.GetHostName());
            foreach (IPAddress ip in host.AddressList)
            {
                if(ip.AddressFamily.ToString() == "InterNetwork")
                {
                    localIP = ip.ToString();
                }
            }
            return localIP;

        }


    }
}
