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
    class FreeCamera :TargetKamera
    {
        float fYaw, fPitch;

        public float Yaw
        {
            get
            {
                return fYaw;
            }
            set
            {
                fYaw = value;
            }
        }

        public float Pitch
        {
            get
            {
                return fPitch;
            }
            set
            {
                fPitch = value; 
            }
        }

        public FreeCamera(GameScreen okno, Vector3 pozice, Vector3 target)
            :base(okno,pozice,target)
        {
            fYaw = 0;
            fPitch = 0;
        }
        //Vector3 Target;
        public override void Update()
        {
            fYaw -= Parent.Engine.Input.DeltaX*0.01f;
            fPitch -= Parent.Engine.Input.DeltaY * 0.01f;

            Matrix rotace = Matrix.CreateFromYawPitchRoll(fYaw,fPitch,0);

            
            Vector3 posun = Vector3.Zero;
            if (Parent.Engine.Input.DrzenaKlavesa(Keys.Up)|| Parent.Engine.Input.DrzenaKlavesa(Keys.W)) posun += Vector3.Forward;
            if (Parent.Engine.Input.DrzenaKlavesa(Keys.Down) || Parent.Engine.Input.DrzenaKlavesa(Keys.S)) posun += Vector3.Backward;
            if (Parent.Engine.Input.DrzenaKlavesa(Keys.Left) || Parent.Engine.Input.DrzenaKlavesa(Keys.A)) posun += Vector3.Left;
            if (Parent.Engine.Input.DrzenaKlavesa(Keys.Right) || Parent.Engine.Input.DrzenaKlavesa(Keys.D)) posun += Vector3.Right;

            posun *= 0.1f * (float)Parent.Engine.GameTime.ElapsedGameTime.TotalMilliseconds;
            Pozice += Vector3.Transform(posun, rotace);
            Target = Pozice + Vector3.Transform(Vector3.Forward,rotace);
            base.Update();
        }

        


    }
}
