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
    public class TargetKamera :Kamera
    {
        private Vector3 fTarget;

        public Vector3 Target
        {
            get{
                return fTarget;
            }
            set
            {
                fTarget = value;
            }
        }

        public TargetKamera(GameScreen okno, Vector3 pozicia, Vector3 target)
            : base(okno)
        {
            Pozice = pozicia;
            Target = target;
        }
        public override void Update()
        {
            this.View = Matrix.CreateLookAt(Pozice, Target, Vector3.Up);
        }
        


    }
}
