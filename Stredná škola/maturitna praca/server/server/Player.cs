using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Threading.Tasks;

namespace server
{
    public class Player
    {
        Pripojenie p;
        public Socket socket;
        public Player(Socket socket,Pripojenie p)
        {
            this.p = p;
            Health = 100;
            this.socket = socket;
        }
        public int Health;
        string Prijate="";
        public float poziciaX, poziciaY, poziciaZ, Yaw, Pitch;
        string SpoziciaX, SpoziciaY, SpoziciaZ, SYaw, SPitch;
        public void aktualizovat(string prijate)
        {
            Prijate = prijate;
            int moznost=0;
            SpoziciaX="";
            SpoziciaY="";
            SpoziciaZ="";
            SYaw = ""; 
            SPitch="" ;


            for (int i = 1;
                i < Prijate.Length ;
                i++)
            {
                try
                {
                    if (Prijate[i] == '#')
                    {
                        moznost++;
                        i++;
                    }
                    switch (moznost)
                        {
                            case 1: SpoziciaX +=Prijate[i];
                                break;
                            case 2: SpoziciaY += Prijate[i];
                                break;
                            case 3: SpoziciaZ += Prijate[i];
                                break;
                            case 4: SYaw += Prijate[i];
                                break;
                            case 5: SPitch += Prijate[i];
                                break;
                        }
                 
   
                }
                catch { }

                


            }
            try
                {
            poziciaX = float.Parse(SpoziciaX);
            poziciaY = float.Parse(SpoziciaY);
            poziciaZ = float.Parse(SpoziciaZ);
            Yaw = float.Parse(SYaw);
            Pitch = float.Parse(SPitch);
            SpoziciaX = "";
            SpoziciaY = "";
            SpoziciaZ= "";
            SYaw = "";
            SPitch= "";
                }
            catch { }
        }
        public string naodoslanie()
        {
            string d;
            d = Health.ToString()+ "#";
            foreach(Player player in p.players)
            {
                if(player!= this)
                    d += player.poziciaX.ToString() + "#" + player.poziciaY.ToString() + "#" + player.poziciaZ.ToString() + "#" + player.Yaw.ToString() + "#" + player.Pitch.ToString()+ "#" + p.players.Count;
            }
            char[] arr = d.ToCharArray();
            //Array.Reverse(arr);
            return new string(arr);
           // return d;
        }

    }
}
