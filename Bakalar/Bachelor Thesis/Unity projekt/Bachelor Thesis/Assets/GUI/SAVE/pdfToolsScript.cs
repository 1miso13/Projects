/*
 Autor: Michal Ondrejó
 Project: Bachelor Thesis - Tvorba řezů z 3D modelů pro vystřihovánky "sliceforms"
*/
using UnityEngine;
using UnityEngine.UI;

public class pdfToolsScript : MonoBehaviour {
    public GameObject pdfSaveSettings;
	// Use this for initialization
	void Start () {
	}
	
	// Update is called once per frame
	void Update () {
        int value = GetComponent<Dropdown>().value;
        pdfSaveSettings.SetActive(value == 0);
       
    }
}
