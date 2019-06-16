/*
 Autor: Michal Ondrejó
 Project: Bachelor Thesis - Tvorba řezů z 3D modelů pro vystřihovánky "sliceforms"
*/
using UnityEngine;
using UnityEngine.UI;

public class ShiftScript : MonoBehaviour {
    public bool IsX;
    public GameObject Text;
    public static float shiftX;
    public static float shiftY;
    // Use this for initialization
    void Start () {
        GetComponent<Slider>().onValueChanged.AddListener(delegate { ValueChangeCheck(); });
    }

    // Update is called once per frame
    void Update()
    {

    }
    public void ValueChangeCheck()
    {
        if (IsX)
        {
            shiftX = GetComponent<Slider>().value/100f;
        }
        else
        {
            shiftY = GetComponent<Slider>().value/100f;
        }
        Text.GetComponent<InputField>().text= (GetComponent<Slider>().value).ToString()+"%";
    }
	
}
