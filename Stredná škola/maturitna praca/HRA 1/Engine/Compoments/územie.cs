using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Design;
using Microsoft.Xna.Framework.GamerServices;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Media;
using Microsoft.Xna.Framework.Net;
using Microsoft.Xna.Framework.Storage;
namespace MojEngine
{
    class územie :Model3D
    {
        public územie(Vector3 pozicia,string model)
            :base(pozicia,model)
        {

        }
        //public BoundingBox d;
        protected override void Load()
        {

            Parent.engine.stenyF = new List<BoundingBox>();
            Parent.engine.stenyT = new List<BoundingBox>();


            //Parent.engine.stenyT.Add(new BoundingBox(new Vector3(3001, 0, 3001), new Vector3(-3001, 500, -3001)));

            //Parent.engine.ddd = new BoundingBox(new Vector3(3001, 0, 3001), new Vector3(-3001, 800, -3001));
            //Parent.engine.stenyT.Add(new BoundingBox(new Vector3(3000, 0, -3000), new Vector3(-3000, 800, 3000)));
            //Okraj-steny
            //X
            //Parent.engine.stenyF.Add(new BoundingBox(new Vector3(3000, 0, -3010), new Vector3(3010,10000, 3010)));
            //Z
            Parent.engine.stenyF.Add(new BoundingBox(new Vector3(-3010, 0, 3000), new Vector3(3010,9000, 3010)));
            //-Z
            Parent.engine.stenyF.Add(new BoundingBox(new Vector3(-3010, 0, -3010), new Vector3(3010, 7000, -3000)));
            //-X
            Parent.engine.stenyF.Add(new BoundingBox(new Vector3(-3010, 0, -3010), new Vector3(-3000, 8000, 3010)));

            //schody
            //float koniecSchodou = ;
            /*
            Parent.engine.stenyF.Add(new BoundingBox(new Vector3(-1200, 0, -1700), new Vector3(-koniecSchodou, 20, -1600)));
            Parent.engine.stenyF.Add(new BoundingBox(new Vector3(-1180, 0, -1700), new Vector3(-koniecSchodou, 30, -1600)));
            Parent.engine.stenyF.Add(new BoundingBox(new Vector3(-1160, 0, -1700), new Vector3(-koniecSchodou, 40, -1600)));
            Parent.engine.stenyF.Add(new BoundingBox(new Vector3(-1140, 0, -1700), new Vector3(-koniecSchodou, 50, -1600)));
            Parent.engine.stenyF.Add(new BoundingBox(new Vector3(-1120, 0, -1700), new Vector3(-koniecSchodou, 60, -1600)));
            Parent.engine.stenyF.Add(new BoundingBox(new Vector3(-1100, 0, -1700), new Vector3(-koniecSchodou, 70, -1600)));
            Parent.engine.stenyF.Add(new BoundingBox(new Vector3(-1080, 0, -1700), new Vector3(-koniecSchodou, 80, -1600)));
            Parent.engine.stenyF.Add(new BoundingBox(new Vector3(-1060, 0, -1700), new Vector3(-koniecSchodou, 90, -1600)));
            Parent.engine.stenyF.Add(new BoundingBox(new Vector3(-1040, 0, -1700), new Vector3(-koniecSchodou, 100, -1600)));
            Parent.engine.stenyF.Add(new BoundingBox(new Vector3(-1020, 0, -1700), new Vector3(-koniecSchodou, 110, -1600)));
            Parent.engine.stenyF.Add(new BoundingBox(new Vector3(-1000, 0, -1700), new Vector3(-koniecSchodou, 120, -1600)));
            Parent.engine.stenyF.Add(new BoundingBox(new Vector3(-980, 0, -1700), new Vector3(-koniecSchodou, 130, -1600)));
            Parent.engine.stenyF.Add(new BoundingBox(new Vector3(-960, 0, -1700), new Vector3(-koniecSchodou, 140, -1600)));
            Parent.engine.stenyF.Add(new BoundingBox(new Vector3(-940, 0, -1700), new Vector3(-koniecSchodou, 150, -1600)));*/
            int x1=1200;
           
            for(int pocetschodou=70,i=0;pocetschodou>0;i+=10,pocetschodou-- )
            {
            
            Parent.engine.stenyF.Add(new BoundingBox(new Vector3(i-x1-10 , 0 , -1700), new Vector3(-x1+i, i+10, -1600)));
            }

            //bočné steny
            Parent.engine.stenyF.Add(new BoundingBox(new Vector3(-20, -1, -3010), new Vector3(20, 320, -1180)));
            Parent.engine.stenyF.Add(new BoundingBox(new Vector3(-20, -5, 1180), new Vector3(20, 320, 3010)));///
            
            //oddelovacie steny
            Parent.engine.stenyF.Add(new BoundingBox(new Vector3(-1800, 0, -600), new Vector3(-1800, 500, 600)));//
            Parent.engine.stenyF.Add(new BoundingBox(new Vector3(1790, 0, -600), new Vector3(1800, 500, 600)));//
            
            //kocky
            Parent.engine.stenyF.Add(new BoundingBox(new Vector3(-94, 0, -393), new Vector3(94, 200, -208)));//
            Parent.engine.stenyF.Add(new BoundingBox(new Vector3(-393, 0, -94), new Vector3(-208, 200, 94)));//
            Parent.engine.stenyF.Add(new BoundingBox(new Vector3(-94, 0, 208), new Vector3(94, 200, 393)));
            Parent.engine.stenyF.Add(new BoundingBox(new Vector3(208, 0, -94), new Vector3(393, 200, 94)));

            Parent.engine.stenyT.Add(new BoundingBox(new Vector3(500, 0, 500), new Vector3(300, 500, 300)));

            //dolná plocha
            Parent.engine.stenyF.Add(new BoundingBox(new Vector3(-4000, -50, -4000), new Vector3(4000, 0,4000)));
            //spawn
            Parent.engine.stenyF.Add(new BoundingBox(new Vector3(-100, 950, -100), new Vector3(100, 1000, 100)));
            /*Parent.engine.stenyF = new List<BoundingBox>(8);
            Parent.engine.stenyT = new List<BoundingBox>();
            d= new BoundingBox(new Vector3(3001, 0, 3001), new Vector3(-3001, 500, -3001));
            
            Parent.engine.stenyT.Add(d);
            
            
            Parent.engine.stenyF.Add(new BoundingBox(new Vector3(20, 0, -1200), new Vector3(-20, 500, -3000)));
            Parent.engine.stenyF.Add(new BoundingBox(new Vector3(20, 0, 1200), new Vector3(-20, 500, 3000)));
            Parent.engine.stenyF.Add(new BoundingBox(new Vector3(-1800, 0, -600), new Vector3(-1800,500, 600)));//
            Parent.engine.stenyF.Add(new BoundingBox(new Vector3(1800, 0, -600), new Vector3(1800, 500, 600)));//
            Parent.engine.stenyF.Add(new BoundingBox(new Vector3(-94, 0, -393), new Vector3(94, 500, -208)));//
            Parent.engine.stenyF.Add(new BoundingBox(new Vector3(-393, 0,-94 ), new Vector3(-208, 500, 94 )));//
            Parent.engine.stenyF.Add(new BoundingBox(new Vector3(-94, 0, 393), new Vector3(94, 500, 208)));
            Parent.engine.stenyF.Add(new BoundingBox(new Vector3(393, 0, -94), new Vector3(208, 500, 94)));

            Parent.engine.stenyF.Add(new BoundingBox(new Vector3(500, 0, 500), new Vector3(300, 500, 300)));*/
            base.Load();
        }
        
        public override void Draw(Matrix View, Matrix Projection, Vector3 CameraPosition)
        {
            if(zapnuteobrisy)
            {
            //BoundingBoxRenderer.Render(Parent.engine.ddd, Parent.Engine.graphics, Parent.Kamera.View, Parent.Kamera.Projection, Color.Yellow);
            foreach (BoundingBox Box in Parent.engine.stenyT)
            {

                BoundingBoxRenderer.Render(Box, Parent.Engine.graphics, Parent.Kamera.View, Parent.Kamera.Projection, Color.Yellow);
                
                
            }
            foreach (BoundingBox Box in Parent.engine.stenyF)
            {
                BoundingBoxRenderer.Render(Box, Parent.Engine.graphics, Parent.Kamera.View, Parent.Kamera.Projection, Color.Red);


            }}/*
            for (int j = 0; j < Parent.engine.ostatnyHraci.Count; j++)
                {
                Parent.AddComponent(Parent.engine.ostatnyHraci[j]);
            }*/
            int i;
            try
            {
                for (int j = 0; j < Parent.engine.ostatnyHraci.Count; j++)
                {
                    Parent.engine.ostatnyHraci[j].Draw(/*View, Projection, CameraPosition*/);
                }
            }
            catch (Exception e)
            { 
                //throw  e;
                
                i = 5;
                    if(true)
                        i=10;
            }
            base.Draw(View, Projection, CameraPosition);
        }
        bool zapnuteobrisy=false;
        public override void Update()
        {
            /*Parent.engine.stenyT.Add(new BoundingBox(new Vector3(3000, 0, 3000), new Vector3(-3000, 500, -3000)));

            Parent.engine.stenyF.Add(new BoundingBox(new Vector3(20, 0, -1200), new Vector3(-20, 500, -3000)));
            Parent.engine.stenyF.Add(new BoundingBox(new Vector3(20, 0, 1200), new Vector3(-20, 500, 3000)));
            Parent.engine.stenyF.Add(new BoundingBox(new Vector3(-1800, 0, -600), new Vector3(-1800, 500, 600)));
            Parent.engine.stenyF.Add(new BoundingBox(new Vector3(1800, 0, -600), new Vector3(1800, 500, 600)));


            Parent.engine.stenyF.Add(new BoundingBox(new Vector3(-94, 0, -393), new Vector3(94, 500, -208)));
            Parent.engine.stenyF.Add(new BoundingBox(new Vector3(-393, 0, -94), new Vector3(-208, 500, 94)));
            Parent.engine.stenyF.Add(new BoundingBox(new Vector3(-94, 0, 393), new Vector3(94, 500, 208)));
            Parent.engine.stenyF.Add(new BoundingBox(new Vector3(393, 0, -94), new Vector3(208, 500, 94)));

            Parent.engine.stenyT.Remove(new BoundingBox(new Vector3(3000, 0, 3000), new Vector3(-3000, 500, -3000)));

            Parent.engine.stenyF.Remove(new BoundingBox(new Vector3(20, 0, -1200), new Vector3(-20, 500, -3000)));
            Parent.engine.stenyF.Remove(new BoundingBox(new Vector3(20, 0, 1200), new Vector3(-20, 500, 3000)));
            Parent.engine.stenyF.Remove(new BoundingBox(new Vector3(-1800, 0, -600), new Vector3(-1800, 500, 600)));
            Parent.engine.stenyF.Remove(new BoundingBox(new Vector3(1800, 0, -600), new Vector3(1800, 500, 600)));


            Parent.engine.stenyF.Remove(new BoundingBox(new Vector3(-94, 0, -393), new Vector3(94, 500, -208)));
            Parent.engine.stenyF.Remove(new BoundingBox(new Vector3(-393, 0, -94), new Vector3(-208, 500, 94)));
            Parent.engine.stenyF.Remove(new BoundingBox(new Vector3(-94, 0, 393), new Vector3(94, 500, 208)));
            Parent.engine.stenyF.Remove(new BoundingBox(new Vector3(393, 0, -94), new Vector3(208, 500, 94)));*/

            if(Parent.engine.Input.StisknutaKlavesa(Keys.F7))
                zapnuteobrisy = !zapnuteobrisy;

            


            base.Update();
        }
    }
}
