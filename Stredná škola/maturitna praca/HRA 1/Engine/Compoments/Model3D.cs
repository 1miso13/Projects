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
    public class Model3D :Component
    {
        private string modelName;
        protected Model Model;
        public  float yaw=0;
        private Vector3 fPozice;
        public Matrix fRotace;
        private Vector3 fMeritko;
        private Matrix[] transformace;
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
        
        public Matrix Rotace
        {
            get
            {
                return fRotace;
            }
            set
            {
                fRotace = value;
            }
        }

        public Vector3 Meritko
        {
            get{return fMeritko;}
            set{fMeritko = value;}
        }

        public Model3D(Vector3 pozice, string model)
            : this(pozice, Matrix.Identity, model)
        {
            
        }
        public Model3D(Vector3 pozice,Matrix rotace, string model)
            : this(pozice,rotace,Vector3.One, model)
        {

        }
        public Model3D(Vector3 pozice, Matrix rotace, string model, bool kolidujuce)
            : this(pozice, rotace, Vector3.One, model)
        {

        }
        public Model3D(Vector3 pozice, Matrix rotace, Vector3 meritko, string model)
        {
            Pozice = pozice;
            modelName = model;
            Meritko = meritko;
            Rotace = rotace;
        }
        public bool kolizia= false;
        public Model3D(Vector3 pozice, Matrix rotace, Vector3 meritko, string model,bool kolidujuce)
            :this(pozice,rotace,meritko,model)
        {
            
            kolizia = kolidujuce;
        }
        protected override void Load()
        {
            Model = Parent.Engine.Content.Load<Model>(modelName);
            transformace = new Matrix[Model.Bones.Count];
            Model.CopyAbsoluteBoneTransformsTo(transformace);
        }
        
        public bool health=true;
        public override void Update()
        {
            /*vzdialenost= ;//vypocet vzdialenosti gulky od stredu 
            if(kolizia)
            {
                if (vzdialenost <= maxVzdialenost)
                {
                    health = false;
                }
            }*/
            
            

            base.Update();
        }
        public BoundingBox koliznakrabica;
        public bool dsdadsad=false;
        public override void Draw(Matrix View, Matrix Projection, Vector3 CameraPosition)
        {
            if (dsdadsad)
            {
                if (health)
                {
                    Matrix worldMatrix = Utility.CreateWorld(Pozice, Rotace, Meritko)/*Matrix.CreateScale(0.5f, 0.5f, 0.5f) *
               Matrix.CreateRotationY(MathHelper.Pi) *
               Matrix.CreateTranslation(new Vector3(19, 12, -5))*/;

                    Matrix[] xwingTransforms = new Matrix[Model.Bones.Count];
                    Model.CopyAbsoluteBoneTransformsTo(xwingTransforms);
                    //Matrix world = Utility.CreateWorld(Pozice, Rotace, Meritko);
                    int i = 0;
                    foreach (ModelMesh mesh in Model.Meshes)
                    {
                        foreach (ModelMeshPart part in mesh.MeshParts)
                        {
                            if (part.Effect is BasicEffect)
                            {
                                BasicEffect efekt = part.Effect as BasicEffect;
                                efekt.View = View;
                                efekt.Projection = Projection;
                                efekt.World = /*transformace[Model.Meshes.IndexOf(mesh)]  **/ xwingTransforms[mesh.MeshParts.IndexOf(part)]*worldMatrix;
                                efekt.EnableDefaultLighting();
                                i++;
                            }
                        }
                        mesh.Draw(); 
                    }
                    //i = i;
                    /*
                    foreach (ModelMesh mesh in Model.Meshes)
                    {
                        foreach (Effect currentEffect in mesh.Effects)
                        {
                            if (currentEffect is BasicEffect)
                            {
                                //currentEffect.CurrentTechnique = currentEffect.Techniques["BasicEffect"];
                                currentEffect.Parameters["World"].SetValue(xwingTransforms[mesh.ParentBone.Index] * worldMatrix);
                                currentEffect.Parameters..View = View;
                                currentEffect.Projection = Projection;
                                currentEffect.Parameters["View"].SetValue(View);
                                currentEffect.Parameters["Projection"].SetValue(Projection);
                            }
                        }
                        mesh.Draw();
                    }*/
                    if (Parent.engine.renderBoudingObject)
                        BoundingBoxRenderer.Render(this.koliznakrabica, Parent.Engine.graphics, Parent.Kamera.View, Parent.Kamera.Projection, Color.Yellow);
                }
            }
            else
            {
                if (health)
                {
                    
                    Matrix world = Utility.CreateWorld(Pozice, Rotace, Meritko);
                    foreach (ModelMesh mesh in Model.Meshes)
                    {
                        foreach (ModelMeshPart part in mesh.MeshParts)
                        {
                            if (part.Effect is BasicEffect)
                            {
                                BasicEffect efekt = part.Effect as BasicEffect;
                                efekt.View = View;
                                efekt.Projection = Projection;
                                efekt.World = transformace[mesh.ParentBone.Index] *world;
                                efekt.EnableDefaultLighting();
                            }
                        }
                        mesh.Draw();
                    }
                    if (Parent.engine.renderBoudingObject)
                        BoundingBoxRenderer.Render(this.koliznakrabica, Parent.Engine.graphics, Parent.Kamera.View, Parent.Kamera.Projection, Color.Yellow);
                }
            }
        }


    }
}
