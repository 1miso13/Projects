/*
 Autor: Michal Ondrejó
 Project: Bachelor Thesis - Tvorba řezů z 3D modelů pro vystřihovánky "sliceforms"
*/
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ColorPickerSliderScript : MonoBehaviour {
    public GameObject inputField;
	// Use this for initialization
	void Start () {
        GetComponent<Slider>().onValueChanged.AddListener(delegate { ValueChangeCheck(); });
    }
    public void ValueChangeCheck()
    {
        inputField.GetComponent<InputField>().text = (GetComponent<Slider>().value).ToString();
    }
    // Update is called once per frame
    void Update () {
		
	}
}
