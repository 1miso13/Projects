/*
 Autor: Michal Ondrejó
 Project: Bachelor Thesis - Tvorba řezů z 3D modelů pro vystřihovánky "sliceforms"
*/
using UnityEngine;
using UnityEngine.EventSystems;

public class mouseOver : MonoBehaviour, IPointerEnterHandler, IPointerExitHandler
{
    GameObject thePlayer;
    CameraScript playerScript;
    static public bool mouseExit, mouseEnter;
    public void OnPointerEnter(PointerEventData eventData)
    {
        mouseExit = false;
        mouseEnter = true;
    }

    public void OnPointerExit(PointerEventData eventData)
    {
        mouseEnter = false;
        mouseExit = true;
    }

    // Use this for initialization
    void Start() {
        thePlayer = GameObject.Find("Main Camera");
        playerScript = thePlayer.GetComponent<CameraScript>();
        mouseExit = false;
    }

    // Update is called once per frame
    void Update() {
        if (mouseExit && (!Input.GetMouseButton(0) && !Input.GetMouseButton(1) && !Input.GetMouseButton(2)))
        {
            playerScript.active = true;
        }
        if (mouseEnter && (!Input.GetMouseButton(0) && !Input.GetMouseButton(1) && !Input.GetMouseButton(2)))
        {
            playerScript.active = false;
        }
    }
}
