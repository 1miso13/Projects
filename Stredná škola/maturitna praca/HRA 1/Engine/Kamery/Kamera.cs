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
    public abstract class Kamera
    {
        private Matrix fView;
        private Matrix fProjection;

        public Matrix View
        {
            get
            {
                return fView;
            }
            set
            {
                fView = value;
            }
        }

        public Matrix Projection
        {
            get
            {
                return fProjection;
            }
            set
            {
                fProjection = value;
            }
        }

        private Vector3 fPozice;
        public Vector3 Pozice
        {
            get
            {
                return fPozice;
            }
            set
            {
                fPozice = value;
            }
        }
        public float PoziceX
        {
            get
            {
                return fPozice.X;
            }
            set
            {
                fPozice.X = value;
            }
        }
        public float PoziceY
        {
            get
            {
                return fPozice.Y;
            }
            set
            {
                fPozice.Y = value;
            }
        }
        public float PoziceZ
        {
            get
            {
                return fPozice.Z;
            }
            set
            {
                fPozice.Z = value;
            }
        }
        private float fFarPlane;

        public float FarPlane
        {
            get { return fFarPlane; }
            set { fFarPlane = value;
            Projection = Matrix.CreatePerspectiveFieldOfView(MathHelper.PiOver2,Parent.Engine.GraphicsDevice.Viewport.AspectRatio,1f,FarPlane);}
        }

        protected GameScreen Parent;
        //public float piover = MathHelper.PiOver2;
        public Kamera(GameScreen okno)
        {
            Parent = okno;
            FarPlane = 100000f;
            
                Projection = Matrix.CreatePerspectiveFieldOfView(MathHelper.PiOver2, Parent.Engine.GraphicsDevice.Viewport.AspectRatio, 1f, FarPlane);
        }

        public virtual void Update()
        {
           

        }
        public virtual void Draw(Matrix View, Matrix Projection, Vector3 CameraPosition)
        {
            
        }
        
    }
}
