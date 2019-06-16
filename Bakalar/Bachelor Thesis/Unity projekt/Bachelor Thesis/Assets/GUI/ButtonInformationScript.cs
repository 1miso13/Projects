/*
 Autor: Michal Ondrejó
 Project: Bachelor Thesis - Tvorba řezů z 3D modelů pro vystřihovánky "sliceforms"
*/
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ButtonInformationScript : MonoBehaviour {

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
        string moreInfo = "";
        if (Input.GetKey(KeyCode.M))
        {
            moreInfo = 
                  "Up    or num8 - Camera up" +
                "\nDown  or num2 - Camera down" +
                "\nLeft  or num4 - Camera left" +
                "\nRight or num6 - Camera right" +
                "\nNum +/- - Camera distance increase/decrease";
        }
        else
        {
            moreInfo = "M - More info";
        }
        GetComponent<Text>().text = "Esc - Exit"+
            "\nCtrl+Q - Show border lines:" + (Slice.CreateLines ? "On" : "Off") +
            "\nCtrl+F - FullScreen mode:" + (Screen.fullScreen ? "On" : "Off") +
            "\nCtrl+E - Change light mode:" + (lightScript.cameraMode.ToString())+
            "\n" +moreInfo; 
    }
}
