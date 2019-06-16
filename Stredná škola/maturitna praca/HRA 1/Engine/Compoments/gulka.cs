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
    public class gulka :Model3D
    {
        public float rychlost;
        public Vector3 position, staraPozice;
        public float Yaw;
        public float Pitch;
        public float speed;
        public bool isactive;
        public Ray boudingSphere;
        GameTime GameTime;
        //Model3D bullet;
        //Component c;
        public gulka(Vector3 position,float Yaw, float Pitch,float speed)
            : base(/*new Vector3(0, 70, 0)*/position, (Matrix.CreateRotationX(Pitch + 3.15f)) * (Matrix.CreateRotationY((Yaw))), new Vector3(1f), "projektil FBX")
        {
            //c = new Model3D(new Vector3(0, 70, 0), Matrix.CreateScale(20f), "projektil FBX");
            //Parent.AddComponent(c);
            //bullet= new Model3D(position,Matrix.CreateScale(250f),"projektil FBX");
            //konštruktor
            this.rychlost = speed;
            this.Yaw = Yaw;
            //this.GameTime = Parent.engine.GameTime;
            this.position = position;
            this.Pitch = Pitch;
            isactive = true;
            boudingSphere = new Ray(-position, new Vector3((float)Math.Sin((double)Yaw) * (float)Math.Cos((double)Pitch), -(float)Math.Sin((double)Pitch) * 1f, ((float)Math.Cos((double)Yaw)) * (float)Math.Cos((double)Pitch))/* * new Vector3((float)GameTime.ElapsedGameTime.Milliseconds) */* 50f);
            //if(Parent.engine.gulky. == null)
            starapozicia = position;
            BoundingSphere = new BoundingSphere(position, 5);
            
            // (float)Parent.Engine.GameTime.ElapsedGameTime.TotalMilliseconds;
        }bool zastavenieGulky = false;
        BoundingSphere BoundingSphere;
        protected override void Load()
        {
            
            Parent.Engine.gulky.Add(boudingSphere);
            base.Load();
        }
        ~gulka()
        {
            
        }
        int timetodead;
        Vector3 starapozicia;
        public override void Update()
        {
            GameTime = Parent.engine.GameTime;
            if (isactive)
            {
                //posun
                
                //Pozice += new Vector3(0, 0, 1);
                for (int i = 0; i < 1; i++)
                {
                    //if(Pitch>1
                    Pozice -= new Vector3((float)Math.Sin((double)Yaw) * (float)Math.Cos((double)Pitch), -(float)Math.Sin((double)Pitch) * 1f, ((float)Math.Cos((double)Yaw)) * (float)Math.Cos((double)Pitch)) * new Vector3((float)GameTime.ElapsedGameTime.Milliseconds)*5f;
                    //zistenie či nastáva kolízia


                }
                timetodead++;
                if (timetodead >= 500)
                    zastavenie();
                
                ///
                ///zastavenie();
                //nad hranicou a pod hranicou
              /*  if (position.Y < 0 || position.Y > 500)
                    zastavenie();*/
            //   staraPozice = Pozice;
                
                
                Parent.Engine.gulky.Remove(boudingSphere);
                Vector3 dddddbfgfgfbhxd = /*starapozicia */- new Vector3((float)Math.Sin((double)Yaw) * (float)Math.Cos((double)Pitch), -(float)Math.Sin((double)Pitch) * 1f, ((float)Math.Cos((double)Yaw)) * (float)Math.Cos((double)Pitch)) * new Vector3((float)GameTime.ElapsedGameTime.Milliseconds) * 10f;
               // starapozicia = boudingSphere.Direction;
                boudingSphere = new Ray(Pozice/*starapozicia*/, dddddbfgfgfbhxd);

                BoundingSphere.Center = Pozice;
                
                
                //if (position != staraPozice)
                {
                   
                    Parent.Engine.gulky.Add(boudingSphere);
                }


                if(isactive)
                foreach (BoundingBox box in Parent.engine.enemy){
                    if (boudingSphere.Intersects(box)!=null)
                    {
                        
                    //int i = Parent.engine.enemy.IndexOf(box);
                    
                    //Parent.Engine.enemy.Remove(box);
                        /*if(dascscscs)Parent.Engine.gulky.Remove(boudingSphere);
                            isactive = false;
                        
                        Parent.RemoveComponent(this);
                        //this.zastavenie();
                        
                        
                        dascscscs = true;*/
                    ///Parent.engine.kkk.RemoveAt(i);
                }
                }
                //koliziagulky = new kolize(,boudingSphere);

            }//kolízia so stenou

            if(zastavenieGulky)
                zastavenie();

            foreach (BoundingBox Box in Parent.engine.stenyF)
            {

                

                if (boudingSphere.Intersects(Box)<20)
                {

                    //zastavenie();
                    zastavenieGulky = true;

                }

            }
            if (!isactive)
            {Parent.engine.gulky.Remove(boudingSphere);
                Parent.RemoveComponent(this); 
            }
            base.Update(/*GameTime*/);
        }
        
        void zastavenie()
        { 
            //Parent.RemoveComponent(c);
            
                isactive = false;
                
                //Parent.RemoveComponent(this);
        }
        /*public override void Draw(Matrix View, Matrix Projection, Vector3 CameraPosition)
        {
            
            
        }*/
        public override void Draw()
        {
            /*Parent.engine.SpriteBatch.Begin();
            //Parent.Engine.SpriteBatch.DrawString(maly, "Z:" + Parent.Engine.pozice.Z.ToString(), new Vector2(1, 80), Color.Black);
            Parent.engine.SpriteBatch.End(); */
            RayRenderer.Render(this.boudingSphere, 10000f, Parent.engine.graphics, Parent.Kamera.View, Parent.Kamera.Projection, Color.Red);
            base.Draw();
        }
    }
}
