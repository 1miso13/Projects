/*
 Autor: Michal Ondrejó
 Project: Bachelor Thesis - Tvorba řezů z 3D modelů pro vystřihovánky "sliceforms"
*/
using UnityEngine;
using UnityEngine.UI;
using Assets.generovanie_prierezov;

public class HideTogglerScript : MonoBehaviour {
    Toggle t;
    // Use this for initialization
    void Start () {
        t = GetComponent<Toggle>();
        t.onValueChanged.AddListener((on) => { f(on); });
    }
	
	// Update is called once per frame
	public void f(bool x)
    {
        AuxiliaryPlanes.X.GetComponent<Renderer>().enabled = x;
        AuxiliaryPlanes.Y.GetComponent<Renderer>().enabled = x;
    }
}
