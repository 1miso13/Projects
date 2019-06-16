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
namespace tvorba_3D
{
    class ingameMenu :Microsoft.Xna.Framework.DrawableGameComponent
    {


        Game1 game;
        private List<menuButton> polozky;
        public ingameMenu(Game1 game)
            : base(game)
        {
            polozky = new List<menuButton>();
            this.game = game;
            Texture = Game.Content.Load<Texture2D>(@"menu\Rounded_Button");
            Load();
        }
        protected Texture2D Texture;
        protected  void Load()
        {
            pozice = new Vector2(game.GraphicsDevice.PresentationParameters.BackBufferWidth / 3, game.GraphicsDevice.PresentationParameters.BackBufferHeight/3);
            
            PridajPolozku("Continue");
            PridajPolozku("TEST");
            PridajPolozku("Settings");
            PridajPolozku("Quit");
        }
        public bool tttt = true;
        public override void Update(GameTime gameTime)
        {
            if (tttt)
            if (game.Hra.zapnutahra)
            {
            
                
            

                if (game.klavesnicaold.IsKeyUp(Keys.Enter) && game.klavesnica.IsKeyDown(Keys.Enter))
                    menu();
                    


                if (game.klavesnicaold.IsKeyUp(Keys.Down) && game.klavesnica.IsKeyDown(Keys.Down))
                    VyberDalsi();
                if (game.klavesnicaold.IsKeyUp(Keys.Up) && game.klavesnica.IsKeyDown(Keys.Up))
                    VyberPredchozi();

               // myš = Mouse.GetState() ;
                //klik();
            }
            base.Update(gameTime);
        }
       // MouseState myš;
        public menuButton vybranaPolozka;
        Vector2 pozice;//= new Vector2(game.GraphicsDevice.PresentationParameters.BackBufferWidth/3, 1);
        int vyska = 60;
        public void PridajPolozku(string text)
        {
             //nastavení pozice podla poradia položky
            Vector2 p = new Vector2(pozice.X, pozice.Y + polozky.Count * vyska);
            menuButton polozka = new menuButton(text, p);
            polozky.Add(polozka);
             //výber prvej položky
            if (vybranaPolozka == null)
                vybranaPolozka = polozka;
            polozka.Rectangle = new Rectangle((int)polozka.pozicia.X, (int)polozka.pozicia.Y + polozky.Count, (int)velkostX, (int)velkostY);
        }
        
        Color nevybranaBarva = Color.Cyan, vybranaBarva = Color.White;
        int velkostX = 150, velkostY= 50;
        public bool starting = false, started= false;
        public override void Draw(GameTime gameTime)
        {
            game.SpriteBatch.Begin();
            
            if (tttt)
            if (game.Hra.zapnutahra)
            {

                foreach (menuButton polozka in polozky)
                {
                    Color barva = nevybranaBarva;
                    if (polozka == vybranaPolozka)
                        barva = vybranaBarva;

                    game.SpriteBatch.Draw(this.Texture, new Rectangle((int)polozka.pozicia.X, (int)polozka.pozicia.Y, velkostX, velkostY), barva*0.8f);
                    game.SpriteBatch.DrawString(game.Text, polozka.text, polozka.pozicia + new Vector2(velkostX / 4, velkostY / 4), Color.White);

                }
                game.SpriteBatch.DrawString(game.Text, polozky.IndexOf(vybranaPolozka).ToString()/* polozky.Count.ToString()*/, new Vector2(6 * game.GraphicsDevice.PresentationParameters.BackBufferWidth / 7, 50), Color.Black);
                
                
                
            }game.SpriteBatch.End();
                game.GraphicsDevice.SamplerStates[0] = SamplerState.LinearWrap;
                game.GraphicsDevice.DepthStencilState = DepthStencilState.Default;
                game.GraphicsDevice.BlendState = BlendState.Opaque;
            base.Draw(gameTime);
        }
        public void VyberDalsi()
        {
            int index = polozky.IndexOf(vybranaPolozka);
            if (index < polozky.Count - 1)
                vybranaPolozka = polozky[index + 1];
            else
                vybranaPolozka = polozky[0];
        }

         //<summary>
         //Vybere předchozí položku
         //</summary>
        public void VyberPredchozi()
        {
            int index = polozky.IndexOf(vybranaPolozka);
            if (index > 0)
                vybranaPolozka = polozky[index - 1];
            else
                vybranaPolozka = polozky[polozky.Count - 1];
        }
      /*  void klik()
        {
            foreach(menuButton button in polozky)
            {
                if (button.Rectangle.Contains(new Point(myš.X, myš.Y)))
                {
                    int i = polozky.IndexOf(button);
                    vybranaPolozka = polozky[i];
                    if(myš.LeftButton == ButtonState.Pressed)
                    {
                        menu();
                    }
                }
            }
        }*/
        void menu()
        {
            switch (/*polozky.IndexOf(vybranaPolozka) + 1*/ vybranaPolozka.text)
            {
                case "Continue"/*1*/:
                    //starting = true;
                    tttt = false;
                    //game.Hra.ZapniHru();
                    break;
                case /*2*/"Settings":

                    break;
                case /*3*/"Quit":
                    game.Hra.vypniHru();
                    //game.Exit();
                    break;
                default:
                    
                    break;
            }
        }

    }
}