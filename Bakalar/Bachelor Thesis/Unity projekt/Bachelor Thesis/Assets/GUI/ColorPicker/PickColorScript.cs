/*
 Autor: Michal Ondrejó
 Project: Bachelor Thesis - Tvorba řezů z 3D modelů pro vystřihovánky "sliceforms"
*/
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class PickColorScript : MonoBehaviour {

    public GameObject ColorPickerPanel;
    public GameObject Image;
    // Use this for initialization
    void Start () {
        GetComponent<Button>().onClick.AddListener(() => { myFunctionForOnClickEvent(); });
    }
	// Update is called once per frame
	void myFunctionForOnClickEvent() {
        Image.GetComponent<ColorPickerScript>().SelectedObject = gameObject;
        ColorPickerPanel.SetActive(true);
    }
}
