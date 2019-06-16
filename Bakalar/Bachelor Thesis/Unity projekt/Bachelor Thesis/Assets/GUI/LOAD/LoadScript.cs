/*
 Autor: Michal Ondrejó
 Project: Bachelor Thesis - Tvorba řezů z 3D modelů pro vystřihovánky "sliceforms"
*/
using UnityEngine;
using UnityEngine.UI;
using Assets.generovanie_prierezov;
using System;
using Assets;

public class LoadScript : MonoBehaviour
{
    GameObject generovanie;
    public GameObject ObjectPreview;
    public GameObject FileExplorerGO;
    Button myButton;
    // Use this for initialization
    void Start()
    {

        myButton = GetComponent<Button>();
        myButton.onClick.AddListener(() => { myFunctionForOnClickEvent(); });
        generovanie = GameObject.Find("GenerovaniePrierezov");

    }
    
    void myFunctionForOnClickEvent()
    {

        ObjImporter objImporter = new ObjImporter();
        string path = FileExplorerGO.GetComponent<FileExplorer>().path;


        if (path != "")
        {
            Mesh mesh;
            if ((mesh = objImporter.ImportFile(path)) != null)
            {
                ObjectPreview.transform.position = Vector3.zero;
                generovanie.GetComponent<generate>().objectNaSpracovanie.transform.position = Vector3.zero;
                generovanie.GetComponent<generate>().NameOfFile=(path.Substring(path.LastIndexOf(FileExplorer.delimiter) + 1)).Split('.')[0];

                var indices = mesh.triangles;
                mesh.vertices = CreateExtendVerticesForEveryTriangle(mesh.vertices,ref indices);
                //Set preview
                Mesh meshpreview=new Mesh();
                meshpreview.vertices = mesh.vertices;
                meshpreview.triangles = indices;
                meshpreview.RecalculateBounds();
                meshpreview.RecalculateNormals();
                ObjectPreview.GetComponent<MeshFilter>().mesh = meshpreview;

                //revert triangles
                var triangleCount = indices.Length / 3;
                for (var i = 0; i < triangleCount; i++)
                {
                    var tmp = indices[i * 3];
                    indices[i * 3] = indices[i * 3 + 1];
                    indices[i * 3 + 1] = tmp;
                    
                }
                mesh.triangles = indices;
                
                mesh.RecalculateBounds();
                mesh.RecalculateNormals();
                
                var posunBodov = mesh.bounds.center;
                mesh.vertices = PosunBody(mesh.vertices, posunBodov);
                ObjectPreview.GetComponent<MeshFilter>().mesh.vertices = mesh.vertices;
                generovanie.GetComponent<generate>().objectNaSpracovanie.GetComponent<MeshFilter>().mesh = mesh;
                generovanie.GetComponent<generate>().loaded = true;//umoznuje pouzit tlacidlo generovania prierezov

               
                Test.NewData("\n*************************************************************************\n*************************************************************************\nLoadObject", path,"Count triangles",mesh.triangles.Length, DateTime.Now.ToString());
                Test.Print();
                //nastaví velkost objektu
                setSize(generovanie.GetComponent<generate>().objectNaSpracovanie.GetComponent<MeshFilter>().mesh.bounds.size);
            }
            else
            {
                Debug.Log("Chyba pri načítaní objektu!");
            }
        }
    }

    private Vector3[] PosunBody(Vector3[] vertices, Vector3 posunBodov)
    {
        for (int i = 0; i < vertices.Length; i++)
        {
            vertices[i] -= posunBodov;
        }
        return vertices;
    }

    public GameObject Size;
    private void setSize(Vector3 size)
    {
        string s = "X = " + Math.Round(size.x,2) + "\nY = " + Math.Round(size.y,2) + "\nZ = " + Math.Round(size.z,2);
        Size.GetComponent<Text>().text = s;
        NotchesCreator.SizeOfObject = size;
    }
    private Vector3[] RevertNormals(Vector3[] norm)
    {
        for (int i=0; i< norm.Length;i++)
        {
            norm[i] *= -1;
        }
        return norm;
    }
    private Vector3[] CreateExtendVerticesForEveryTriangle(Vector3[] vertices,ref int[] triangles)
    {
        Vector3 [] NewVertices=new Vector3[triangles.Length];
        for (int i = 0; i < triangles.Length; i++)
        {
            NewVertices[i] = vertices[triangles[i]];
            triangles[i] = i;
        }
        return NewVertices;
    }
}

