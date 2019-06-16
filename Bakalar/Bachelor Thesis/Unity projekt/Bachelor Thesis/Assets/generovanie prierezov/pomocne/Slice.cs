/*
 Autor: Michal Ondrejó
 Project: Bachelor Thesis - Tvorba řezů z 3D modelů pro vystřihovánky "sliceforms"
*/
using Assets;
using Assets.generovanie_prierezov.pomocne;
using System;
using System.Collections.Generic;
using UnityEngine;

public class Slice
{
    public GameObject gameobject;
    public int[] triangles;
    public List<Vector3> listOfPoints;
    Vector3 normal;
    List<GameObject> lines=new List<GameObject>();
    private List<Vector3> IPoints = new List<Vector3>();
    public Slice(List<Vector3> listOfPoints, Color color,Quaternion rotation,string name,Vector3 normal)
    {
        this.normal = normal;
        this.listOfPoints =  new List<Vector3>();
        for (int i = 0; i < listOfPoints.Count; i++)
        {
            this.listOfPoints.Add(listOfPoints[i]);
        }

        DateTime StartOfTriangulate=DateTime.Now;
        createTrianglesFromPoints(listOfPoints);
        Test.NewDataPost("Triangulate ","Count of points",listOfPoints.Count, "Ticks",(DateTime.Now-StartOfTriangulate).Ticks);
        gameobject = new GameObject(name);
        gameobject.AddComponent<MeshRenderer>();

        
        gameobject.transform.position = new Vector3(0, 0, 0);
        gameobject.transform.rotation =  rotation;
        gameobject.AddComponent<MeshFilter>();
        CreateMesh();
        SetColor(color);
        gameobject.GetComponent<MeshRenderer>().receiveShadows = false;


        AddLines();
    }
    public void AddLines()
    {
        if (CreateLines)
        {
            //tvorenie okrajov prierezov
            for (int i = 0; i < listOfPoints.Count; i++)
            {
                GameObject g;
                g = Line.DrawLine(listOfPoints[i], listOfPoints[i + 1 < listOfPoints.Count ? i + 1 : 0], Color.black, 2000f);
                g.transform.parent = gameobject.transform;
                lines.Add(g);
            }
        }
    }

    public void SetColor(Color color)
    {
        gameobject.GetComponent<Renderer>().material.color = color;
    }
    /// <summary>
    /// return Created triangles to variable "triangles"
    /// </summary>
    /// <param name="listOfPoints"></param>
    private void createTrianglesFromPoints(List<Vector3> listOfPoints)
    {
        Triangulator triangulator = new Triangulator(listOfPoints,normal);
        int[] triangle0 = triangulator.triangles.ToArray();

        triangles = new int[triangle0.Length * 2];
        Array.Copy(triangle0, triangles, triangle0.Length);
        Array.Reverse(triangle0);
        Array.Copy(triangle0, 0, triangles, triangle0.Length, triangle0.Length);        
    }
    public void Triangulate()
    {
        createTrianglesFromPoints(this.listOfPoints);
        CreateMesh();
    }
    static public Vector3 getNormal(List<Vector3> listOfPoints)
    {
        Vector3 point0 = listOfPoints[0];
        Vector3 point1 = listOfPoints[0 + 1];
        Vector3 point2 = listOfPoints[0 + 2];
        for (int i = 0; i < listOfPoints.Count-2; i++)
        {
            point0 = listOfPoints[i];
            point1 = listOfPoints[i + 1];
            point2 = listOfPoints[i + 2];
            int n = 0;
            if (Math.Abs(point0.x - point1.x) < 0.00001f && Math.Abs( point1.x - point2.x) < 0.00001f)
            {
                n++;
            }
            if (Math.Abs(point0.y - point1.y )< 0.00001f && Math.Abs(point1.y - point2.y) < 0.00001f)
            {
                n++;
            }
            if (Math.Abs(point0.z - point1.z)<0.00001f && Math.Abs(point1.z - point2.z) < 0.00001f)
            {
                n++;
            }
            if (n < 2)
            {
                break;
            }
        }
        
        return Vector3.Cross(point1 - point0, point2- point0).normalized;
    }
    internal void Destroy()
    {
        
        for (int i = 0; i < lines.Count; i++)
        {
            UnityEngine.Object.Destroy(lines[i]);
        }
        lines.Clear();
    }

    static public Vector3 getNormal(Vector3[] listOfPoints)
    {
        Vector3 point0 = listOfPoints[0];
        Vector3 point1 = listOfPoints[0 + 1];
        Vector3 point2 = listOfPoints[0 + 2];
        for (int i = 0; i < listOfPoints.Length - 2; i++)
        {
            point0 = listOfPoints[i];
            point1 = listOfPoints[i + 1];
            point2 = listOfPoints[i + 2];
            int n = 0;
            if (point0.x == point1.x && point1.x == point2.x)
            {
                n++;
            }
            if (point0.y == point1.y && point1.y == point2.y)
            {
                n++;
            }
            if (point0.z == point1.z && point1.z == point2.z)
            {
                n++;
            }
            if (n < 2)
            {
                break;
            }
        }

        return Vector3.Cross(point1 - point0, point2 - point0).normalized;
        
    }
    private void CreateMesh()
    {
        Mesh mesh = new Mesh();
        Vector3[] vert = listOfPoints.ToArray();
        Vector3[] verts = new Vector3[vert.Length ];
        Array.Copy(vert, verts, vert.Length);
        mesh.vertices = verts;
        mesh.triangles = triangles;

        Vector3[] normals = new Vector3[vert.Length];
        for (int i = 0; i < vert.Length; i++)
            normals[i] = new Vector3(0, 1, -1); 
        mesh.normals = normals;
        gameobject.GetComponent<MeshFilter>().mesh = mesh;
    }

    public Vector3 getNormal()
    {
        return normal;
    }
    public List<Vector3> getPoints()
    {
        return listOfPoints;
    }
    public List<int> IntersectingIndex=new List<int>();

    static public bool CreateLines = true;

    public void AddIPoints(List<Vector3> ListIPoints,int index)
    {
        IPoints.AddRange(ListIPoints);
        for (int i = 0; i < ListIPoints.Count; i++)
        {
            IntersectingIndex.Add(index);
        }

    }
    public List<Vector3> GetIPoints()
    {
        return IPoints;
    }
}

