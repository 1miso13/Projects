/*
 Autor: Michal Ondrejó
 Project: Bachelor Thesis - Tvorba řezů z 3D modelů pro vystřihovánky "sliceforms"
*/
using UnityEngine;
using System;
public class CameraScript : MonoBehaviour {

    public bool active;
    public float speed, scrollSpeed;
    float vzdialenost;
    float x,xdelta;
    float y,ydelta;
    float z,zdelta,pitch,yaw;
    Vector3 point;

    int ScreenHeight, ScreenWidth;
	// Use this for initialization
	void Start () {
        speed = ((float)Math.PI*10)/100f;
        vzdialenost = 5;
        yaw = 0;
        pitch = 0;
        point = new Vector3(0,0,0);
        xdelta = 0;
        ydelta = 0;
        zdelta = 0;
        scrollSpeed = 0.5f;


        ScreenHeight = Screen.height;
        ScreenWidth = Screen.width;
    }
    public GameObject guiGO;
    // Update is called once per frame
    void Update()
    {

        if (Input.GetKeyDown(KeyCode.Z) && (Input.GetKey(KeyCode.LeftControl)))
            guiGO.SetActive(!guiGO.activeSelf);
        if (Input.GetKey("escape"))
            Application.Quit();
        if (active )
        {
            mouseScroll();
            mouseButtons();
            x = (float)Math.Sin((double)yaw) * (float)Math.Cos((double)pitch) * vzdialenost;
            z = (float)Math.Cos((double)yaw) * (float)Math.Cos((double)pitch) * vzdialenost;
            y = (float)Math.Sin((double)pitch) * vzdialenost;
        }
        if (Input.GetKeyDown(KeyCode.F) && (Input.GetKey(KeyCode.LeftControl) || Input.GetKey(KeyCode.RightControl)))
        {
            //Screen.fullScreen = !Screen.fullScreen;
            
            if (!Screen.fullScreen)
            {
                ScreenHeight = Screen.height;
                ScreenWidth = Screen.width;
                Screen.SetResolution(Screen.currentResolution.width, Screen.currentResolution.height, true);
            }
            else
            {
                Screen.SetResolution(ScreenWidth, ScreenHeight, false);
            }
        }
    }
    private void mouseScroll()
    {
        if (Input.GetAxis("Mouse ScrollWheel") != 0)
        {
            
            vzdialenost -= Input.GetAxis("Mouse ScrollWheel") * scrollSpeed * (vzdialenost>0.5f?vzdialenost:0.5f);
        }
        if (Input.GetKey(KeyCode.KeypadPlus))
        {
            vzdialenost -= 0.01f * scrollSpeed * (vzdialenost > 0.5f ? vzdialenost : 0.5f);
        }
        if (Input.GetKey(KeyCode.KeypadMinus))
        {
            vzdialenost += 0.01f * scrollSpeed * (vzdialenost > 0.5f ? vzdialenost : 0.5f);
        }
    }
    private void mouseButtons()
    {
        mouseButton0();
        mouseButton1();
        mouseButton2();
    }
    private void mouseButton0()
    {
        if (Input.GetMouseButton(0))
        {
            if (Input.GetAxisRaw("Mouse X") != 0)
                yaw += Input.GetAxisRaw("Mouse X")  * speed;
            if (Input.GetAxisRaw("Mouse Y") != 0)
                pitch -= Input.GetAxisRaw("Mouse Y")  * speed;
            if (pitch > (float)Math.PI / 2 - 0.001f)
                pitch = (float)Math.PI / 2 - 0.001f;
            if (pitch < (float)-Math.PI / 2 + 0.001f)
                pitch = (float)-Math.PI / 2 + 0.001f;
        }
        float speedTurbo=0.02f;
        if (Input.GetKey(KeyCode.LeftShift)|| Input.GetKey(KeyCode.RightShift))
        {
            speedTurbo = 0.05f;
        }
        if (Input.GetKey(KeyCode.Keypad6) || Input.GetKey(KeyCode.RightArrow))
        {
            yaw -= speedTurbo * speed;
        }
        if (Input.GetKey(KeyCode.Keypad4) || Input.GetKey(KeyCode.LeftArrow))
        {
            yaw += speedTurbo * speed;
        }
        if (Input.GetKey(KeyCode.Keypad2) || Input.GetKey(KeyCode.DownArrow))
        {
            pitch -= speedTurbo * speed;
        }
        if (Input.GetKey(KeyCode.Keypad8) || Input.GetKey(KeyCode.UpArrow))
        {
            pitch += speedTurbo * speed;
        }
        if (pitch > (float)Math.PI / 2 - 0.001f)
            pitch = (float)Math.PI / 2 - 0.001f;
        if (pitch < (float)-Math.PI / 2 + 0.001f)
            pitch = (float)-Math.PI / 2 + 0.001f;
    }
    private void mouseButton1()
    {
        if (Input.GetMouseButton(1))
        {

            point.x += Input.GetAxisRaw("Mouse X") * speed * (float)Math.Cos(yaw) * vzdialenost;
            xdelta += Input.GetAxisRaw("Mouse X") * speed * (float)Math.Cos(yaw) * vzdialenost;
            point.z -= Input.GetAxisRaw("Mouse X") * speed * (float)Math.Sin(yaw) * vzdialenost;
            zdelta -= Input.GetAxisRaw("Mouse X")* speed * (float)Math.Sin(yaw) * vzdialenost;

            point.y -= Input.GetAxisRaw("Mouse Y") * speed * (float)Math.Cos(pitch) * vzdialenost;
            ydelta -= Input.GetAxisRaw("Mouse Y") * speed  *(float)Math.Cos(pitch) * vzdialenost;
            point.x += Input.GetAxisRaw("Mouse Y") * speed* (float)Math.Sin(yaw) * (float)Math.Sin(pitch) * vzdialenost;
            xdelta += Input.GetAxisRaw("Mouse Y") * speed * (float)Math.Sin(yaw) * (float)Math.Sin(pitch) * vzdialenost;
            point.z += Input.GetAxisRaw("Mouse Y") * speed * (float)Math.Cos(yaw) * (float)Math.Sin(pitch) * vzdialenost;
            zdelta += Input.GetAxisRaw("Mouse Y") * speed * (float)Math.Cos(yaw) * (float)Math.Sin(pitch) * vzdialenost;
        }
    }
    private void mouseButton2()
    {
        if (Input.GetMouseButton(2))
        {
            vzdialenost -= Input.GetAxisRaw("Mouse Y") * scrollSpeed;
            vzdialenost -= Input.GetAxisRaw("Mouse X") * scrollSpeed/5;
        }
        transform.position = new Vector3(x + xdelta, y + ydelta, z + zdelta);
        transform.LookAt(point);
    }
}
