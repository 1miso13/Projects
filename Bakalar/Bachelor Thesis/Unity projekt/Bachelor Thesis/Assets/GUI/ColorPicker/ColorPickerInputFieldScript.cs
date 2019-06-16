/*
 Autor: Michal Ondrejó
 Project: Bachelor Thesis - Tvorba řezů z 3D modelů pro vystřihovánky "sliceforms"
*/
using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ColorPickerInputFieldScript : MonoBehaviour {
    public GameObject Slider;
    // Use this for initialization
    void Start () {
        GetComponent<InputField>().text = Slider.GetComponent<Slider>().value.ToString();
        GetComponent<InputField>().onEndEdit.AddListener(delegate { SetPath(GetComponent<InputField>()); });
    }

    private void SetPath(InputField inputField)
    {
        int value;
        if (int.TryParse(GetComponent<InputField>().text, out value))
        {
            Slider.GetComponent<Slider>().value = value;
        }
    }

    // Update is called once per frame
    void Update () {
        //GetComponent<InputField>().text = Slider.GetComponent<Slider>().value.ToString();
    }
}
