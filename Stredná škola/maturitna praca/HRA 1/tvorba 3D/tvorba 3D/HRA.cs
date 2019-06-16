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
using System.Net;
using System.Net.Sockets;
namespace tvorba_3D
{
    /// <summary> 
    /// Class for Engine
    /// </summary>
    public class HRA : Microsoft.Xna.Framework.DrawableGameComponent
    {
        public Engine engine;
        Game1 game;
        public bool zapnutahra = false;
        public HRA(Game1 game)
            : base(game)
        {
            
            game.zapnuta = false;
            this.game=game;
            
            // TODO: Construct any child components here
        }
        public void vypniHru()
        {
            //if(p·d)
            engine.Connection.disconnect();
            zapnutahra = false;
            game.zapnuta = false;
            game.Menu.started = false;
            game.Components.Remove(this);
        }
        public bool p·d=false;
        public void ZapniHru()
        {
            game.zapnuta = true;
            zapnutahra = true;
            engine = new Engine(game, p·d,game.seting.IP_Adress);
            engine.rozlÌöenie = game.rozliöenie;
            engine.IPadress= game.seting.IP_Adress;
            p·d = engine.p·d;
            if (p·d)
            { game.cantConnect = true; vypniHru(); zapnutahra = false; }
            else{
                game.cantConnect = false;
                
            engine.PushGameScreen(new MojeHerniOkno());
            engine.Connection.IPAddres = IPAddress.Parse(game.seting.IP_Adress);
            engine.renderBoudingObject = game.seting.vykreslovaniekolizii;
           // int i = 0;
            }
            
        }
        /// <summary>
        /// Allows the game component to perform any initialization it needs to before starting
        /// to run.  This is where it can query for any required services and load content.
        /// </summary>
        public override void Initialize()
        {
            // TODO: Add your initialization code here
            engine.renderBoudingObject = game.seting.vykreslovaniekolizii;
            game.Load();
            base.Initialize();
        }

        /// <summary>
        /// Allows the game component to update itself.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        public override void Update(GameTime gameTime)
        {
            game.IsMouseVisible = !zapnutahra;
            if (zapnutahra)
            // TODO: Add your update code here
            {
                engine.Update(gameTime);
                base.Update(gameTime);
                engine.aktivne = game.IsActive;
                
                if (engine.Input.DrzenaKlavesa(Keys.Escape) )
                {
                    // treba spraviù menu
                    if (menu == null)
                    {
                        menu = new ingameMenu(game);
                        game.Components.Add(menu);
                    }
                    else
                    {
                        
                        game.Components.Remove(menu);
                        menu.tttt = true;
                        game.Components.Add(menu);
                    }

                    //vypniHru();
                    //game.Components.Remove(this);
                    
                }
                
                
            }
        }
        ingameMenu menu;
        public override void Draw(GameTime gameTime)
        {
            if(zapnutahra)
            
            engine.Draw(gameTime);
            base.Draw(gameTime);
        }
    }
}
