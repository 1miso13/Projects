/*
 Autor: Michal Ondrejó
 Project: Bachelor Thesis - Tvorba řezů z 3D modelů pro vystřihovánky "sliceforms"
*/
using UnityEngine;
using UnityEngine.UI;

public class ShowObjectToggleScript : MonoBehaviour {
    public GameObject gO;
	// Use this for initialization
	void Start () {
	    GetComponent<Toggle>().onValueChanged.AddListener((on) => { f(on); });
    }
	
	// Update is called once per frame
	void Update () {
	    
	}
    public void f(bool x)
    {
        gO.GetComponent<Renderer>().enabled = x;
    }
}
