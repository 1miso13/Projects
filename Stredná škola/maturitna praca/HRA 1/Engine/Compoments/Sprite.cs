using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
namespace MojEngine
{
    public class Sprite :Component
    {
        private string textureName;

        private Vector2 fPosition;

        public Vector2 Position
        {
            get
            {
                return fPosition;
            }
            set
            {
                fPosition = value;
            }
        }

        public Sprite(string textureName, Vector2 position){
            this.textureName = textureName;
            Position = position;
        }

        public Rectangle DestinationRectangle { get; set; }

        public Sprite(string textureName, Vector2 position, int width, int height)
            : this(textureName, position)
        {
            DestinationRectangle = new Rectangle((int)position.X, (int)position.Y, width, height);
        }

        protected Texture2D Texture;

        protected override void Load(){
            Texture = Parent.Engine.Content.Load<Texture2D>(textureName);

            if (DestinationRectangle.Width == 0 || DestinationRectangle.Height == 0)
                DestinationRectangle = new Rectangle((int)Position.X, (int)Position.Y, Texture.Width, Texture.Height);
            DestinationRectangle = new Rectangle((int)Position.X, (int)Position.Y, DestinationRectangle.Width, DestinationRectangle.Height);

        }

        public override void Draw()
        {
            Parent.Engine.SpriteBatch.Begin();
            Parent.engine.SpriteBatch.Draw(Texture, DestinationRectangle, Color.White);
            Parent.Engine.SpriteBatch.End();

            Parent.Engine.GraphicsDevice.SamplerStates[0] = SamplerState.LinearWrap;
            Parent.Engine.GraphicsDevice.DepthStencilState = DepthStencilState.Default;
            Parent.Engine.GraphicsDevice.BlendState = BlendState.Opaque;

        }
        

    }
}
