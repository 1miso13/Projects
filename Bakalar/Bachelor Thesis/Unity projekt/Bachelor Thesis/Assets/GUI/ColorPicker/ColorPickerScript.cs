/*
 Autor: Michal Ondrejó
 Project: Bachelor Thesis - Tvorba řezů z 3D modelů pro vystřihovánky "sliceforms"
*/
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ColorPickerScript : MonoBehaviour
{
    public GameObject SelectedObject;
    private GameObject SelectedObjectold;
    public GameObject SliderR;
    public GameObject SliderG;
    public GameObject SliderB;
    public GameObject InputFieldR;
    public GameObject InputFieldG;
    public GameObject InputFieldB;
    public GameObject BackgroundR;
    public GameObject BackgroundG;
    public GameObject BackgroundB;
    // Use this for initialization
    void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
        if (SelectedObject!=SelectedObjectold)
        {
            SelectedObjectold = SelectedObject;
            var color = SelectedObject.GetComponent<Image>().color;
            SliderR.GetComponent<Slider>().value = color.r * 255;
            SliderG.GetComponent<Slider>().value = color.g * 255;
            SliderB.GetComponent<Slider>().value = color.b * 255;
        }
        int R = (int)SliderR.GetComponent<Slider>().value;
        int G = (int)SliderG.GetComponent<Slider>().value;
        int B = (int)SliderB.GetComponent<Slider>().value;

        BackgroundR.GetComponent<Image>().color = R / 255.0f * Color.red + Color.black;
        BackgroundG.GetComponent<Image>().color = G / 255.0f * Color.green + Color.black;
        BackgroundB.GetComponent<Image>().color = B / 255.0f * Color.blue + Color.black;

        GetComponent<Image>().color = R / 255.0f * Color.red+ G / 255.0f * Color.green+ B / 255.0f * Color.blue + Color.black;
	}
}
