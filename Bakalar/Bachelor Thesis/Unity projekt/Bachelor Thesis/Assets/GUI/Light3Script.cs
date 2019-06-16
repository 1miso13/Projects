/*
 Autor: Michal Ondrejó
 Project: Bachelor Thesis - Tvorba řezů z 3D modelů pro vystřihovánky "sliceforms"
*/
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Light3Script : MonoBehaviour {
    public GameObject lightGO;
    // Use this for initialization
    void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
        
        transform.position = -lightGO.transform.position;
        transform.LookAt(lightGO.transform.position);
    }
}
