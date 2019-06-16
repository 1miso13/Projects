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
using MojEngine;
using System.Reflection;
using System.IO;
using System.Net;
using System.Net.Sockets;
namespace tvorba_3D
{
    
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        public bool cantConnect=false;
        private Settings1 setting;
        public Settings1 seting
        {
            get { return setting; }
            set { setting = value; }
        }
        GraphicsDeviceManager graphics;
        //public Engine engine;
        public List<Menu> menu;
        public SpriteBatch SpriteBatch;
        //public FPS fps;
        public Game1()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";
        }
        protected Vector2 roz;
        public Vector2 rozlišenie
        {
            get{return roz;}
        }
        protected override void Initialize()
        {
            setting = new Settings1();
            
            // set resolution (0 - maxDisplayResolution)
            if ( setting.resolution_Y == 0 || setting.resolution_Y==0)
            {
                
                setting.resolution_Y=GraphicsDevice.DisplayMode.Height;
                setting.resolution_X=GraphicsDevice.DisplayMode.Width;
            
            }
            graphics.PreferredBackBufferHeight = /*GraphicsDevice.DisplayMode.Height*/setting.resolution_Y; //750;
            graphics.PreferredBackBufferWidth = /*GraphicsDevice.DisplayMode.Width*/setting.resolution_X; //1366;
            roz = new Vector2(setting.resolution_X, setting.resolution_Y);
            graphics.IsFullScreen = setting.fullScreen;//true;
            graphics.PreferMultiSampling = true;
            IsMouseVisible = true;
            //graphics.ToggleFullScreen();
            graphics.ApplyChanges();

            
            //fps = new FPS(this);
            base.Initialize();

            

        }

        public void Load()
        {
            Hra.engine.renderBoudingObject = setting.vykreslovaniekolizii;

        }
        /*public void ZapniHru()
        {
            zapnutahra = true;
            engine = new Engine(this);
            engine.PushGameScreen(new MojeHerniOkno());
        }*/
        public void vypniMenu()
        {
            menu.Remove(Menu);
        }
        public void vypniHru()
        {
            this.Exit();
        }
       public  Menu Menu;
       public HRA Hra;
       public SpriteFont Text;
        protected override void LoadContent()
        {
            
            Text = Content.Load<SpriteFont>("maly");
            SpriteBatch = new SpriteBatch(GraphicsDevice);
            Hra = new HRA(this);
            Menu = new Menu(this);
            //menu.Add(Menu);
            
            //engine = new Engine(this);
            Components.Add(Menu);
            Components.Add(Hra);
            /*
            GameScreen okno = new GameScreen("okenko");
            Engine.PushGameScreen(okno);
            okno.AddComponent(new BarevnePozadi(Color.Orange));*/

            //engine.PushGameScreen(new MojeHerniOkno());
        }


        protected override void UnloadContent()
        {

        }
        public bool zapnuta = false;
        public KeyboardState klavesnica, klavesnicaold;
        protected override void Update(GameTime gameTime)
        {
            
            if(!Hra.zapnutahra)
                if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed || klavesnicaold.IsKeyUp(Keys.Escape) && klavesnica.IsKeyDown(Keys.Escape))
                vypniHru();
            //if (zapnutahra)
            //engine.Update(gameTime);


            if (klavesnicaold.IsKeyUp(Keys.F2) && klavesnica.IsKeyDown(Keys.F2))
                graphics.ToggleFullScreen();
            
            klavesnicaold = klavesnica;
            klavesnica = Keyboard.GetState();
            /*if(!zapnutahra)
            if (klavesnicaold.IsKeyUp(Keys.F3) && klavesnica.IsKeyDown(Keys.F3))
                ZapniHru();*/
            //if (IsActive())
            /*if(zapnutahra)
                engine.aktivne = IsActive;*/
            if (!zapnuta)
                if(!Components.Contains(Hra))
                    Components.Add(Hra);
            
            base.Update(gameTime);

        }


        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.CornflowerBlue);
            /*if(zapnutahra)
            engine.Draw(gameTime);*/
            
            base.Draw(gameTime);
        }
    }
}
