/*
 Autor: Michal Ondrejó
 Project: Bachelor Thesis - Tvorba řezů z 3D modelů pro vystřihovánky "sliceforms"
*/
using UnityEngine.UI;
using UnityEngine;

public class SetSliceCountScript : MonoBehaviour {
    /// <summary>
    /// Gameobject obsahujuci text s počtom rezov
    /// </summary>
    public GameObject valueGO;
    public bool increment;
	// Use this for initialization
	void Start () {
        GetComponent<Button>().onClick.AddListener(() => { ListenerOnClick(); });
    }
	
	void ListenerOnClick ()
    {
        var value = int.Parse(valueGO.GetComponent<Text>().text);
        if (increment)
        {
            value++;
        }
        else
        {
            value--;
        }
        if (value==0)
        {
            value++;
        }
        valueGO.GetComponent<Text>().text = value.ToString();
    }
}
