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

namespace MojEngine
{
    public class TextLabel :Component
    {
        private string fontName;

        public Vector2 Position{
            get;
            set;
        }

        public Color Color{
            get;
            set;
        }

        public string Text{
            get;
            set;
        }

        public TextLabel (string text,Vector2 position,string fontName)
            :this(text,position,Color.Black,fontName){
        }

        public TextLabel (string text, Vector2 position , Color color,string fontName)
        {
            Text = text;
            Position = position;
            this.fontName = fontName;
            Color = color;
        }

        protected SpriteFont SpriteFont;

        protected override void Load()
        {
            SpriteFont = Parent.Engine.Content.Load<SpriteFont>(fontName);
        }

        public override void Draw()
        {
            Parent.Engine.SpriteBatch.Begin();
            Parent.Engine.SpriteBatch.DrawString(SpriteFont, Text, Position, Color);
            Parent.Engine.SpriteBatch.End();


            Parent.Engine.GraphicsDevice.SamplerStates[0] = SamplerState.LinearWrap;
            Parent.Engine.GraphicsDevice.DepthStencilState = DepthStencilState.Default;
            Parent.Engine.GraphicsDevice.BlendState = BlendState.Opaque;
            base.Draw();
        }

    }
}
