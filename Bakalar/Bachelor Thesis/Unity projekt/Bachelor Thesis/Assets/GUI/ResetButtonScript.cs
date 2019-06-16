/*
 Autor: Michal Ondrejó
 Project: Bachelor Thesis - Tvorba řezů z 3D modelů pro vystřihovánky "sliceforms"
*/
using UnityEngine;
using UnityEngine.UI;

public class ResetButtonScript : MonoBehaviour {

    Button myButton;
    GameObject generovanie;
    // Use this for initialization
    void Start () {

        myButton = GetComponent<Button>();
        myButton.onClick.AddListener(() => { myFunctionForOnClickEvent(); });
        generovanie = GameObject.Find("GenerovaniePrierezov");
    }
	
	// Update is called once per frame
	void myFunctionForOnClickEvent() {
        generovanie.GetComponent<generate>().Clear();

        GameObject.Find("ShowSaveButton").GetComponent<Button>().interactable = false;
    }
}
