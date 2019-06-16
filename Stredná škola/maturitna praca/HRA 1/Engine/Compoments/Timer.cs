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

namespace MojEngine
{
    public class Timer : Component
    {
        public TimeSpan Time
        {
            get;
            private set;
        }


        public Timer()
        {
            Time = TimeSpan.Zero;
        }

        public bool Running
        {
            get;
            private set;
        }

        public virtual void Start()
        {
            Running = true;
        }

        public virtual void Stop()
        {
            Running = false;
        }

        public virtual void Reset()
        {
            Time = TimeSpan.Zero;
        }

        public override void Update()
        {
            if (Running) Time += Parent.Engine.GameTime.ElapsedGameTime;
        }
        public override void Draw(Microsoft.Xna.Framework.Matrix View, Microsoft.Xna.Framework.Matrix Projection, Microsoft.Xna.Framework.Vector3 CameraPosition)
        {
            Parent.Engine.SpriteBatch.Begin();
            //Parent.Engine.SpriteBatch.DrawString(Parent.engine.maly, Time.ToString(@"mm\:ss\.ff"), new Vector2(1,250), Color.Red);
            Parent.Engine.SpriteBatch.End();
        }
    }
}
