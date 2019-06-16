using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Content;

namespace MojEngine
{
    public class Component
    {

        public virtual void Update()
        {
            
        }


        public virtual void Draw()
        {
            if (Parent.Kamera != null)
            {
                Draw(Parent.Kamera.View, Parent.Kamera.Projection, Parent.Kamera.Pozice);
            }
        }
        public virtual void Draw(Matrix View, Matrix Projection, Vector3 CameraPosition)
        {

        }
        private string fName;

        public string Name
        {
            get
            {
                return fName;
            }
            set
            {
                fName = value;
            }
        }

        public bool Visible
        {
            get;
            set;
        }

        //Konštruktor
        public Component()
        {
            Visible = true;
        }

        private GameScreen fParent;

        public GameScreen Parent
        {
            get { return fParent; }
            set
            {
                if (fParent == value) return;
                if (fParent != null) fParent.RemoveComponent(this);
                fParent = value;
                if (value != null) fParent.AddComponent(this);
            }
        }

        bool loaded;

        public bool Loaded
        {
            get
            {
                return loaded;
            }
        }

        public void LoadComponent()
        {
            if (loaded) return;

            loaded = true;
            Load();

        }

        protected virtual void Load()
        {

        }

        




    }
}
