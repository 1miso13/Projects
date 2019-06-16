/*
 Autor: Michal Ondrejó
 Project: Bachelor Thesis - Tvorba řezů z 3D modelů pro vystřihovánky "sliceforms"
*/
using UnityEngine;
using UnityEngine.UI;

public class MouseOverHelpScript : MonoBehaviour {

	// Use this for initialization
	void Start () {
        GetComponent<Button>().onClick.AddListener(() => { myFunctionForOnClickEvent(); });
    }
	
	// Update is called once per frame
	void Update () {

    }
    void myFunctionForOnClickEvent()
    {
        mouseOver.mouseEnter = false;
        mouseOver.mouseExit = true;
    }
}
