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
    public class Pripojenie
    {
        
        static Settings1 setting;
        static byte[] data;  // 1
        static Socket socket; // 1
        void pripoj(string IP_adresa, int port,Socket socket1)
        {
            socket1 = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp); // 2
            socket1.Bind(new IPEndPoint(IPAddress.Any/*.Parse(setting.IP_adresa)*/, port)); // 3
        }
       // bool ukoncene=false;
        //Socket accepteddata;
        int MAXpocetHracov, OstavajúciPocetHracov;
        List<Socket> sockets;
        //Socket testovaniePripojenia;
#region Start server
        public Pripojenie()
        {
            Console.WriteLine(zistenieIP());
            sockets = new List<Socket>();
            setting = new Settings1();
            setting.PocetUzivatelov = 0;
            MAXpocetHracov = setting.PocetUzivatelov;
            
            
            if ((double)setting.PocetUzivatelov<= 0)
            {
                string sss;
                bool chyba;
                Console.Write("Zadajte pocet hráčov: ");
                do
                {

                    sss = "10";//Console.In.ReadLine();
                    try
                    {
                MAXpocetHracov= Convert.ToInt32(sss);
                        chyba = false;
                    }
                    catch
                    {
                        Console.WriteLine("Zadany chybny pocet.");
                        Console.Write("Zadajte pocet hráčov: ");
                        chyba=true;

                    }
                }while(chyba);
                
            }OstavajúciPocetHracov = MAXpocetHracov;
            Console.WriteLine("Max pocet hracov: " + MAXpocetHracov);
            Console.WriteLine("Pripájanie...");
            /*pripoj("", 6668, testovaniePripojenia);
            pripoj(setting.IP_adresa, setting.port,socket);*/
            //testovaniePripojenia = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp); // 2
            //testovaniePripojenia.Bind(new IPEndPoint(IPAddress.Any, 6668));
            //posielanieDat = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp); // 2
            //posielanieDat.Bind(new IPEndPoint(IPAddress.Any, 6667));
            socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp); // 2
            socket.Bind(new IPEndPoint(IPAddress.Any, 6666));
            
            //testovaniePripojenia.Listen(MAXpocetHracov);
            socket.Listen(MAXpocetHracov); // 4
            //posielanieDat.Listen(MAXpocetHracov);
            Console.WriteLine("Pripravený...");
            
            //OstavajúciPocetHostov--;
            Console.WriteLine("Ostavajúci Pocet Hracov: " + OstavajúciPocetHracov +".\n");
      ///
         Thread newThread = new Thread(new ThreadStart(ssss));
      /// 
            Thread vlakno = new Thread(new ThreadStart(vlakno1));
            Thread sssscasc = new Thread(new ThreadStart(test));
            //vlákna.Add(newThread);
            //vlákna.Add(vlakno);
            //Parallel.ForEach<Thread>(,) ;
            //sssscasc.IsBackground = newThread.IsBackground = vlakno.IsBackground = true;
            sssscasc.Start();
       ///
       newThread.Start();
       /// 
            vlakno.Start();
            
            #region
            /*Parallel.ForEach(vlákna, currentFile =>
        {
            
            currentFile.Start();

        } //close lambda expression
             );
            Parallel.ForEach(vlákna, currentFile =>
            {
                //currentFile.Join(50);

            } //close lambda expression
             );
            /*foreach (Thread fffdfvfvdf in vlákna)
            {
                fffdfvfvdf.Start();
            }
            foreach (Thread fffdfvfvdf in vlákna)
            {
                fffdfvfvdf.Join();
            }
            */
#endregion
        }List<Thread> hraci = new List<Thread>();
#endregion
      //  List<Thread> vlákna = new List<Thread>();
        //Socket posielanieDat;
        void vlakno1()
        {
            int j;
            Socket d = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            do
            {
                if (sockets.Count < MAXpocetHracov)
                try
                {
                    
                    //    if (testovaniePripojenia.Accept() != null || socket.Accept() != null)
                    {
                        
                        d = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                        d = socket.Accept();

                        data = new byte[d.SendBufferSize]; // 6
                        j = d.Receive(data); // 7
                        byte[] adata = new byte[j];         // 7
                        for (int i = 0; i < j; i++)         // 7
                            adata[i] = data[i];             // 7
                        string dat = Encoding.Default.GetString(adata);
                        string IP = "";
                        //try
                        {

                            for (int i = 0; dat[i] != '#' || i < dat.Length - 1; i++)
                            {
                                IP += dat[i];
                            }
                        }
                        /*catch (IndexOutOfRangeException e)
                        {
                            int i = 1;
                            Console.WriteLine("Pripojenie hráča sa skusa znova:");

                            j = d.Receive(data); // 7
                            adata = new byte[j];         // 7
                            for (int i = 0; i < j; i++)         // 7
                                adata[i] = data[i];             // 7
                            dat = Encoding.Default.GetString(adata);
                            for (int i = 0; dat[i] != '#' || i < dat.Length - 1; i++)
                            {
                                IP += dat[i];
                            }
                        }*/
                        Console.WriteLine("Hráč " + IP + "pripojení. Ostáva:" + OstavajúciPocetHracov);
                    }

                }

                catch{
                    int i = 1; 
                    Console.WriteLine("Pripojenie Hráča zlihalo");
                }
                finally
                {
                    
                    Console.WriteLine("reconecting..");
                    //Thread vlaknodd = new Thread(komunikaciaSHracom);

                    // hraci.Add(vlaknodd);
                    //hraci[(hraci.IndexOf(vlaknodd))].Start(d);
                    
                    players.Add(new Player(d, this));
                    sockets.Add(d);
                    byte[] joinrequirest = Encoding.Default.GetBytes("potvrdenie");
                    d.Send(joinrequirest);Console.WriteLine("Hráč je pripojení. Ostáva:" + OstavajúciPocetHracov);
                }
                //Console.Write("Pripojený Hráč");
#region
                /*
#region Príjem dát
                Console.WriteLine("čakanie na pripojenie hráča. Pripojenych" + OstavajúciPocetHostov);
                Socket d  ;
                if ((null==(d=socket.Accept())))
                {
                accepteddata = d; // 5
                sockets.Add(accepteddata);
                //newThread.();
                Console.WriteLine("Data boli prijaté:");
                
                data = new byte[accepteddata.SendBufferSize]; // 6
                int j = accepteddata.Receive(data); // 7
                byte[] adata = new byte[j];         // 7
                for (int i = 0; i < j; i++)         // 7
                    adata[i] = data[i];             // 7
                string dat = Encoding.Default.GetString(adata); //8
#endregion


                IPAddress address;
                if (IPAddress.TryParse(dat, out address))
                {
                    Console.WriteLine("Host " + dat + "pripojený.\nOstáva " + OstavajúciPocetHostov + "pocet hostov.");
                    OstavajúciPocetHostov--;
                }
                else
                {
                    Console.WriteLine(dat);
                    switch (dat)
                    {
                        case " disconnected":
                            OstavajúciPocetHostov++;
                            break;
                        default:
                            Console.WriteLine(dat);
                            break;
                    }
                }

                }*/
#endregion
            } while (true);
        }
        void test()
        {
            for (; ; )
            {
                cislo++;
                Console.WriteLine(sockets.Count.ToString()+ "  " + players.Count.ToString() + " " + hraci.Count.ToString());
                
                for (int i = 0; i < hraci.Count; i++)
                {
                    //if()
                    //hraci[hraci.IndexOf()].Abort();
                    //hraci.RemoveAt(hraci.IndexOf(this.komunikaciaSHracom));
                }

            }

        }
        double cislo=0;
#region Prijem Dát
        void ssss()
        {
            for (; ; )
            {
                try
                {
                    if( 0!=sockets.Count)
                    foreach (Socket accepteddata in sockets)
                    {
                        #region Príjem dát
                        // 5
                        //Console.WriteLine("Príjmanie dát.");
                        if (accepteddata.Connected)
                        {
                            data = new byte[accepteddata.SendBufferSize]; // 6
                            // Console.WriteLine("Data boli prijaté:");


                            int j = accepteddata.Receive(data); // 7
                            byte[] adata = new byte[j];         // 7
                            for (int i = 0; i < j; i++)         // 7
                                adata[i] = data[i];             // 7
                            string dat = Encoding.Default.GetString(adata); //8
                        #endregion
                            string dataprijate = "";
                            bool dsafsvd = false;
                            #region Posledné celé prijaté data
                            for (int i = dat.Length - 1; i >= 0; i--)
                            {


                                if (dsafsvd)
                                {
                                    dataprijate += dat[i];
                                    if (dat[i] == '$')
                                        break;

                                }

                                if (dat[i] == '$')
                                {
                                    if (dat[i - 1] == '$')
                                        i--;
                                    dsafsvd = true;
                                }

                            }
                            dataprijate = ReverseString(dataprijate);//otočenie stringu(kvoli tomu že to ide odzadu)
                            #endregion

                            Console.WriteLine(dataprijate); //vypísanie prijatých dát


                            IPAddress address;
                            if (IPAddress.TryParse(dat, out address))
                            {
                                Console.WriteLine("Hrac " + dat + "pripojený.\nOstáva " + OstavajúciPocetHracov + "pocet hracov.");
                                OstavajúciPocetHracov--;
                            }
                            else
                            {
                               // Console.WriteLine(dat);
                                #region switch - Prijate data

                                switch (dat)
                                {

                                    case " disconnected":
                                        #region Switch-Disconnnected
                                        OstavajúciPocetHracov++; //zviśenie počtu pripojitelných hráčov

                                        foreach (Player player in players)
                                        {
                                            if (player.socket == accepteddata)
                                                players.Remove(player);
                                        }
                                        sockets.Remove(accepteddata);
                                        break;
                                        #endregion
                                    default:
                                        #region ukladanie a odosielanie dát
                                        Player ssss;
                                        string dataa = "";
                                        byte[] dataaa = new byte[accepteddata.SendBufferSize]; ///vytvorenie pola Byte[] 
                                        
                                        foreach (Player player in players)
                                        {
                                            bool scsac = false;
                                            byte[] datasaa;
                                            if (player.socket != accepteddata)
                                            {
                                                //byte[] dataaa = new byte[accepteddata.SendBufferSize];
                                                //Zistenie ktorý hráć je aktualny
                                               // ssss = player;
                                                player.aktualizovat(dataprijate);
                                                /*
                                                if (players.Count>1)
                                                {
                                                    scsac = true;
                                                    datasaa = Encoding.Default.GetBytes(dataa);

                                                    player.socket.Send(datasaa);
                                                }*/
                                            }
                                            else{
                                                /*if ((int)players.Count < 1 && !scsac)
                                                {
                                                    
                                                    datasaa = Encoding.Default.GetBytes("empty!");
                                                    player.socket.Send(datasaa);
                                                }*/
                                                dataa += "^"+player.naodoslanie() + "^"; //Pridanie do dataa 
                                                
                                            }



                                        }
                                        dataaa = Encoding.Default.GetBytes(dataa);

                                        #region Odosielanie Dát
                                        foreach (Player player in players)
                                        {
                                            if (player.socket != accepteddata)
                                            {
                                                //byte[] dataaa = new byte[accepteddata.SendBufferSize];


                                                byte[] datasaa = Encoding.Default.GetBytes(dataa);

                                                player.socket.Send(datasaa);


                                            }
                                        }
                                        #endregion


                                        //Console.WriteLine(dat);
                                        break;
                                        #endregion
                                }
                                #endregion

                            }
                        }
                        //else
                        
                        {
                            
                            foreach (Player player in players)
                            {
                                if(!player.socket.Connected){
                                if (player.socket == accepteddata)
                                {
                                    
                                    sockets.Remove(accepteddata);
                                }
                                players.Remove(player);}
                            }
                        }

#region Odstránenie hráča
                        /*foreach (Player player in players)
                        {
                            if (!player.socket.Connected)
                            {
                                
                                foreach (Socket socket in sockets)
                                {
                                    sockets.Remove(player.socket);
                                }
                                players.Remove(player);
                            }
                        }*/
#endregion

                    }
                }
                catch
                {
                    //int i;
                }

            }
        }
        public List<Player> players = new List<Player>();
        public static string ReverseString(string s) //otočenie stringu
        {
            char[] arr = s.ToCharArray();
            Array.Reverse(arr);
            return new string(arr);
        }
#endregion
        public string zistenieIP()
        {
            IPHostEntry host;
            string localIP = "?";
            host = Dns.GetHostEntry(Dns.GetHostName());
            foreach (IPAddress ip in host.AddressList)
            {
                if (ip.AddressFamily.ToString() == "InterNetwork")
                {
                    localIP = ip.ToString();
                }
            }
            return localIP;

        }
        void komunikaciaSHracom(Object param)
        {
            Socket socket=(Socket)param;
            for (; ; )
            {
                try
                {
                    
                            #region Príjem dát
                            // 5
                            //Console.WriteLine("Príjmanie dát.");
                            if (socket.Connected)
                            {
                                data = new byte[socket.SendBufferSize]; // 6
                                // Console.WriteLine("Data boli prijaté:");


                                int j = socket.Receive(data); // 7
                                byte[] adata = new byte[j];         // 7
                                for (int i = 0; i < j; i++)         // 7
                                    adata[i] = data[i];             // 7
                                string dat = Encoding.Default.GetString(adata); //8
                            #endregion
                                string dataprijate = "";
                                bool dsafsvd = false;
                                #region Posledné celé prijaté data
                                for (int i = dat.Length - 1; i >= 0; i--)
                                {


                                    if (dsafsvd)
                                    {
                                        dataprijate += dat[i];
                                        if (dat[i] == '$')
                                            break;

                                    }

                                    if (dat[i] == '$')
                                    {
                                        if (dat[i - 1] == '$')
                                            i--;
                                        dsafsvd = true;
                                    }

                                }
                                dataprijate = ReverseString(dataprijate);//otočenie stringu(kvoli tomu že to ide odzadu)
                                #endregion

                                Console.WriteLine(dataprijate); //vypísanie prijatých dát


                                IPAddress address;
                                if (IPAddress.TryParse(dat, out address))
                                {
                                    Console.WriteLine("Hrac " + dat + "pripojený.\nOstáva " + OstavajúciPocetHracov + "pocet hracov.");
                                    OstavajúciPocetHracov--;
                                }
                                else
                                {
                                    //Console.WriteLine(dat);
                                    #region switch - Prijate data

                                    switch (dat)
                                    {

                                        case " disconnected":
                                            #region Switch-Disconnnected
                                            OstavajúciPocetHracov++; //zviśenie počtu pripojitelných hráčov

                                            foreach (Player player in players)
                                            {
                                                if (player.socket == socket)
                                                    players.Remove(player);
                                            }
                                            sockets.Remove(socket);
                                            break;
                                            #endregion
                                        default:
                                            #region ukladanie a odosielanie dát
                                            Player ssss;
                                            string dataa = "";
                                            byte[] dataaa = new byte[socket.SendBufferSize]; ///vytvorenie pola Byte[] 

                                            foreach (Player player in players)
                                            {
                                                if (player.socket == socket)
                                                {
                                                    //byte[] dataaa = new byte[accepteddata.SendBufferSize];
                                                    //Zistenie ktorý hráć je aktualny
                                                    ssss = player;
                                                    ssss.aktualizovat(dataprijate);
                                                }
                                                else
                                                    dataa += player.naodoslanie() + "^"; //Pridanie do dataa 





                                            }
                                            dataaa = Encoding.Default.GetBytes(dataa);

                                            #region Odosielanie Dát
                                            foreach (Player player in players)
                                            {
                                                if (player.socket == socket)
                                                {
                                                    //byte[] dataaa = new byte[accepteddata.SendBufferSize];


                                                    byte[] datasaa = Encoding.Default.GetBytes(dataa);

                                                    player.socket.Send(datasaa);


                                                }
                                            }
                                            #endregion


                                            //Console.WriteLine(dat);
                                            break;
                                            #endregion
                                    }
                                    #endregion

                                
                            }
                            //else

                            {

                               
                                {
                                    if (!socket.Connected)
                                    { sockets.Remove(socket);
                                        foreach (Player player in players)
                                        players.Remove(player);

                                        
                                    }
                                    
                                }
                            }



                        }
                }
                catch
                {
                }

            }
        }
    }
}
