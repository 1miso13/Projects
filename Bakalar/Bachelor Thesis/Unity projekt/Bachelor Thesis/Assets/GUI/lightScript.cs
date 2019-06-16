/*
 Autor: Michal Ondrejó
 Project: Bachelor Thesis - Tvorba řezů z 3D modelů pro vystřihovánky "sliceforms"
*/
using System;
using UnityEngine;

public class lightScript : MonoBehaviour {
    public GameObject cameraGO;
    public bool fromCamera=true;
    public static CameraMode cameraMode;
    // Use this for initialization
    Vector3 DefPosition;
    Quaternion DefRotation;
     int multiplier = 10;
	void Start () {
        DefPosition = transform.position;
        DefRotation = transform.rotation;
        cameraMode = CameraMode.Camera;
    }
    public static bool rotate=true;
    public static bool reset=false;
    // Update is called once per frame
    void Update() {

        switch (cameraMode)
        {
            case CameraMode.Rotate:
                //rotating light around object
                transform.LookAt(default(Vector3));
                transform.Translate(Vector3.right * Time.deltaTime * multiplier);
                break;
            case CameraMode.Static:
                transform.rotation = DefRotation;
                transform.position = DefPosition;
                reset = false;
                rotate = false;
                break;
            case CameraMode.Camera:
                transform.rotation = cameraGO.transform.rotation;
                transform.position = cameraGO.transform.position;
                break;
            default:
                break;
        }
            if (Input.GetKeyDown("e") && (Input.GetKey(KeyCode.LeftControl) || Input.GetKey(KeyCode.RightControl)))
            {
                switch (cameraMode)
                {
                    case CameraMode.Rotate:
                        cameraMode = CameraMode.Static;
                        break;
                    case CameraMode.Static:
                        cameraMode = CameraMode.Camera;
                        break;
                    case CameraMode.Camera:
                        cameraMode = CameraMode.Rotate;
                        break;
                    default:
                        break;
                }
            }
        }

    
    public enum CameraMode
    {
        Rotate,
        Static,
        Camera
    }
}
