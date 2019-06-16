/*
 Autor: Michal Ondrejó
 Project: Bachelor Thesis - Tvorba řezů z 3D modelů pro vystřihovánky "sliceforms"
*/
using UnityEngine;
using UnityEngine.UI;
using System;

public class SliderBarScript : MonoBehaviour {

    public Slider mainSlider;
    public Slider SliderX;
    public Slider SliderY;
    public Slider SliderZ;

    public GameObject Text;
    // Use this for initialization
    void Start () {

        mainSlider = (T == 1 ? SliderX : (T == 2 ? SliderY : SliderZ));
        mainSlider.onValueChanged.AddListener(delegate { ValueChangeCheck(); });

    }
	    
	// Update is called once per frame
	void Update () {
	
	}
    GameObject @object;
    public string type;
    public int T;//1 pitch 2 yaw 3 roll
    public void ValueChangeCheck()
    {
        @object = GameObject.Find(type);
        float pitch = ((T == 1) ? mainSlider.value : SliderX.value);
        float roll = (T == 2) ? mainSlider.value : SliderY.value;
        float yaw = (T == 3) ? mainSlider.value : SliderZ.value ;
        Vector3[] vert= new Vector3[4] {
            Quaternion.Euler((T==1?0:90)+pitch, yaw, roll) * new Vector3(-1,0,-1),
            Quaternion.Euler((T==1?0:90)+pitch, yaw, roll) * new Vector3(-1,0,1),
            Quaternion.Euler((T==1?0:90)+pitch, yaw, roll) * new Vector3(1,0,1),
            Quaternion.Euler((T==1?0:90)+pitch, yaw, roll) * new Vector3(1,0,-1) };
        Vector3[] normals = new Vector3[8];
        Vector3[] verts = new Vector3[8];
        Array.Copy(vert, verts, vert.Length);
        Array.Copy(vert, 0, verts, vert.Length, vert.Length);
        
        
        normals[0] = new Vector3(0, 1, -1); 
        normals[1] = new Vector3(0, 1, -1); 
        normals[2] = new Vector3(0, 1, -1); 
        normals[3] = new Vector3(0, 1, -1); 
        normals[4] = new Vector3(0, 1, -1); 
        normals[5] = new Vector3(0, 1, -1); 
        normals[6] = new Vector3(0, 1, -1); 
        normals[7] = new Vector3(0, 1, -1); 

        @object.GetComponent<MeshFilter>().mesh.vertices = verts;
        @object.GetComponent<MeshFilter>().mesh.normals = normals;


        Text.GetComponent<InputField>().text = (GetComponent<Slider>().value).ToString()+"°";
    }
}
