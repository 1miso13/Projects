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
    public class BarevnePozadi : Component
    {
        public Color Barva
        {
            get;
            set;
        }

        public BarevnePozadi(Color barva)
        {
            Barva = barva;
        }

        public override void Draw()
        {
            Parent.Engine.GraphicsDevice.Clear(Barva);
        }


    }
}
