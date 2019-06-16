using System;
using System.Collections.Generic;
using System.Linq;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.GamerServices;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Media;


namespace MojEngine
{
    /// <summary>
    /// This is a game component that implements IUpdateable.
    /// </summary>
    /// 
    
    public class GUI : Component
    {
        GameTime gameTime;
        public GUI()
           
        {
            // TODO: Construct any child components here
        }
            
        /// <summary>
        /// Allows the game component to perform any initialization it needs to before starting
        /// to run.  This is where it can query for any required services and load content.
        /// </summary>
        
        
        /// <summary>
        /// Allows the game component to update itself.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        
        Texture2D healthBar;
        Texture2D healthBar_Panel;
        Texture2D scope;
        Texture2D naboj;
        Vector2 poziciaHealthBar;
        public SpriteFont Font;
        Vector2 pozicie;
        public SoundEffect zvukvystrelu, zvukvybitej;
        Texture2D zbran;
        //System.Timers.Timer Timer = new System.Timers.Timer(1000);

        //Timer timeReload;

        protected override void Load()
        {
            zbran = Parent.Engine.Content.Load<Texture2D>("GuI/zbran");
            Font = Parent.Engine.Content.Load<SpriteFont>("maly");
            poziciaHealthBar = new Vector2(1/*Parent.Engine.GraphicsDevice.DisplayMode.Width/2*/, Parent.Engine.rozlíšenie.Y/* .GraphicsDevice.DisplayMode.Height*/-80);
            healthBar = Parent.Engine.Content.Load<Texture2D>("GuI/Healthbar/Health_bar_panel");
            healthBar_Panel = Parent.Engine.Content.Load<Texture2D>("GuI/Healthbar/HealthBar_HealthPoint");
            scope = Parent.Engine.Content.Load<Texture2D>("Gui/Scope/Scope2");
            healthPoint_percent = 100;
            
            zvukvystrelu = Parent.engine.Content.Load<SoundEffect>("strelba");
            zvukvybitej = Parent.engine.Content.Load<SoundEffect>("zvuk");

            pocetNabojov = maxPocetNabojov;
            pozicie = new Vector2(maxPocetNabojov, pocetNabojov);
            naboj = Parent.engine.Content.Load<Texture2D>(@"GUI/naboj");


            Parent.engine.gulky = new List<Ray>();
            //Parent.AddComponent(new Enemy(Parent.Kamera.Pozice,Matrix.CreateScale(1f)*Matrix.CreateRotationZ(1.5f), "untitled"));
           // Parent.AddComponent(new zbran(Parent.Kamera.Pozice,Matrix.CreateRotationX(-MathHelper.PiOver2),new Vector3(1f,1f,1f),"untitled"));
 


            //timeReload = null;
            //timeReload = new Timer();
            //Parent.AddComponent(new Timer());
            
            base.Load();
        }


        int hodnota= 5;
        Rectangle healthPoint = new Rectangle(0,0,30,80);
        void zmensiHP(int hodnota)
        {
            healthPoint_percent -= hodnota;
        }
        void zvecsiHP(int hodnota)
        {
            healthPoint_percent += hodnota;
        }
        int healthPoint_percent;
        TimeSpan gt;
        int test=0;
        List<Model3D> ostatny = new List<Model3D>();
        public override void Update()
        {
#region
            for (int i = 0; i < ostatny.Count; i++)
            {

                Parent.RemoveComponent(ostatny[i]);
            }
            ostatny.Clear();
            for (int i = 0; i < Parent.engine.ostatnyHraci.Count; i++)
            {
                ostatny.Add(Parent.engine.ostatnyHraci[i]);
            }
            
            
            for(int i = 0;i< Parent.engine.ostatnyHraci.Count;i++)
            {

                Parent.AddComponent(ostatny[i]);
            }



#endregion
            gameTime = Parent.engine.GameTime;



            test++;
            healthPoint.Width = 40 + healthPoint_percent * 2;
            if (Parent.Engine.Input.DrzenaKlavesa(Keys.NumPad8))
            {
                if (healthPoint_percent < 100)
                    zvecsiHP(hodnota);
                else
                    healthPoint_percent = 100;
            }
            if (Parent.Engine.Input.DrzenaKlavesa(Keys.NumPad2))
            {
                if (healthPoint_percent > 0)
                    zmensiHP(hodnota);
                else
                    healthPoint_percent = 0;
            }
            
            Parent.engine.health = healthPoint_percent;
            if (healthPoint_percent == 0)
                healthPoint_percent = 100;
            if(zapnuteNabijanie)
            gt += Parent.engine.GameTime.ElapsedGameTime;
            /*
            maxHP 240
            minHP 40*/
            if (Parent.engine.Input.StisknutaKlavesa(Keys.R)) 
                if(pocetNabojov!=maxPocetNabojov)
                    zapnuteNabijanie = true;
            
            if (gt.Seconds >= 2 /*timeReload.Time.Seconds = >2 /*new TimeSpan(0, 0, 5)*/)
            NabitieZbrane();


            if (Parent.engine.Input.StisknutaKlavesa(Keys.F1)) help = !help;
            
            if (Parent.Engine.Input.LeveTlacitkoStisknuto())
            {
                /*if (gt.TotalGameTime.Ticks > 1000
                    gt.ElapsedGameTime.)
                    gt.TotalGameTime.Add(new TimeSpan(0, 0, 2));*/
                if (pocetNabojov > 0 && !zapnuteNabijanie)
                {

                    zvukvystrelu.Play();

                    //gulka gulka= new gulka(new Vector3(20,50,20),0f,0f,0);
                    pocetNabojov--;
                    //Parent.AddComponent(new Model3D(/*new Vector3(0, 70, 0)*/Parent.Kamera.Pozice, (Matrix.CreateRotationX(Parent.engine.fPitch + 3.15f))*(Matrix.CreateRotationY((Parent.engine.fYaw))),new Vector3(1f), "projektil FBX"));
                    Parent.AddComponent(new gulka(Parent.Kamera.Pozice, Parent.engine.fYaw, Parent.engine.fPitch, 1f));
                    //Parent.engine.gulky.Add(gulka.boudingSphere);
                    //zvuk.zvukRada.Play();
                    //new gulka(Pozice,fYaw,fPitch,0f);
                }

                else
                {
                    //NabitieZbrane();
                    zvukvybitej.Play();
                }
            }
           // if (Parent.engine.Input.StisknutaKlavesa(Keys.F5))
           // Parent.RemoveComponent(Parent.engine.jbbjkkkkkkkkkkkkkb);

            base.Update();
        }
       // gulka gulka;
        protected bool help= false;
        bool zapnuteNabijanie=false;
        int maxPocetNabojov = 10;
        int pocetNabojov;
        void NabitieZbrane()
        {
            
            //timeReload.Start();
            /*if(timeReload.Running)
                   //timeReload.Start();
                    {*/
                        
                        {
                            zapnuteNabijanie = false;
                            gt = new TimeSpan(0,0,0);
                           // timeReload.Stop();
                            //timeReload.Reset();
                            pocetNabojov = maxPocetNabojov;
                        }
                   // }
                    /*else
                    timeReload.Start();*/

           
        }
        
        public override void Draw()
        {
            
            Parent.Engine.SpriteBatch.Begin();



 //           Parent.Engine.SpriteBatch.Draw(zbran,/* new Vector2(Parent.Engine.GraphicsDevice.PresentationParameters.BackBufferWidth / 2, Parent.Engine.GraphicsDevice.PresentationParameters.BackBufferHeight - Parent.Engine.GraphicsDevice.PresentationParameters.BackBufferHeight / 2),*/ new Rectangle(3*Parent.Engine.GraphicsDevice.PresentationParameters.BackBufferWidth / 7, /*Parent.Engine.GraphicsDevice.PresentationParameters.BackBufferHeight -*/ Parent.Engine.GraphicsDevice.PresentationParameters.BackBufferHeight / 2, Parent.Engine.GraphicsDevice.PresentationParameters.BackBufferWidth / 2, /*Parent.Engine.GraphicsDevice.PresentationParameters.BackBufferHeight -*/ Parent.Engine.GraphicsDevice.PresentationParameters.BackBufferHeight / 2/*test.Width,test.Height*/), Color.White);
            
            
            
            if (Parent.Engine.Input.PraveTlacitkoDrzeno()/* || Parent.engine.Input.DrzenaKlavesa(Keys.LeftAlt) || Parent.engine.Input.DrzenaKlavesa(Keys.RightAlt)*/)
            {
                Parent.Engine.SpriteBatch.Draw(scope, new Vector2(( /*Parent.engine.oknoWidth*/ Parent.Engine.rozlíšenie.X/*GraphicsDevice.PresentationParameters.BackBufferWidth /*.DisplayMode.Width /*Parent.Engine.GraphicsDevice.DisplayMode.Width*//*Parent.Engine.GraphicsDevice.Adapter.CurrentDisplayMode.Width*/ / 2) - 99, (/*Parent.Engine.oknoHeight*/ Parent.Engine.rozlíšenie.Y/*GraphicsDevice.PresentationParameters.BackBufferHeight/* .DisplayMode.Height /*Parent.Engine.GraphicsDevice.Adapter.CurrentDisplayMode.Height*/ / 2) - 98), Color.White);
            }
            //HealthBar
            Parent.Engine.SpriteBatch.Draw(healthBar, poziciaHealthBar/* new Vector2(50,100)*/, Color.White);
            Parent.Engine.SpriteBatch.Draw(healthBar_Panel, poziciaHealthBar,healthPoint, Color.White);

            //HP èíslo
            /*Sprite text = new Sprite("maly",new Vector2 (Parent.Engine.GraphicsDevice.DisplayMode.Width,Parent.Engine.GraphicsDevice.DisplayMode.Width));*/
            Parent.Engine.SpriteBatch.DrawString(Font, healthPoint_percent.ToString() , poziciaHealthBar+ new Vector2(120,25) /*new Vector2(Parent.Engine.GraphicsDevice.DisplayMode.Width/2, Parent.Engine.GraphicsDevice.DisplayMode.Height/2)*/, Color.Black);


            





            //vypísanie pozície
            if (help)
            {
                Parent.Engine.SpriteBatch.DrawString(Font, "Yaw:" + Parent.Engine.fYaw.ToString(), new Vector2(1, 20), Color.Black);
                Parent.Engine.SpriteBatch.DrawString(Font, "Pitch:" + Parent.Engine.fPitch.ToString(), new Vector2(1, 1), Color.Black);
                Parent.Engine.SpriteBatch.DrawString(Font, "X:" + Parent.Engine.pozice.X.ToString(), new Vector2(1, 40), Color.Black);
                Parent.Engine.SpriteBatch.DrawString(Font, "Y:" + Parent.Engine.pozice.Y.ToString(), new Vector2(1, 60), Color.Black);
                Parent.Engine.SpriteBatch.DrawString(Font, "Z:" + Parent.Engine.pozice.Z.ToString(), new Vector2(1, 80), Color.Black);
                Parent.Engine.SpriteBatch.DrawString(Font, "(sin)yaw:" + Math.Sin((double)Parent.Engine.fYaw).ToString(), new Vector2(1, 100), Color.Black);
                Parent.Engine.SpriteBatch.DrawString(Font, "(cos)yaw:" + Math.Cos((double)Parent.Engine.fYaw).ToString(), new Vector2(1, 120), Color.Black);
                Parent.Engine.SpriteBatch.DrawString(Font, "(sin)pitch:" + Math.Sin((double)Parent.Engine.fPitch).ToString(), new Vector2(1, 140), Color.Black);
                Parent.Engine.SpriteBatch.DrawString(Font,"Poèet nábojov:" + pocetNabojov.ToString(),new Vector2(1,200),Color.Black);
                Parent.Engine.SpriteBatch.DrawString(Font, "gametime:" + Math.Sin((double)Parent.Engine.GameTime.ElapsedGameTime.Milliseconds).ToString(), new Vector2(1, 160), Color.Black);
                Parent.engine.SpriteBatch.DrawString(Font, /*timeReload.Time.*/gt.ToString(@"mm\:ss\.ff"), new Vector2(1, 250), Color.Black);
                Parent.engine.SpriteBatch.DrawString(Font, Parent.engine.pozicel.ToString(), new Vector2(1, 270), Color.Black);
                Parent.engine.SpriteBatch.DrawString(Font, Parent.engine.gulky.Count.ToString(), new Vector2(1, 300), Color.Black);
                Parent.engine.SpriteBatch.DrawString(Font, Parent.engine.enemy.Count.ToString(), new Vector2(1, 320), Color.Black);
                Parent.engine.SpriteBatch.DrawString(Font, Parent.engine.health.ToString(), new Vector2(1, 340), Color.Black);
                Parent.engine.SpriteBatch.DrawString(Font, Parent.engine.jkjbkjb.ToString(), new Vector2(1, 360), Color.Black);
                Parent.engine.SpriteBatch.DrawString(Font, Parent.engine.jkjbkjbd.ToString(), new Vector2(1, 380), Color.Black);
                Parent.engine.SpriteBatch.DrawString(Font, MathHelper.PiOver4.ToString(), new Vector2(1, 410), Color.Red);
                Parent.engine.SpriteBatch.DrawString(Font, Parent.engine.kolizia_S_Objektom_index.ToString(), new Vector2(1, 400), Color.Black);
                Parent.engine.SpriteBatch.DrawString(Font, Parent.engine.rychlostpadania.ToString(), new Vector2(1, 450), Color.Green);
                Parent.engine.SpriteBatch.DrawString(Font, Parent.engine.vyskaodpodlahy.ToString(), new Vector2(1, 470), Color.Green);
                try
                {
                    Parent.engine.SpriteBatch.DrawString(Font, "FPS: " + (1000 / gameTime.ElapsedGameTime.Milliseconds), new Vector2(Parent.Engine.rozlíšenie.X/*GraphicsDevice.PresentationParameters.BackBufferWidth*/ - 100, 1), Color.White);
                }
                catch
                { }
                try
                {
                    Parent.engine.SpriteBatch.DrawString(Font, Parent.engine.data, new Vector2(1, 500), Color.White);
                }
                catch
                {
                    Parent.engine.SpriteBatch.DrawString(Font, "chyba dát ", new Vector2(Parent.Engine.rozlíšenie.X/*.GraphicsDevice.PresentationParameters.BackBufferWidth*/ - 100, 100), Color.White);
                }
                Parent.engine.SpriteBatch.DrawString(Font, Parent.engine.Connection.IPAddres.ToString(), new Vector2(Parent.Engine.rozlíšenie.X/*GraphicsDevice.PresentationParameters.BackBufferWidth */- 150, 20), Color.White);
            }
            //text nabijania
            if (pocetNabojov == 0 && !zapnuteNabijanie)
            {
                Parent.engine.SpriteBatch.DrawString(Font, "Stlaète R na nabitie zbrane", new Vector2(Parent.engine.rozlíšenie.X / 2 - 150, Parent.engine.rozlíšenie.Y / 4), Color.Red);
            }
            else
            {
                if(zapnuteNabijanie)
                    Parent.engine.SpriteBatch.DrawString(Font, "Nabíjanie", new Vector2(Parent.engine.rozlíšenie.X/*GraphicsDevice.DisplayMode.Width*/ / 2 - 75, Parent.engine.rozlíšenie.Y/*GraphicsDevice.DisplayMode.Height*/ / 4), Color.Red);
            }

            //naboje
            
            for (int w = (int)Parent.engine.rozlíšenie.X/*.GraphicsDevice.DisplayMode.Width*/ - 50, i=0; i < pocetNabojov;w-=20, i++)
            {
                Parent.engine.SpriteBatch.Draw(naboj, new Rectangle(w, (int)Parent.engine.rozlíšenie.Y/*.GraphicsDevice.DisplayMode.Height */- 100, 20, 70), Color.White);
            }



            Parent.Engine.SpriteBatch.End();
            Parent.Engine.GraphicsDevice.SamplerStates[0] = SamplerState.LinearWrap;
            Parent.Engine.GraphicsDevice.DepthStencilState = DepthStencilState.Default;
            Parent.Engine.GraphicsDevice.BlendState = BlendState.Opaque;
            
            base.Draw();
        }
        public override void Draw(Matrix View, Matrix Projection, Vector3 CameraPosition)
        {
            Parent.Kamera.Draw(View, Projection, CameraPosition);
            base.Draw(View, Projection, CameraPosition);
        }
    }

}
