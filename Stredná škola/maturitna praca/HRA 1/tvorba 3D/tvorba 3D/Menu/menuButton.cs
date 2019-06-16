using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
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
    public class menuButton// : Microsoft.Xna.Framework.DrawableGameComponent
    {
        Game1 game;
        public Vector2 pozicia;
        public string text;
        Rectangle fRectangle;
        public menuButton(/*string text*/Game1 game)
            //: base(game)
        {
            this.game = game;
            //this.text = text;
        }
        public menuButton(string text,/* Game1 game,*/Vector2 pozicia)
            
            //:this(game)
        {
            
            this.text = text;
            this.pozicia = pozicia;
            //this.pozicia = pozicia;
            
        }
        public Rectangle Rectangle
        {
            get
            {
                return fRectangle;
            }
            set
            {
                fRectangle = value;
            }
        }
        

        

    }
}
