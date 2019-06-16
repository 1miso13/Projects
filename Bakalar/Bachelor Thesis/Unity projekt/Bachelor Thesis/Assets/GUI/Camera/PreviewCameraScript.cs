/*
 Autor: Michal Ondrejó
 Project: Bachelor Thesis - Tvorba řezů z 3D modelů pro vystřihovánky "sliceforms"
*/
using UnityEngine;

public class PreviewCameraScript : MonoBehaviour {
    public GameObject MainCamera;
	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
        transform.position= MainCamera.transform.position;
        transform.rotation = MainCamera.transform.rotation;

    }
}
