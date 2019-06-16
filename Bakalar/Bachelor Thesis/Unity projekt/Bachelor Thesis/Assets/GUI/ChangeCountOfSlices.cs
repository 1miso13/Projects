/*
 Autor: Michal Ondrejó
 Project: Bachelor Thesis - Tvorba řezů z 3D modelů pro vystřihovánky "sliceforms"
*/
using UnityEngine;
using UnityEngine.UI;
using System;
using UnityEngine.EventSystems;

public class ChangeCountOfSlices : MonoBehaviour, IPointerDownHandler, IPointerUpHandler
{

    // Use this for initialization
    Button myButton;
    GameObject Text;
    double l = 0.01;
    public string type, typeofOperation;
    void Start () {
        myButton = GetComponent<Button>();
        myButton.onClick.AddListener(() => { myFunctionForOnClickEvent(); });
        pressed = false;
        time = 0;

    }
    bool pressed;
    private void myFunctionForOnClickEvent()
    {
        Text = GameObject.Find(type);
        double num = double.Parse(Text.GetComponent<Text>().text);
        l=Text.GetComponent<l>().d;
        switch (typeofOperation)
        {
            case "sub":
                num = (num - l);
                if (Math.Log10(num) % 1 == 0)
                {
                    l=Text.GetComponent<l>().d /= 10;
                }
                break;
            case "add":

                if (Math.Log10(num) % 1 == 0)
                {
                    l=Text.GetComponent<l>().d *= 10;
                }
                num = (num + l);
                break;
        }
        Text.GetComponent<Text>().text = num.ToString();
    }
    float time;
    float clickAgain;
    // Update is called once per frame
    void Update ()
    {
        if (pressed)
        {
            time += Time.deltaTime;
            if (time > 0.75)
            {
                //obmedzenie zrychlovania 
                if (time>10)
                {
                    time = 10;
                }
                clickAgain +=time /20;
                if (clickAgain > 1)
                {
                    clickAgain = 0;
                    myFunctionForOnClickEvent();
                }
            }
        }
    }

    public void OnPointerDown(PointerEventData eventData)
    {
        pressed = true;
        clickAgain=time = 0;
    }

    public void OnPointerUp(PointerEventData eventData)
    {
        pressed = false;
        clickAgain=time = 0;
    }
}
