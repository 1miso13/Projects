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
    public class skybox :Model3D
    {
        public skybox(Vector3 position)
            : base(new Vector3(50, 70, 0), Matrix.CreateScale(1000f), "Skybox/skybox")
        {

            Pozice = position;
            
        }


        //public Vector3 position;
        public override void Update()
        {
            Pozice= Parent.engine.pozice;
            //position 
            base.Update();
        }
    
    }
}
