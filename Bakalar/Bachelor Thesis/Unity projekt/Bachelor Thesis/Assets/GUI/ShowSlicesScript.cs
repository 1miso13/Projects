/*
 Autor: Michal Ondrejó
 Project: Bachelor Thesis - Tvorba řezů z 3D modelů pro vystřihovánky "sliceforms"
*/
using Assets.generovanie_prierezov;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using System;

public class ShowSlicesScript : MonoBehaviour {
    public bool IsX;
	// Use this for initialization
	void Start () {
        GetComponent<Toggle>().onValueChanged.AddListener((on) => { f(on); });
    }

    private void f(bool on)
    { 
        for (int i = 0; i< (IsX ? SliceCreator.listX : SliceCreator.listY).Count; i++)
        {
            (IsX?SliceCreator.listX:SliceCreator.listY)[i].gameobject.SetActive(on);

        }
    }

    // Update is called once per frame
    void Update () {
    }
}
