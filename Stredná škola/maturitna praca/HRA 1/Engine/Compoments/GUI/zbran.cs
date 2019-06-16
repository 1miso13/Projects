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
    class zbran: Model3D
    {
        public zbran(Vector3 pozice, Matrix rotace, Vector3 meritko, string model)
            :base(pozice, rotace, meritko, model)
        {
           // Parent.engine.zbran1 = &this;
        }
        Vector3 pozicia;
 //       Matrix rotacia;
        float pitch, Yaw;
        public void zbrand(Vector3 pozice, Matrix rotace, float pitch , float yaw)
        {
            this.pozicia = pozice;
            base.Pozice = pozice;
            base.Rotace = rotace;
            this.pitch = pitch;
            this.Yaw = yaw;
            
        }
      //  public override void Update()
      //  {
      //     Pozice = /*new Vector3((float)Math.Sin((double)Parent.engine.fYaw) * (float)Math.Cos((double)Parent.engine.fPitch), (float)Math.Sin((double)Parent.engine.fPitch) * 1f, ((float)Math.Cos((double)Parent.engine.fYaw)) * (float)Math.Cos((double)Parent.engine.fPitch))*/   pozicia + /*+new Vector3(2,0,0),Parent.engine.fYaw*/ new Vector3(-(float)Math.Sin((double)Yaw * 2) + 2, 1, -(float)Math.Cos((double)Yaw * 2) + 2);
        //    base.fRotace =/* Matrix.CreateFromAxisAngle(new Vector3(2, 0, 0), Yaw);*/ Matrix.CreateFromYawPitchRoll(Yaw, pitch + -MathHelper.PiOver2, -MathHelper.PiOver2);
          //  base.Update();
      //  }
    }
}
