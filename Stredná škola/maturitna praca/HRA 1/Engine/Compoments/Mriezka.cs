using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework;

namespace MojEngine
{
    public class Mriezka : Component
    {
        VertexPositionColor[] body;
        int vzdalenost;
        int pocet;

        BasicEffect effect;

        public Mriezka(int pocet, int vzdalenost)
        {
            this.vzdalenost = vzdalenost;
            this.pocet = pocet;

            body = new VertexPositionColor[pocet * 4];
        }

        protected override void Load()
        {
            effect = new BasicEffect(Parent.Engine.GraphicsDevice);
            effect.VertexColorEnabled = true;

            Color Color = Color.Black;
            int k = 0;
            for (int i = 0; i < pocet; i++)
            {
                Vector3 tmp = Vector3.Left * vzdalenost * i;
                if (pocet / 2 == i)
                {
                    body[k] = new VertexPositionColor(tmp, Color.Red);
                    body[k + 1] = new VertexPositionColor(tmp + Vector3.Forward * (vzdalenost * (pocet - 1)), Color.Red);
                    tmp = Vector3.Forward * vzdalenost * i;
                    body[k + 2] = new VertexPositionColor(tmp, Color.Green);
                    body[k + 3] = new VertexPositionColor(tmp + Vector3.Left * (vzdalenost * (pocet - 1)), Color.Green);
                }
                else
                {
                    body[k] = new VertexPositionColor(tmp, Color);
                    body[k + 1] = new VertexPositionColor(tmp + Vector3.Forward * (vzdalenost * (pocet - 1)), Color);
                    tmp = Vector3.Forward * vzdalenost * i;
                    body[k + 2] = new VertexPositionColor(tmp, Color);
                    body[k + 3] = new VertexPositionColor(tmp + Vector3.Left * (vzdalenost * (pocet - 1)), Color);
                }

                k += 4;
            }
        }

        public override void Draw(Matrix View, Matrix Projection, Vector3 CameraPosition)
        {
            effect.View = View;
            effect.Projection = Projection;
            effect.World = Matrix.CreateTranslation(new Vector3(pocet * (vzdalenost / 2), 0, pocet * (vzdalenost / 2)));

            effect.CurrentTechnique.Passes[0].Apply();
            Parent.Engine.GraphicsDevice.DrawUserPrimitives<VertexPositionColor>(PrimitiveType.LineList, body, 0, body.Length / 2);
        }
    }
}
