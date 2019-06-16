/*
 Autor: Michal Ondrejó
 Project: Bachelor Thesis - Tvorba řezů z 3D modelů pro vystřihovánky "sliceforms"
*/
using System;
using UnityEngine;
using UnityEngine.UI;

public class SetSliderValueByTextScript : MonoBehaviour {
    public char type='°';
    public GameObject SliderGO;
    // Use this for initialization
    public int defaultValue = 0;
	void Start () {
        GetComponent<InputField>().text = defaultValue.ToString() + type;
        GetComponent<InputField>().onEndEdit.AddListener(delegate { SetPath(GetComponent<InputField>()); });
    }
	
	// Update is called once per frame
	void Update () {
        
    }

    private void SetPath(InputField mainInputField)
    {
        //test if is valid
        int n;
        bool isNumeric = int.TryParse(mainInputField.text.Replace(type.ToString(), ""), out n);
        if (isNumeric)
        {
            SliderGO.GetComponent<Slider>().value = n;
            defaultValue = n;
        }
        else
        {//nastavime povodnu hodnitu
            mainInputField.text = defaultValue.ToString() + type;
        }

    }
}
