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
    public class zvuk
    {
        Engine engine;
        public zvuk(Engine en)
        {
            engine = en;
            Load();
        }
        public SoundEffect zvukRada;
        protected void Load()
        {
            zvukRada = engine.Content.Load<SoundEffect>("strelba");
            //MediaPlayer.Play(zvukRada);
            
            
        }

    }
}
