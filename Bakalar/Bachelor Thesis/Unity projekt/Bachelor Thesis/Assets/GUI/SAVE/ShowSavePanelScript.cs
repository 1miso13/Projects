/*
 Autor: Michal Ondrejó
 Project: Bachelor Thesis - Tvorba řezů z 3D modelů pro vystřihovánky "sliceforms"
*/
using UnityEngine;
using UnityEngine.UI;

public class ShowSavePanelScript : MonoBehaviour
{

    Button myButton;
    // Use this for initialization
    void Start()
    {

        myButton = GetComponent<Button>();
        myButton.onClick.AddListener(() => { myFunctionForOnClickEvent(); });
    }

    // Update is called once per frame
    void Update()
    {

    }
    public GameObject p;
    public bool CantBeShown=false;
    void myFunctionForOnClickEvent()
    {
        // p.GetComponent<Renderer>().enabled = true;
        if (!CantBeShown)
        {

            p.SetActive(!p.activeSelf);
        }
        else
        {
            p.SetActive(false);
        }
        
    }
}
