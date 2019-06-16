using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MojEngine;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace MojEngine
{
    public class MojeHerniOkno :GameScreen
    {
        public MojeHerniOkno()
            : base("Moje herne okno")
        {
            
        }
        Model3D clovek = new Model3D(new Vector3(50, 24, 0), "dude");
        Engine parent;
        
        protected override void Load()
        {AddComponent(new územie(new Vector3(0,0,0),"stena"));
            parent = base.Engine;
            base.Engine.enemy= new List<BoundingBox>();
            //engine.kkk = new List<testKolizii>();
            clovek = new Model3D(new Vector3(50, 24, 0), "dude");
            //premazanie
            
            //AddComponent(new BarevnePozadi(Color.Orange));
            AddComponent(new skybox(new Vector3( 0,0,0)));
            //AddComponent(new Enemy(new Vector3(250,/*24*/2,25), Matrix.CreateRotationX(0), "dude"));

            ludia();
            //target kamera
            Kamera = new FPSKamera(this, new Vector3(50, 1070,0), new Vector3(0,1070, 0));
     ///
     /// 
     /// Kamera = new FreeCamera(this, new Vector3(50, 1070, 0), new Vector3(0, 1070, 0));
     /// 

            //AddComponent(new Model3D(new Vector3(0,/*24*/2,0), Matrix.CreateScale(50f),"dude"));
            AddComponent(new Model3D(new Vector3(-70, -22+1000, -9),Matrix.Identity,new Vector3(2.1f,1f,1.17f), "koberec1"));
            //AddComponent( new Model3D(new Vector3(0,70,0),Matrix.CreateScale(50f),"projektil FBX"));
            //AddComponent(new Model3D(new Vector3(100, 0, -1000), Matrix.CreateScale(0.25f), @"building/Old Building 28"));
            /*AddComponent(new Sprite("erb", new Vector2(230, 170)));
            AddComponent(new Sprite("erb",new Vector2(420,220),200,50));*/
            //AddComponent(new TextLabel("Michal Ondrejó",new Vector2(300,350),"maly"));
            //AddComponent(new Model3D(new Vector3(20, 100, 500), Matrix.CreateScale(50f) *Matrix.CreateRotationX(-1.5f), "devil"));
            //AddComponent(new Model3D(new Vector3(50, 50, 0), "dude"));
            //RemoveComponent(clovek);
          //  parent.jbbjkkkkkkkkkkkkkb = new Enemy(new Vector3(2500,/*24*/2, 0), Matrix.CreateRotationX(0), /*"Victoria-hat-dance"*/"dude");
           // AddComponent(parent.jbbjkkkkkkkkkkkkkb);

            AddComponent(new Mriezka(1000, 20));
            
            AddComponent(new GUI());
        }
        void ludia()
        {
            for(int i=-1000;i<1000;i+=500)
            {
                for (int  j = -1000; j < 1000; j += 100)
                {
                    Enemy t = new Enemy(new Vector3(2500,/*24*/2, 0), Matrix.CreateRotationX(0), /*"Victoria-hat-dance"*/"dude");
                   // parent.kkk.Add(t);
                  ///  parent.jbbjb = new Enemy(new Vector3(2500,/*24*/2, 0), Matrix.CreateRotationX(0), /*"Victoria-hat-dance"*/"dude");
                    AddComponent(t);
                }
            
            }
        }

        
            
        

        
    }
    
}
