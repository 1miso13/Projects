/*
 Autor: Michal Ondrejó
 Project: Bachelor Thesis - Tvorba řezů z 3D modelů pro vystřihovánky "sliceforms"
*/
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ColorPickerSelectScript : MonoBehaviour {
    public GameObject image;
    public GameObject ColorPickerPanel;
	// Use this for initialization
	void Start () {
        GetComponent<Button>().onClick.AddListener(() => { myFunctionForOnClickEvent(); });
    }
	
	void myFunctionForOnClickEvent() {
        image.GetComponent<ColorPickerScript>().SelectedObject.GetComponent<Image>().color = image.GetComponent<Image>().color;
        ColorPickerPanel.SetActive(false);

    }
}
