/*
 Autor: Michal Ondrejó
 Project: Bachelor Thesis - Tvorba řezů z 3D modelů pro vystřihovánky "sliceforms"
*/
using UnityEngine;
using UnityEngine.UI;

public class LoadButton : MonoBehaviour {
    public GameObject LoadPanel;
    public bool CantBeShown = false;
    // Use this for initialization
    void Start () {
        GetComponent<Button>().onClick.AddListener(() => { myFunctionForOnClickEvent(); });
    }

    // Update is called once per frame
    void myFunctionForOnClickEvent()
    {
        if (!CantBeShown)
        {

            LoadPanel.SetActive(!LoadPanel.activeSelf);
        }
        else
        {
            LoadPanel.SetActive(false);
        }
    }
}

