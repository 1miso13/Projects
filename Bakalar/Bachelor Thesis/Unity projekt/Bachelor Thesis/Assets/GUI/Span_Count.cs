/*
 Autor: Michal Ondrejó
 Project: Bachelor Thesis - Tvorba řezů z 3D modelů pro vystřihovánky "sliceforms"
*/
using UnityEngine;
using UnityEngine.UI;

public class Span_Count : MonoBehaviour {

    public GameObject SpanCountText;
    public GameObject SpanPanel;
    public GameObject CountPanel;
	// Use this for initialization
	void Start () {
        GetComponent<Slider>().onValueChanged.AddListener((value) => { ChangedValue(value); });
        
	}
	/// <summary>
    /// 
    /// </summary>
    /// <param name="x">Value</param>
    public void ChangedValue(float x)
    {
        SpanPanel.SetActive(x==0?true:false);
        CountPanel.SetActive(x==0?false:true);
        if (x == 0)
        {
            SpanCountText.GetComponent<Text>().text = "Span";
        }
        else
        {
            SpanCountText.GetComponent<Text>().text = "Count";
        }
    }
}
