/*
 Autor: Michal Ondrejó
 Project: Bachelor Thesis - Tvorba řezů z 3D modelů pro vystřihovánky "sliceforms"
*/
using System;
using System.Collections.Generic;
using UnityEngine;

internal class ObjExporter
{
    int Offset;
    public ObjExporter()
    {
        Offset = 0;
    }

    string text = string.Empty;
    internal void Export(string path,string name)
    {

        
        try
        {
            System.IO.File.WriteAllText(path+name+".obj", text);
        }
        catch (Exception)
        {
            Debug.Log("Nepodarilo sa ulozit");
            
        }
    }
    
    internal void AddSlice(List<Vector3> listOfPoints, int[] triangles, string name)
    {
        text += "o " + name + "\n";
        for (int i = 0; i < listOfPoints.Count; i++)
        {
            text += "v " + listOfPoints[i].x + " " + listOfPoints[i].y + " " + listOfPoints[i].z + "\n";
        }

        for (int i = 0; i < triangles.Length; i += 3)
        {
            text += "f " + (triangles[i]+1+ Offset) + " " + (triangles[i + 1]+1+ Offset) + " " + (triangles[i + 2]+1+ Offset) + "\n";
        }


        Offset +=listOfPoints.Count;
    }
}