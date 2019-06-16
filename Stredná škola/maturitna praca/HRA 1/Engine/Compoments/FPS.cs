/*using System;
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
    public class FPS :Component
    {
        Game game;
        public FPS(Game game)
            
        {

            this.game=game;
        }
        SpriteFont _spr_font;
        int _total_frames = 0;
        float _elapsed_time = 0.0f;
        int _fps = 0;
        protected override void Load()
        {_spr_font = game.Content.Load<SpriteFont>("maly");
            base.Load();
        }
        
         protected override void Update(GameTime gameTime)
        {
            gameTime = Parent.engine.GameTime;
            // Update
            _elapsed_time += (float)gameTime.ElapsedGameTime.TotalMilliseconds;
 
            // 1 Second has passed
            if (_elapsed_time && gt == 1000.0f)
            {
                _fps = _total_frames;
                _total_frames = 0;
                _elapsed_time = 0;
            }
            base.Update();
        }GameTime gameTime;
        protected override void Draw()
        {
            // Only update total frames when drawing
            _total_frames++;
            
            Parent.engine.SpriteBatch.Begin();
            Parent.engine.SpriteBatch.DrawString(_spr_font, string.Format("FPS={0}", _fps),new Vector2(10.0f, 20.0f), Color.White);
            Parent.engine.SpriteBatch.End();
 
            base.Draw();
        }

    }
}
*/