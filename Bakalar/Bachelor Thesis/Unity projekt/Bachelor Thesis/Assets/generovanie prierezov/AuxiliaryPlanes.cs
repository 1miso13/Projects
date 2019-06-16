/*
 Autor: Michal Ondrejó
 Project: Bachelor Thesis - Tvorba řezů z 3D modelů pro vystřihovánky "sliceforms"
*/
using System;
using UnityEngine;

namespace Assets.generovanie_prierezov
{
    public static class AuxiliaryPlanes
    {
        public static GameObject X; //ref na object X z ktoreho sa bude brat natocenie 
        public static GameObject Y;//ref na object Y z ktoreho sa bude brat natocenie 
        /// <summary>
        /// Create cutting planes 
        /// </summary>
        /// <param name="vert"></param>
        /// <param name="s"></param>
        /// <param name="position"></param>
        /// <param name="color"></param>
        /// <param name="refGameobject"></param>
        public static void CreateAuxiliaryPlanes(Vector3[] vert, string s, Vector3 position, Color color, ref GameObject refGameobject,Transform transform)
        {



            Mesh mesh = new Mesh();

            Vector3[] verts = new Vector3[8];
            Array.Copy(vert, verts, vert.Length);

            Array.Copy(vert, 0, verts, vert.Length, vert.Length);

            int[] tri = new int[12];
            tri[0] = 0;
            tri[1] = 2;
            tri[2] = 1;
            tri[3] = 3;
            tri[4] = 2;
            tri[5] = 0;

            tri[6] = 0;
            tri[7] = 1;
            tri[8] = 2;
            tri[9] = 0;
            tri[10] = 2;
            tri[11] = 3;
            Vector3[] normals = new Vector3[8];

            normals[0] = new Vector3(0, 1, -1);
            normals[1] = new Vector3(0, 1, -1);
            normals[2] = new Vector3(0, 1, -1);
            normals[3] = new Vector3(0, 1, -1);
            normals[4] = new Vector3(0, 1, -1);
            normals[5] = new Vector3(0, 1, -1); 
            normals[6] = new Vector3(0, 1, -1);
            normals[7] = new Vector3(0, 1, -1); 
            
            mesh.vertices = verts;
            mesh.triangles = tri;
            mesh.normals = normals;

            var gameObject = new GameObject(s);
            gameObject.AddComponent<MeshFilter>();

            gameObject.AddComponent<MeshRenderer>();

            gameObject.transform.position = position;
            gameObject.transform.rotation = transform.rotation;
            gameObject.GetComponent<MeshFilter>().mesh = mesh;

            gameObject.GetComponent<MeshRenderer>().receiveShadows = false;
            gameObject.GetComponent<MeshRenderer>().shadowCastingMode= UnityEngine.Rendering.ShadowCastingMode.Off;

            gameObject.GetComponent<Renderer>().material.color = color;
            refGameobject = gameObject;
        }
    }
}
