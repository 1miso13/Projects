/*
 Autor: Michal Ondrejó
 Project: Bachelor Thesis - Tvorba řezů z 3D modelů pro vystřihovánky "sliceforms"
 Čiastočne prebrané z: http://wiki.unity3d.com/index.php?title=ObjImporter
*/
using UnityEngine;
using System.Collections.Generic;
using System.IO;

public class ObjImporter
{

    private struct meshStruct
    {
        public List<int> triang;
        public List<Vector3> vert;
        public string fileName;
    }

    // Use this for initialization
    public Mesh ImportFile(string filePath)
    {
        meshStruct newMesh = createMeshStruct(filePath);

        populateMeshStruct(ref newMesh);
        Vector3[] newVerts = newMesh.vert.ToArray();
        
        float d =5;

        //normalizujeme objekt
        d=sizeOfObject(newVerts);
        for (int k = 0; k < newVerts.Length; k++)
        {
            Vector3 vert3 = newVerts[k];
            vert3.x = vert3.x / d;
            vert3.y = vert3.y / d;
            vert3.z = vert3.z / d;
            newVerts[k] = vert3;
        }
        Mesh mesh = new Mesh();

        mesh.vertices = newVerts;
        mesh.triangles = newMesh.triang.ToArray();

        mesh.RecalculateBounds();
        mesh.RecalculateNormals();
        

        return mesh;
    }
    private float sizeOfObject(Vector3[] vert)
    {
        float Xmin = vert[0].x;
        float Ymin = vert[0].y;
        float Zmin = vert[0].z;
        float Xmax = vert[0].x;
        float Ymax = vert[0].y;
        float Zmax = vert[0].z;
        for (int i = 1; i < vert.Length; i++)
        {
            if (vert[i].x > Xmax)
            {
                Xmax = vert[i].x;
            }
            if (vert[i].y > Ymax)
            {
                Ymax = vert[i].y;
            }
            if (vert[i].z > Zmax)
            {
                Zmax = vert[i].z;
            }

            if (vert[i].x < Xmin)
            {
                Xmin = vert[i].x;
            }
            if (vert[i].y < Ymin)
            {
                Ymin = vert[i].y;
            }
            if (vert[i].z < Zmin)
            {
                Zmin = vert[i].z;
            }
        }
        float xy = (Xmax - Xmin > Ymax - Ymin) ? Xmax - Xmin : Ymax - Ymin;
        return (Zmax-Zmin > xy) ? Zmax - Zmin : xy;
    }
    private static meshStruct createMeshStruct(string filename)
    {
        meshStruct mesh = new meshStruct();
        mesh.fileName = filename;
        mesh.triang = new List<int>();
        mesh.vert = new List<Vector3>();
        return mesh;
    }

    private static void populateMeshStruct(ref meshStruct mesh)
    {
        StreamReader stream = File.OpenText(mesh.fileName);
        string entireText = stream.ReadToEnd();
        stream.Close();
        using (StringReader reader = new StringReader(entireText))
        {
            string currentText = reader.ReadLine();

            char[] splitIdentifier = { ' ' };
            char[] splitIdentifier2 = { '/' };
            string[] brokenString;
            string[] brokenBrokenString;
            while (currentText != null)
            {
                if (!currentText.StartsWith("f ") && !currentText.StartsWith("v ") && !currentText.StartsWith("vt ") &&
                    !currentText.StartsWith("vn ") && !currentText.StartsWith("g ") && !currentText.StartsWith("usemtl ") &&
                    !currentText.StartsWith("mtllib ") && !currentText.StartsWith("vt1 ") && !currentText.StartsWith("vt2 ") &&
                    !currentText.StartsWith("vc ") && !currentText.StartsWith("usemap "))
                {
                    currentText = reader.ReadLine();
                    if (currentText != null)
                    {
                        currentText = currentText.Replace("  ", " ");
                    }
                }
                else
                {
                    currentText = currentText.Trim();
                    brokenString = currentText.Split(splitIdentifier);
                    switch (brokenString[0])
                    {
                        case "g":
                            break;
                        case "usemtl":
                            break;
                        case "usemap":
                            break;
                        case "mtllib":
                            break;
                        case "v":
                            mesh.vert.Add( new Vector3(System.Convert.ToSingle(brokenString[1]), System.Convert.ToSingle(brokenString[2]),
                                                     System.Convert.ToSingle(brokenString[3])));
                            break;
                        case "vt":
                            break;
                        case "vt1":
                            break;
                        case "vt2":
                            break;
                        case "vn":
                            break;
                        case "vc":
                            break;
                        case "f":

                            List<int> intArray = new List<int>();
                            int j = 1;
                            while (j < brokenString.Length && ("" + brokenString[j]).Length > 0)
                            {
                                
                                brokenBrokenString = brokenString[j].Split(splitIdentifier2);    //Separate the face into individual components (vert, uv, normal)
                                try
                                {
                                    intArray.Add(System.Convert.ToInt32(brokenBrokenString[0])-1);
                                }
                                catch { Debug.Log(brokenBrokenString[0]); }
                                j++;
                            }
                            j = 1;
                            while (j + 2 < brokenString.Length)     //Create triangles out of the face data.  There will generally be more than 1 triangle per face.
                            {
                                mesh.triang.Add(intArray[0]);
                                mesh.triang.Add(intArray[j]);
                                mesh.triang.Add(intArray[j+1]);
                                j++;
                            }
                            break;
                    }
                    currentText = reader.ReadLine();
                    if (currentText != null)
                    {
                        currentText = currentText.Replace("  ", " ");       //Some .obj files insert double spaces, this removes them.
                    }
                }
            }
        }
    }
}