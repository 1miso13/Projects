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
    class Enemy : Model3D
    {
       // Matrix Projection,View;
       // int health=100;
        bool mrtvi;
        GameTime GameTime;
        public void die()
        {
            Parent.engine.enemy.Remove(koliznakrabica);
            mrtvi= true;
            //Rotace *= Matrix.CreateRotationX((float)Math.Cos((double)yaw)*(1.5f)) * Matrix.CreateRotationZ((float)Math.Cos((double)yaw) * 1.5f);
            Rotace *= Matrix.CreateRotationX(1.5f);
        }
        //public BoundingBox koliznakrabica;
        public Enemy(Vector3 pozice, Matrix rotace, string model)
            : base(pozice, rotace, model,true)
        {
            
        }
        //Random random = new Random();
        protected override void Load()
        {
            //ZakladniBox = Utility.VypoctiBoundingBox(Model, transformace);
            koliznakrabica = new BoundingBox(Pozice - new Vector3(20,  2, 20), Pozice + new Vector3(20, 70, 20));
            Parent.engine.enemy.Add(koliznakrabica);
            randomNumber = Parent.engine.random.Next(3, 5);
            smer = Parent.engine.random.Next(1, 3);
            base.Load();

//  dsdadsad = true;
            
        }
        
        int k=0;
        int randomNumber,smer;
        static float NextFloat(Random random)
        {
            double mantissa = (random.NextDouble() * 2.0) - 1.0;
            double exponent = Math.Pow(2.0, random.Next(-126, 128));
            return (float)(mantissa * exponent);
        }
        float oldYaw;
        public override void Update()
        {
            if (!mrtvi)
            {
                GameTime = Parent.engine.GameTime;
                k++;
                // 
                smer = Parent.engine.random.Next(1, 3);
                /*switch (smer)
                {

                    case 1: yaw +=0.1f;
                        return;
                    case 2: yaw-=0.1f;
                        return;
                }*/


                //yaw+= //NextFloat(Parent.engine.random);
                int dddddd = 2;//Parent.engine.random.Next(1, 3);
                if (k > 120)
                {
                    oldYaw = yaw;
                    if (dddddd == 2)
                    {
                        string d = Convert.ToString(Parent.engine.random.Next(-1, 2))+ "," + Convert.ToString(Parent.engine.random.Next(0,6 ));
                        double y = Convert.ToDouble(d);
                        yaw += (float)y;
                    }
                    /*if(dddddd==1)
                    {
                        string d = Convert.ToString(Parent.engine.random.Next(0, 2)) + "," + Convert.ToString(Parent.engine.random.Next(0, 6));
                        double y = Convert.ToDouble(d);
                        yaw -= (float)y;
                    }*/
                    oldYaw -= yaw;
                    k = 0;
                    Rotace *= Matrix.CreateRotationY(-oldYaw);

                }
                

               ///
               /// 
               Pozice += new Vector3((float)Math.Sin((double)yaw), 0, ((float)Math.Cos((double)yaw))) * new Vector3(GameTime.ElapsedGameTime.Milliseconds) * 0.1f;

                /*//for (int j = 0; j == 1; j++)
                {
                    switch (smer)
                    {
                        case 1: 
                            return;
                        case 2: 
                            return;
                    }
                }
            
                switch (yaw)
                {
                        
                                case 1: Pozice += Vector3.Forward;
                                    return;
                                case 2:
                                    Pozice += Vector3.Backward;
                                    return;
                        
                        
                    
                        
                                case 3: Pozice += Vector3.Right;
                                    return;
                                case 4:
                                    Pozice += Vector3.Left;
                                    return;
                                
                        
                       
            
                }

                i++;

                if (i > 50)
                {
                    i = 0;
                    randomNumber = Parent.engine.random.Next(3, 5);
                    switch(randomNumber)
                    {
                        case 3: //Pozice += Vector3.Left;
                            Rotace *= Matrix.CreateRotationY(-1.5f);

                            return;
                        case 4: //Pozice += Vector3.Right;
                            Rotace *= Matrix.CreateRotationY(1.5f);
                            return;
                    }
                }*/

                Parent.Engine.enemy.Remove(koliznakrabica);
                koliznakrabica.Max = Pozice + new Vector3(20, 70, 20);
                koliznakrabica.Min = Pozice - new Vector3(20, Pozice.Y-2, 20);
                Parent.Engine.enemy.Add(koliznakrabica);

                //Rotace *= Matrix.CreateRotationX(1f);
                foreach (Ray Sphere in Parent.engine.gulky)
                {

                    if (koliznakrabica.Intersects(Sphere)<=20  )
                    {
                        float? vzdialenost = koliznakrabica.Intersects(Sphere),vzdialenostOdSteny, vzdialenostOdStenytest = 1000000;
                        foreach (BoundingBox box in Parent.engine.stenyF)
                        {
                            vzdialenostOdSteny = Sphere.Intersects(box);
                            if (vzdialenostOdStenytest > vzdialenostOdSteny)
                            {
                                vzdialenostOdStenytest = vzdialenostOdSteny;
                            }

                            
                        }
                        if (vzdialenost < vzdialenostOdStenytest)
                        {
                            //int i = Parent.engine.enemy.IndexOf(box);
                            /* Random random = new Random();
                             int randomNumber = random.Next(0, 101);
                             health -= randomNumber;
                             if(health<=0)*/
                            die();
                        }
                        //Parent.engine.health =randomNumber;
                        ///Parent.engine.kkk.RemoveAt(i);
                    }
                }
                //Pozice += new Vector3(1, 0, 1);
                //Draw(View, Projection);
                foreach (BoundingBox Box in Parent.engine.stenyT)
                {

                    if (Box.Intersects(koliznakrabica))
                    {
                        yaw += 1.5f;

                        Rotace *= Matrix.CreateRotationY(1.5f);


                    }

                }
                foreach (BoundingBox Box in Parent.engine.stenyF)
                {

                    if (Box.Intersects(koliznakrabica))
                    {
                        yaw += 1.5f;

                        Rotace *= Matrix.CreateRotationY(1.5f);
                    }



                    ///foreach (ModelBone kost in Model.Bones)
                    {
                       // if(Model.Bones.IndexOf(kost)==2)
                            /*foreach(ModelBone kost1 in kost.Children)
                            {
                                if (kost.Children.IndexOf(kost1) == 1)
                                    foreach (ModelBone kost2 in kost1.Children)
                                    {
                                        //if (kost1.Children.IndexOf(kost2) == 1)
                                           // kost2.Transform *= Matrix.CreateRotationY(0.5f);
                                        foreach (ModelBone kost3 in kost2.Children)
                                        {
                                           // if (kost2.Children.IndexOf(kost3) == 1)
                                                kost3.Transform *= Matrix.CreateRotationY(0.5f);
                                        }
                                    }
                              //  kost1.Transform *= Matrix.CreateRotationY(0.0005f);
                            }
                       
                        //if (Model.Bones.IndexOf(kost) == 1)
                       // kost.Transform *= Matrix.CreateRotationY(0.001f);*/
                    }
                    
                }
                
               /* if(dsss)
                {
                    Rotace *= Matrix.CreateRotationZ(1.5f);
                    dsss = false;
                }*/
                base.Update();
            }
        }
///bool dsss=true ;
        /*protected void TransformBox()
        {
            Matrix transform = Matrix.CreateScale(Meritko) * Matrix.CreateTranslation(Pozice);
            BoundingBox transformed = ZakladniBox;
            transformed.Min = Vector3.Transform(transformed.Min, transform);
            transformed.Max = Vector3.Transform(transformed.Max, transform);

            Vector3[] body = new Vector3[8];
            transformed.GetCorners(body);
            for (int i = 0; i < body.Length; i++)
            {
                body[i] = Vector3.Transform(body[i], Rotace);
            }

            transformed = BoundingBox.CreateFromPoints(body);
            fTransformedBox.Min = transformed.Min;
            fTransformedBox.Max = transformed.Max;
        }*/
       // bool d = true;

        public virtual new void Draw(Matrix View, Matrix Projection, Vector3 CameraPosition)
        {
            //if (d)
            {
                //BoundingBoxRenderer.Render(this.koliznakrabica, Parent.Engine.graphics, Parent.Kamera.View, Parent.Kamera.Projection, Color.Yellow);
                //Update();
                //d = d;
            }
            Matrix worldMatrix = Matrix.CreateScale(0.0005f, 0.0005f, 0.0005f) *
           Matrix.CreateRotationY(MathHelper.Pi) *
           Matrix.CreateTranslation(new Vector3(19, 12, -5));

            Matrix[] xwingTransforms = new Matrix[Model.Bones.Count];
            Model.CopyAbsoluteBoneTransformsTo(xwingTransforms);

            BoundingBoxRenderer.Render(this.koliznakrabica, Parent.Engine.graphics, Parent.Kamera.View, Parent.Kamera.Projection, Color.Yellow);
            base.Draw();
        }

    }
}
