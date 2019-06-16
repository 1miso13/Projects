using System;
using System.Collections.Generic;
using System.Linq;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Input;
using System.IO;
using System.Runtime.InteropServices;
namespace MojEngine
{
    public class Engine
    {
        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        public static extern uint MessageBox(IntPtr hWnd, String text, String caption, uint type);

        //unsafe
        
       // public Enemy jbbjkkkkkkkkkkkkkb=new Enemy(new Vector3(2500,/*24*/2, 0), Matrix.CreateRotationX(0), /*"Victoria-hat-dance"*/"dude");
        /*public *///zbran *zbran1;
        public string text;
        public float rychlostpadania, vyskaodpodlahy;
        public Vector2 rozlÌöenie;
        public string IPadress;
        public bool p·d;
        public Connection Connection;
        //public BoundingBox ddd;
        private GameTime gameTime;
        private Game parent;
        public GraphicsDevice graphics;
        private SpriteBatch spriteBatch;
        private ContentManager content;
        private Input fInput;
        public Input Input { get { return fInput; } }
        public float fPitch;
        public float fYaw;
        public Vector3 pozice;
        public bool aktivne;
        public SpriteFont maly;
        public Vector3 pozicel;
        public int health;
        public Random random = new Random();
        public bool renderBoudingObject = true;
        public int jkjbkjb = 0;
        public int jkjbkjbd = 0, kolizia_S_Objektom_index=0;

        public List<Model3D> ostatnyHraci=new List<Model3D>();


        public GameTime GameTime
        {
            get { return gameTime; }
        }
        public void Update(GameTime time)
        {
            this.gameTime = time;
            if (GraphicsDevice.Adapter.IsDefaultAdapter && aktivne)
            fInput.Update();
            
            foreach (GameScreen okno in Screens)
            {
                okno.Update();
            }
            //if (Input.DrzenaKlavesa(Keys.Escape))
            
   //         Connection.Update();
            if ( p·d == true)
            {
                MessageBox(new IntPtr(0), ("Hra sa odpojila od serveru. Sk˙ste sa pripojiù neskÙr."), "Game error", 0);
                parent.Exit();
            }
            /*try
            {
                for (int j = 0; j < ostatnyHraci.Count; j++)
                {
                    ostatnyHraci[j].Update();
                }
            }
            catch
            {
                sdsdds = 1;
            }*/
            //ostatnyHraci.Clear();
           // Connection.prijatData();
        }
        int sdsdds;
        public void load()
        {
            
            
        }
        ~Engine()
        {
            Connection.koniec = true;
            Connection.disconnect();
           // Connection.
        }

      

        public void Draw(GameTime time)
        {
            
            this.gameTime = time;
            foreach (GameScreen okno in Screens)
            {
                okno.Draw();
                
            }
            /*
            if (!aktalizovanieNepratelov)
            {
                test = true;
                foreach (Model3D hraci in ostatnyHraci)
                {

                    hraci.Draw();
                }
                test = false;
            }*/
        }public bool test;
        public Game Parent
        {
            get { return parent; }
        }
        public GraphicsDevice GraphicsDevice {
            get { return graphics; }
        }
        public SpriteBatch SpriteBatch
        {
            get { return spriteBatch; }
        }
        public ContentManager Content{
            get { return content; }
        }

        public void ssscasccs()
        {
            
        }
        // prijatÈ data
        public string data="";
        // Konötruktor
        public Engine (Game parent, bool p·d,string ip){
             //wp·d= this.p·d;
            this.p·d = false;
            Connection = new Connection(this,ip);
            this.parent = parent;
            this.graphics = parent.GraphicsDevice;
            this.spriteBatch = new SpriteBatch(graphics);
            this.content = parent.Content;
            Screens = new List<GameScreen>();
            fInput = new Input(this);
            maly = Content.Load<SpriteFont>("maly");
            gulky = new List<Ray>();
            

            //kkk = new List<testKolizii>();
            
            this.load();
        }
        public bool aktalizovanieNepratelov=false;
        
        //hernÈ okno
        public List<GameScreen> Screens;
        //vloûenie
        public void PushGameScreen(GameScreen okno)
        {
            if (okno.Engine == null && !Screens.Contains(okno))
            {
                Screens.Add(okno);
                okno.engine = this;
                okno.LoadGameScreen();
                
            }

        }
        //vyzdvihnutie
        public GameScreen PopGameScreen()
        {
            if (Screens.Count == 0) return null;

            GameScreen ret = Screens[Screens.Count - 1];
            Screens.Remove(ret);
            ret.engine = null;
            return ret;
        }
        //public List<testKolizii> kkk;
        public List<Ray> gulky;
        public List<BoundingBox> enemy;
        public List <BoundingBox> stenyT;
        public List<BoundingBox> stenyF;

        /*public struct WorldObject
        {
            public Vector3 position;
            public Vector3 velocity;
            public Model model;
            public Texture2D texture2D;
            public Vector3 lastPosition;
            public void MoveForward()
            {
                lastPosition = position;
                position += velocity;
            }
            public void Backup()
            {
                position -= velocity;
            }
            public void ReverseVelocity()
            {
                velocity.X = -velocity.X;
            }
        }
        public  void CheckForCollisions(ref WorldObject c1, ref WorldObject c2)
        {
            for (int i = 0; i < c1.model.Meshes.Count; i++)
            {
                // Check whether the bounding boxes of the two cubes intersect.
                BoundingSphere c1BoundingSphere = c1.model.Meshes[i].BoundingSphere;
                c1BoundingSphere.Center += c1.position;

                for (int j = 0; j < c2.model.Meshes.Count; j++)
                {
                    BoundingSphere c2BoundingSphere = c2.model.Meshes[j].BoundingSphere;
                    c2BoundingSphere.Center += c2.position;

                    if (c1BoundingSphere.Intersects(c2BoundingSphere))
                    {
                        c2.ReverseVelocity();
                        c1.Backup();
                        c1.ReverseVelocity();
                        return;
                    }
                }
            }
        }*/
    }
}
