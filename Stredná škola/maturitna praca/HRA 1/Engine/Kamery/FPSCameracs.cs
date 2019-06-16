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
    public class FPSKamera :TargetKamera
    {
        public float fYaw, fPitch;
        //public Vector3 Pozicel, Pozicea;
        public float Yaw
        {
            get
            {
                return fYaw;
            }
            set
            {
                fYaw = value;
            }
        }

        public float Pitch
        {
            get
            {
                return fPitch;
            }
            set
            {
                fPitch = value;
            }
        }

        BoundingSphere koule;
        
        public FPSKamera(GameScreen okno, Vector3 pozice, Vector3 target)
            : base(okno, pozice, target)
        {
            fYaw = 0;
            fPitch = 0.000000001f;
            zzz = new zbran(Pozice,Matrix.CreateRotationX(-MathHelper.PiOver2),new Vector3(1f,1f,1f),"pistol");
            //koule = new BoundingSphere(pozice, 1);
        }
        public float rychlost;
        /// <summary>
        /// citlivosť myše
        /// </summary>
        Vector2 citlivost = new Vector2(0.5f,0.4f);
        /// <summary>
        /// Metóda Pitch
        /// </summary>
        private void fpitch()
        {
            fPitch -= Parent.Engine.Input.DeltaY*0.005f *  citlivost.Y;
        }
        bool klaknutie = false; bool kolidujeZ, kolidujeX, kolidujeY;

        float startY, jumpspeed = 0;
        bool jumping;
        /// <summary>
        /// Metóda Update()
        /// </summary>
        /// 
        public override void Update()
        {
            #region Rotácia
            Parent.engine.fYaw = fYaw;
           // bool ddscdcsdA=false;
            bool ddscdcsdB = false;

            #region Pitch
            if (fPitch < -1.4)
            {
                fPitch = -1.4f; ddscdcsdB = true;
                
            }
            if (fPitch > 1.4)
            {
                fPitch = 1.4f; ddscdcsdB = true;
            }
            Parent.engine.fPitch = fPitch;
            
            if (!ddscdcsdB)
                fpitch();
            #endregion Pitch
            #region Yaw
            fYaw -= Parent.Engine.Input.DeltaX * 0.01f*citlivost.X;

            if (fYaw < -Math.PI) fYaw += 2 * (float)Math.PI /*6.25f*/;
            if (fYaw > Math.PI) fYaw -= 2 * (float)Math.PI/*6.25f*/;

            if (Parent.Engine.Input.DrzenaKlavesa(Keys.Right) || Parent.Engine.Input.DrzenaKlavesa(Keys.D)) fYaw -= 0.05f;
            if (Parent.Engine.Input.DrzenaKlavesa(Keys.Left) || Parent.Engine.Input.DrzenaKlavesa(Keys.A)) fYaw += 0.05f;
            #endregion yaw
            #endregion Rotácia
            #region posun
            Matrix rotace = Matrix.CreateFromYawPitchRoll(fYaw, 0, 0);
            /*
            //skok
            
            if (jumping)
            {

                PoziceY -= jumpspeed;//Making it go up
                jumpspeed += 1;//Some math (explained later)
                if (PoziceY <= startY)
                //If it's farther than ground
                {
                    PoziceY = startY;//Then set it on
                    jumping = false;
                }
            }
            else
            {
                if (Parent.Engine.Input.StisknutaKlavesa(Keys.Space))
                {
                    jumping = true;
                    jumpspeed = -30;//Give it upward thrust
                    startY = PoziceY;
                }
            }*/


            Vector3 posun = Vector3.Zero;
            if (Parent.Engine.Input.DrzenaKlavesa(Keys.Up) || Parent.Engine.Input.DrzenaKlavesa(Keys.W)) posun += Vector3.Forward;
            if (Parent.Engine.Input.DrzenaKlavesa(Keys.Down) || Parent.Engine.Input.DrzenaKlavesa(Keys.S)) posun += Vector3.Backward;
            if (Parent.Engine.Input.DrzenaKlavesa(Keys.Q)) posun += Vector3.Left;
            if (Parent.Engine.Input.DrzenaKlavesa(Keys.E)) { posun += Vector3.Right; }

            if(!Parent.engine.Input.PraveTlacitkoDrzeno())
                Projection = Matrix.CreatePerspectiveFieldOfView(MathHelper.PiOver2,1.5f/* Parent.Engine.GraphicsDevice.Viewport.AspectRatio*/, 1f, FarPlane);
            else
                Projection = Matrix.CreatePerspectiveFieldOfView(MathHelper.PiOver4, Parent.Engine.GraphicsDevice.Viewport.AspectRatio, 1f, FarPlane);





            


           /* if (Parent.engine.Input.PraveTlacitkoDrzeno())
            {
                
                /*
                Pozicel = Pozicea;
                Pozicel += new Vector3((float)Math.Sin((double)Yaw) * (float)Math.Cos((double)Pitch), -(float)Math.Sin((double)Pitch) * 1f, ((float)Math.Cos((double)Yaw)) * (float)Math.Cos((double)Pitch));
                Pozicea= Pozice;
                Pozice = Pozicel;
                
            }
            //if (Parent.engine.Input.PraveTlacitkoPusteno())
                
            Parent.engine.pozicel = Pozicel;*/




            //rychlosť - beh / chôdza
            if ((Parent.Engine.Input.DrzenaKlavesa(Keys.LeftShift) || Parent.Engine.Input.DrzenaKlavesa(Keys.RightShift)) && !klaknutie) rychlost = 1f;
            //if (!Parent.Engine.Input.DrzenaKlavesa(Keys.LeftShift) || !Parent.Engine.Input.DrzenaKlavesa(Keys.RightShift) || Parent.Engine.Input.DrzenaKlavesa(Keys.LeftAlt) || Parent.Engine.Input.DrzenaKlavesa(Keys.RightAlt)) rychlost = 0.01f;
            else
                rychlost = 0.5f;
            
            posun *= rychlost * (float)Parent.Engine.GameTime.ElapsedGameTime.TotalMilliseconds*0.5f;
            #endregion posun
            #region kolízie////////////////////////////////////////////////////////////////////////////////////////

            Vector3 staraPozice = Pozice;

            Pozice += Vector3.Transform(posun, rotace);
            //koule.Center = Pozice- new Vector3(0,-50,0);
            Parent.engine.jkjbkjb = 0;
            Parent.engine.jkjbkjbd = 0;
            koule = new BoundingSphere(Pozice, 5);
            kolidujeX = false; kolidujeZ = false; kolidujeY = false;
            if(true)    {            
            /*foreach (BoundingBox Box in Parent.engine.stenyT)
            {
                //Parent.engine.jkjbkjbd++;
                if ( Box.Intersects(koule))
                {
                    koliduje=false;
                    //Pozice += Vector3.Transform(posun, rotace);

                }
                
                else
                    
                    koliduje=true;
            }*/
                
            //test ći koliduje Pozicia Z

            Vector3 Pozice1 = staraPozice;
            Pozice1.Z = Pozice.Z;
            koule = new BoundingSphere(Pozice1, 5);
            foreach (BoundingBox Box in Parent.engine.stenyF)
            {
                Parent.engine.jkjbkjb++;
                if (koule.Intersects(Box) || Box.Intersects(koule))
                {
                    kolidujeZ = true;
                    Parent.engine.jkjbkjbd++;
                    Parent.engine.kolizia_S_Objektom_index= Parent.engine.stenyF.IndexOf(Box);
                }
                
            }
            //test či koliduje pozicia X
            Vector3 Pozice2 = staraPozice;
            Pozice2.X = Pozice.X ;
            koule = new BoundingSphere(Pozice2, 5);
            foreach (BoundingBox Box in Parent.engine.stenyF)
            {
                Parent.engine.jkjbkjb++;
                if (koule.Intersects(Box) || Box.Intersects(koule))
                {
                    kolidujeX = true;
                    Parent.engine.jkjbkjbd++;
                    Parent.engine.kolizia_S_Objektom_index = Parent.engine.stenyF.IndexOf(Box);
                }

            }
            #region 
            /*

             Pozice2 = staraPozice;
            Pozice2.Y = Pozice.Y;
            foreach (BoundingBox Box in Parent.engine.stenyF)
            {
                Parent.engine.jkjbkjb++;
                if (koule.Intersects(Box) || Box.Intersects(koule))
                {
                    kolidujeY = true;
                    Parent.engine.jkjbkjbd++;
                    Parent.engine.kolizia_S_Objektom_index = Parent.engine.stenyF.IndexOf(Box);
                }

            } */
            /* foreach (BoundingBox Box in Parent.engine.stenyT)
             {
        
                 if(!koliduje)
                     if (Box.Intersects(koule) /*!Box.Intersects(koule))
                 {//koule.Contains(Box);
                     koliduje = true;
                     Parent.engine.jkjbkjb = Parent.engine.stenyT.IndexOf(Box);
         Parent.engine.jkjbkjbd++;
                 }

             }
             foreach (BoundingBox Box in Parent.engine.stenyF)
             {
        
                 if(!koliduje)
                 if (koule.Intersects(Box))
                 {
                     koliduje = true;
                     Parent.engine.jkjbkjbd = Parent.engine.stenyF.IndexOf(Box);
         Parent.engine.jkjbkjb++;
                 }

             }
             */
            #endregion
            if (kolidujeZ)
                PoziceZ = staraPozice.Z;
            if (kolidujeX)
                PoziceX = staraPozice.X;
            Ray vyska = new Ray(Pozice, /*Pozice +  */new Vector3(0,-1,0));
            testray = vyska;
            
            if (jumping)
            {
                
                
                
                float minimum=-100000;
                foreach (BoundingBox Box in Parent.engine.stenyF)
                {
                    if (vyska.Intersects(Box) != null)
                        if (minimum < PoziceY - (float)vyska.Intersects(Box)+70)
                            minimum = PoziceY - (float)vyska.Intersects(Box)+70;

                }

                

                PoziceY -= jumpspeed;//Making it go up
                jumpspeed += 1;//Some math (explained later)
                if (PoziceY <= -10000)
                    Pozice = new Vector3 (0,10000,0);
                else
                    startY = minimum; 
                if (PoziceY <= startY)
                //If it's farther than ground
                {
                    PoziceY = startY;//Then set it on
                    jumping = false;
                    jumpspeed = 0;
                }
            }
            else
            {
                if (Parent.Engine.Input.StisknutaKlavesa(Keys.Space))
                {
                    jumping = true;
                    jumpspeed = -25;//Give it upward thrust
                    startY = PoziceY;

                }
            }
            Parent.engine.rychlostpadania = jumpspeed;

            kolidujeY = false;
            bool d = false; float min=1000000;/*
            foreach (BoundingBox Box in Parent.engine.stenyF)
            {
                if (vyska.Intersects(Box) < 70f)
            {
                //kolidujeY = true;
                d = true;
            }
                if (vyska.Intersects(Box) != null)
               
                {
                    
                        if (min > (float)vyska.Intersects(Box))
                            min = (float)vyska.Intersects(Box);
                            Parent.engine.vyskaodpodlahy = min;
                }
                

            }
            */
            if (  min  < 70 && min > 0)
                {
                    PoziceY += min;d = true;
                }
                
                if(!d)
                    if (!jumping)
                    {
                        float minimum = -100000;
                        foreach (BoundingBox Box in Parent.engine.stenyF)
                        {
                            if (vyska.Intersects(Box) != null)
                                if (minimum < PoziceY - (float)vyska.Intersects(Box) + 70)
                                    minimum = PoziceY - (float)vyska.Intersects(Box) +70;

                        }
                        
                        PoziceY -= jumpspeed;//Making it go up
                        //Some math (explained later)
                        if (PoziceY <= -10000)
                        {
                            PoziceY = 10000;
                            PoziceX = 0; PoziceZ = 0;
                        }
                        else
                            startY = minimum;
                        if (PoziceY <= startY)
                        //If it's farther than ground
                        {
                            PoziceY = startY;//Then set it on
                            jumpspeed = 0;
                        }
                       /* if (PoziceY-70 <= startY)
                        //If it's farther than ground
                        {
                            PoziceY = minimum;//Then set it on
                            jumpspeed = 0;
                        }*/
                        else
                        {
                            jumpspeed += 1;
                        }
                    }
            #region
            /*
            foreach (BoundingBox Box in Parent.engine.stenyF)
            {
                Parent.engine.jkjbkjb++;
                if (koule.Intersects(Box) || Box.Intersects(koule))
                {
                    kolidujeY = true;
                    Parent.engine.jkjbkjbd++;
                    Parent.engine.kolizia_S_Objektom_index = Parent.engine.stenyF.IndexOf(Box);
                }

            } 
            */
#endregion
            /*if (kolidujeY)
                PoziceY = staraPozice.Y;*/
            }
            //klaknutie
            if(!this.jumping)
            if (Parent.Engine.Input.DrzenaKlavesa(Keys.LeftControl))
            {
                //if (!klaknutie)
                    Pozice += new Vector3(0, -20, 0);
                klaknutie = true;
            }
            else
                if (Parent.Engine.Input.PustenaKlavesa(Keys.LeftControl))
                {
                   // if (klaknutie)
                        Pozice += new Vector3(0, 20, 0);
                    klaknutie = false;
                }

            rotace = Matrix.CreateFromYawPitchRoll(fYaw, fPitch, 0);
            Target = Pozice + Vector3.Transform(/*Vector3.Forward*/ new Vector3(0,0,-10000000), rotace);
        
#endregion            
            
            if (Parent.engine.GraphicsDevice.Adapter.IsDefaultAdapter && Parent.engine.aktivne)
            {
                Parent.Engine.Input.CenterMouse();

            }
            /*
            //strelba
            if (Parent.Engine.Input.LeveTlacitkoStisknuto())
            {
                
                //zvuk.zvukRada.Play();
                //new gulka(Pozice,fYaw,fPitch,0f);
            }*/
            Parent.engine.pozice = Pozice;
            
            Parent.RemoveComponent(zzz);
            zzz.zbrand(Pozice + new Vector3(-(float)Math.Sin((double)-Yaw)*0.05f - (float)Math.Cos((double)Yaw+4.5 )*10, -(float)Math.Sin((double)Pitch)*-10-3, (float)Math.Cos((double)-Yaw)*0.05f + (float)Math.Sin((double)Yaw+4.5)*10), Matrix.CreateFromYawPitchRoll(Yaw, Pitch + -MathHelper.PiOver2, -MathHelper.PiOver2), Pitch, Yaw);
            
            Parent.AddComponent(zzz);
            if(Parent.engine.health==0)
            Pozice=new Vector3(0,2000,0);

            base.Update();
  /*          foreach (GameScreen z in Parent.engine.Screens)
            {
                if (z is zbran)
                {
                    z.Update();
                }
                //Parent.engine.ga.zbran1.Update();
            }*/
        }
       // public bool IsActive { get; }
        Ray testray;

        //public zvuk zvuk= new zvuk( );

        zbran zzz;
        public override void Draw(Matrix View, Matrix Projection, Vector3 CameraPosition)
        {
            //BoundingBoxRenderer.Render(this.koule, Parent.Engine.graphics, Parent.Kamera.View, Parent.Kamera.Projection, Color.Yellow);
            testray.Position.X+= 100f;
            testray.Direction.X+= 100f;
            RayRenderer.Render(testray,1000f,Parent.engine.GraphicsDevice,View,Projection,Color.Green);
            base.Draw(View, Projection, CameraPosition);
        }
        
    }
}
