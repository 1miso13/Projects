using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Content;

namespace MojEngine
{
    public class GameScreen
    {
        public virtual void Update()
        {
            updated.Clear();
            foreach (Component c in fComponents)
            {
                updated.Add(c);
            }

            foreach (Component c in updated)
            {
                c.Update();
            }
            if (Kamera != null)
                Kamera.Update();

        }

        public virtual void Draw()
        {
            foreach (Component c in fComponents)
            {
                if (c.Visible) c.Draw();
            }
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
                if (!String.IsNullOrEmpty(value))
                {
                    fName = value;
                }
                else
                {
                    if (fName == null) fName = "Herne okno";
                }
            }
        }

        public GameScreen(string jmeno)
        {
            Name = jmeno;
            fComponents = new List<Component>();
            updated = new List<Component>();
        }
        private List<Component> updated;
        private List<Component> fComponents;
        public List<Component> Components
        {
            get
            {
                return fComponents;
            }
        }
        
        public void AddComponent(Component c)
        {
            if (c != null && !Components.Contains(c))
            {
                Components.Add(c);
                c.Parent = this;
                c.LoadComponent();
            }
        }

        public void RemoveComponent(Component c)
        {
            if (c != null && Components.Contains(c))
            {
                fComponents.Remove(c);
                c.Parent = null;
            }
        }


        bool loaded;

        public void LoadGameScreen()
        {
            if (loaded) return;
            loaded = true;
            // sem muzeme pozdeji pridat ncitani treba systemu svetel a dalsich veci ktere jsou pro
            // vsechna okna spolecne a musi byt vzdy nahrany
            
            Load();
        }

        protected virtual void Load()
        {
            // kdezto sem dame veci specificke pro kazde dalsi zdedene okno,
            //pridavani komponent a jine
        }

        public bool Loaded
        {
            get
            {
                return loaded;
            }
        }

        internal Engine engine;
        public Engine Engine
        {
            get
            {
                return engine;
            }
        }

        public Kamera Kamera
        {
            get;
            set;
        }







    }
}
