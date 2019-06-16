/*
 Autor: Michal Ondrejó
 Project: Bachelor Thesis - Tvorba řezů z 3D modelů pro vystřihovánky "sliceforms"
*/
using UnityEngine;
using UnityEngine.UI;
using System.Collections.Generic;
using System;
using Assets.generovanie_prierezov;
using Assets;

public class generate : MonoBehaviour {
    public GameObject ColorXstart;
    public GameObject ColorXend;
    public GameObject ColorYstart;
    public GameObject ColorYend;


    public GameObject timer;
    // Use this for initialization
    Color colorOfXstart = Color.red;
    Color colorOfYstart = Color.green;
    Color colorOfXend = Color.blue;
    Color colorOfYend = Color.yellow;
    public bool loaded = false;//ak je false tak nebol nacitany ziadny objekt
    Button myButton;
    public float yaw = 0;
    public float pitch = 0;
    public float roll = 0;
    // public Mesh objectToCreate;

    public GameObject ToggleX;
    public GameObject ToggleY;

    public string NameOfFile = "A";
    void Start()
    {

        ColorXstart.GetComponent<Image>().color = colorOfXstart;
        ColorYstart.GetComponent<Image>().color = colorOfYstart;
        ColorXend.GetComponent<Image>().color = colorOfXend;
        ColorYend.GetComponent<Image>().color = colorOfYend;

        myButton = GetComponent<Button>();
        myButton.onClick.AddListener(() => { myFunctionForOnClickEvent(); });
        SliceCreator.listX = new List<Slice>();
        SliceCreator.listY = new List<Slice>();
        yaw = getRot(0).x;
        pitch = getRot(0).y;
        roll = getRot(0).z;
        AuxiliaryPlanes.CreateAuxiliaryPlanes(new Vector3[4] {
            Quaternion.Euler(yaw+90, pitch, roll)  * new Vector3(-0.5f,0,-0.5f),
            Quaternion.Euler(yaw+90, pitch, roll) * new Vector3(-0.5f,0,0.5f),
            Quaternion.Euler(yaw+90, pitch, roll) * new Vector3(0.5f,0,0.5f),
            Quaternion.Euler(yaw+90, pitch, roll) * new Vector3(0.5f,0,-0.5f)
            }, "somethingA", new Vector3(0, 0f, 0), colorOfXstart, ref AuxiliaryPlanes.X, transform);
        yaw = getRot(1).x;
        pitch = getRot(1).y;
        roll = getRot(1).z;
        AuxiliaryPlanes.CreateAuxiliaryPlanes(new Vector3[4] {
            Quaternion.Euler(yaw, pitch, roll) * new Vector3(-0.5f,0,-0.5f),
            Quaternion.Euler(yaw, pitch, roll) * new Vector3(-0.5f,0,0.5f),
            Quaternion.Euler(yaw, pitch, roll) * new Vector3(0.5f,0,0.5f),
            Quaternion.Euler(yaw, pitch, roll) * new Vector3(0.5f,0,-0.5f)
            }, "somethingB", new Vector3(0, 0f, 0), colorOfYstart, ref AuxiliaryPlanes.Y, transform);
        objectNaSpracovanie = GameObject.Find("objektNaSpracovanie");

    }
    /// <summary>
    /// Vráti hodnotu 
    /// <para>//Bude treba upravit</para>
    /// </summary>
    /// <param name="v">
    /// <para>0 - X</para>
    /// <para>1 - Y</para>
    /// </param>
    /// <returns>Hodnoty slider-ov pre danú os</returns>
    private Vector3 getRot(int v)
    {
        return new Vector3(
            GameObject.Find("Slider " + (v == 0 ? "X" : "Y")).GetComponent<Slider>().value,
            GameObject.Find("Slider (1) " + (v == 0 ? "X" : "Y")).GetComponent<Slider>().value,
            GameObject.Find("Slider (2) " + (v == 0 ? "X" : "Y")).GetComponent<Slider>().value
        );
    }
    /// <summary>
    /// Objekt podla ktoreho sa budu tvorit prierezy
    /// </summary>
    public GameObject objectNaSpracovanie;
    /// <summary>
    /// Zmaže vytvorené prierezy
    /// <para>Ak je volané pocas generovania tak prerusi generovanie </para>
    /// </summary>
    public void Clear()
    {
        for (int i = 0; i < SliceCreator.listX.Count; i++)
        {
            SliceCreator.listX[i].Destroy();
            Destroy(SliceCreator.listX[i].gameobject);
        }
        for (int i = 0; i < SliceCreator.listY.Count; i++)
        {
            SliceCreator.listY[i].Destroy();
            Destroy(SliceCreator.listY[i].gameobject);
        }
        SliceCreator.listX.Clear();
        SliceCreator.listY.Clear();
        for (int i = 0; i < lineList.Count; i++)
        {
            Destroy(lineList[i]);
        }

        lineList.Clear();

        //ak sa vykona pocas generovania tak treba zresetovat aj hodnoty
        SliceCreator.stop = true;
        GenerateID = 5;
    }
    internal static void DelSliceAt(List<Slice> list, int at)
    {
        list[at].Destroy();
        Destroy(list[at].gameobject);
        list.RemoveAt(at);
    }
    /// <summary>
    /// Funkcia ktorá je zavolaná po stisknutí tlačítka generovania
    /// </summary>
    void myFunctionForOnClickEvent()
    {

        MainFunction();

        //povolime save a reset button
        GameObject.Find("ResetButton").GetComponent<Button>().interactable = true;
        GameObject.Find("ShowSaveButton").GetComponent<Button>().interactable = true;

    }

    public GameObject ShiftXGO;
    public GameObject ShiftYGO;
    public GameObject Span_CountXGO;
    public GameObject Span_CountYGO;
    public GameObject countXGO;
    public GameObject countYGO;
    private float spanX=0.5f;
    private float spanY=0.5f;

    /// <summary>
    /// ID akcie pre generovanie, 
    /// <para>0 tak sa zacne generovanie</para>
    /// <para>1 generovanie v X hore</para>
    /// <para>2 generovanie v X dole</para>
    /// <para>3 generovanie v Y hore</para>
    /// <para>4 generovanie v Y dole</para>
    /// <para>>4 koniec generovania</para>
    /// </summary>
    private int GenerateID = 10;
    /// <summary>
    /// Začatie generovania prierezov
    /// </summary>
    void MainFunction() {
        Debug.Log("Zaciatok spracovania");

        Clear();


        /*load object*/

        if (loaded) {

            timeEnable = true;
            //UpdateCount_Span();



            Debug.Log("generovanie X");
            GenerateID = 0;

        }
        else
        {

            Debug.Log("Nebol nacitany ziadny objekt");
        }
    }

    DateTime StartTimeOfGenerate = DateTime.Now;
    float shiftA = 1;
    float shiftB = 1;
    void UpdateCount_Span()
    {
        //nastavime posuv na 0 ak by bol zadany span
        float shiftX = 0;
        float shiftY = 0;
        if (Span_CountXGO.GetComponent<Slider>().value == 0)
        {
            spanX = float.Parse(GameObject.Find("countX").GetComponent<Text>().text);
        }
        else
        {
            if (loaded)
            {
                spanX = SpanCalculator.CalculateSpan(objectNaSpracovanie.GetComponent<MeshFilter>().mesh.vertices, int.Parse(countXGO.GetComponent<Text>().text), AuxiliaryPlanes.X.GetComponent<MeshFilter>().mesh.vertices, out shiftX);
            }
            else
            {
                spanX = 0.5f;
            }
        }
        if (Span_CountYGO.GetComponent<Slider>().value == 0)
        {
            spanY = float.Parse(GameObject.Find("countY").GetComponent<Text>().text);
        }
        else
        {
            if (loaded)
            {
                spanY = SpanCalculator.CalculateSpan(objectNaSpracovanie.GetComponent<MeshFilter>().mesh.vertices, int.Parse(countYGO.GetComponent<Text>().text), AuxiliaryPlanes.Y.GetComponent<MeshFilter>().mesh.vertices, out shiftY);
            }
            else
            {
                spanY = 0.5f;
            }
        }
        //nastavime posuv
        shiftA = Shift.SetShift(spanX, ShiftXGO.GetComponent<Slider>().value / 100f);
        shiftB = Shift.SetShift(spanY, ShiftYGO.GetComponent<Slider>().value / 100f);
        SliceCreator.SetShift(shiftX, shiftY);
        updateColor();
    }
    void updateColor()
    {
        if (colorOfXstart != ColorXstart.GetComponent<Image>().color||
        colorOfYstart != ColorYstart.GetComponent<Image>().color||
        colorOfXend != ColorXend.GetComponent<Image>().color||
        colorOfYend != ColorYend.GetComponent<Image>().color)
        {

            colorOfXstart = ColorXstart.GetComponent<Image>().color;
            colorOfYstart = ColorYstart.GetComponent<Image>().color;
            colorOfXend = ColorXend.GetComponent<Image>().color;
            colorOfYend = ColorYend.GetComponent<Image>().color;

            //update color of all slices
            SliceColor.SetColorOfSlices(SliceCreator.listX, colorOfXstart, colorOfXend);
            SliceColor.SetColorOfSlices(SliceCreator.listY, colorOfYstart, colorOfYend);

            //update color of guide planes
            AuxiliaryPlanes.X.GetComponent<Renderer>().material.color=colorOfXstart;
            AuxiliaryPlanes.Y.GetComponent<Renderer>().material.color=colorOfYstart;
        }
    }
    float t = 0f;
    // Update is called once per frame
    void Update() {
        
        if (loaded)
        {
            myButton.interactable = true;
        }


        if (timeEnable)
        {

            t += Time.deltaTime;
            //timer.GetComponent<Text>().text = "Time elapsed: " + t.ToString() + "s.";
            timeEnable = false;
        }
        UpdateCount_Span();
        UpdateAuxiliaryPlanes();

        ///vykreslenie normaly v ktorej sa spajaju
        //Vector3 XNormal = Slice.getNormal(AuxiliaryPlanes.X.GetComponent<MeshFilter>().mesh.vertices);
        //Vector3 YNormal = Slice.getNormal(AuxiliaryPlanes.Y.GetComponent<MeshFilter>().mesh.vertices);
        //var returnedVector = Vector3.Cross(XNormal, YNormal);
        //Debug.DrawLine(Vector3.zero, returnedVector * 2);

        UpdateGenerate();

        if (Input.GetKeyDown(KeyCode.Q) && (Input.GetKey(KeyCode.LeftControl) || Input.GetKey(KeyCode.RightControl)))
        {
            Slice.CreateLines = !Slice.CreateLines;
        }

    }
    

    Vector3[] AuxiliaryPlanePoints = new Vector3[8] {
            new Vector3(-0.5f,0,-0.5f),
            new Vector3(-0.5f,0,0.5f),
            new Vector3(0.5f,0,0.5f),
            new Vector3(0.5f,0,-0.5f),
            new Vector3(-0.5f,0,-0.5f),
            new Vector3(-0.5f,0,0.5f),
            new Vector3(0.5f,0,0.5f),
            new Vector3(0.5f,0,-0.5f)
        };
void UpdateAuxiliaryPlanes()
    {
        yaw = getRot(0).x;
        pitch = getRot(0).y;
        roll = getRot(0).z;
        Vector3 s = Vector3.zero;
        Vector3[] APP = new Vector3[8];
        Vector3 normal= Slice.getNormal(AuxiliaryPlanePoints).normalized;
        for (int i = 0; i < AuxiliaryPlanePoints.Length; i++)
        {
            APP[i].x = AuxiliaryPlanePoints[i].x + (normal.x * shiftA);
            APP[i].y = AuxiliaryPlanePoints[i].y + (normal.y * shiftA);
            APP[i].z = AuxiliaryPlanePoints[i].z + (normal.z * shiftA);
        }
        for (int i = 0; i < AuxiliaryPlanePoints.Length; i++)
        {
            APP[i] = Quaternion.Euler(yaw + 90, pitch, roll) * APP[i];
        }
        AuxiliaryPlanes.X.GetComponent<MeshFilter>().mesh.vertices = APP;
        yaw = getRot(1).x;
        pitch = getRot(1).y;
        roll = getRot(1).z;

        for (int i = 0; i < AuxiliaryPlanePoints.Length; i++)
        {
            APP[i] = Quaternion.Euler(yaw, pitch, roll) * AuxiliaryPlanePoints[i];
        }
        normal = Slice.getNormal(APP);
        for (int i = 0; i < AuxiliaryPlanePoints.Length; i++)
        {
            APP[i].x = APP[i].x + (normal.x * shiftB);
            APP[i].y = APP[i].y + (normal.y * shiftB);
            APP[i].z = APP[i].z + (normal.z * shiftB);
        }

        AuxiliaryPlanes.Y.GetComponent<MeshFilter>().mesh.vertices = APP;

    }
    DateTime StartOfGenerate;
    void UpdateGenerate()
    {
        if (SliceCreator.stop && GenerateID < 6)
        {
            if (GenerateID == 0)
            {
                Test.Reset();
                StartTimeOfGenerate = DateTime.Now;
                //zapnutie zobrazovania prierezov
                ToggleX.GetComponent<Toggle>().isOn = true;
                ToggleY.GetComponent<Toggle>().isOn = true;
            }
            //skoncilo sa generovanie v danej ose
            GenerateID++;
            Sliceindex = 0;
        }
        switch (GenerateID)
        {
            case 1://generate in X up
                Test.NewLine();
                StartOfGenerate =DateTime.Now;
                SliceCreator.CreateSlices(0, spanX, true, spanY, Sliceindex, transform, ref objectNaSpracovanie, ref lineList);
                Test.NewData("Generate", "ID", GenerateID, Sliceindex, "Ticks", (DateTime.Now - StartOfGenerate).Ticks);
                Sliceindex++;
                writeTimeSpan();
                break;
            case 2://generate in X down
                Sliceindex--;
                Test.NewLine();
                StartOfGenerate = DateTime.Now;
                SliceCreator.CreateSlices(0, spanX, false, spanY, Sliceindex, transform, ref objectNaSpracovanie, ref lineList);
                Test.NewData("Generate", "ID", GenerateID, Sliceindex, "Ticks", (DateTime.Now - StartOfGenerate).Ticks);
                writeTimeSpan();
                break;
            case 3://generate in Y up
                Test.NewLine();
                StartOfGenerate = DateTime.Now;
                SliceCreator.CreateSlices(1, spanY, true, spanX, Sliceindex, transform, ref objectNaSpracovanie, ref lineList);
                Test.NewData("Generate", "ID", GenerateID, Sliceindex, "Ticks", (DateTime.Now - StartOfGenerate).Ticks);
                Sliceindex++;
                writeTimeSpan();
                break;
            case 4://generate in Y down
                Sliceindex--;
                Test.NewLine();
                StartOfGenerate = DateTime.Now;
                SliceCreator.CreateSlices(1, spanY, false, spanX, Sliceindex, transform, ref objectNaSpracovanie, ref lineList);
                Test.NewData("Generate", "ID", GenerateID, Sliceindex, "Ticks", (DateTime.Now - StartOfGenerate).Ticks);
                writeTimeSpan();
                break;
            case 5://postprocess
                //remove noncolliding slices
                bool a = true, b = true;
                while (a || b)
                {
                    a = RemoveSlice.RemoveNotCollidingSlices(SliceCreator.listX, SliceCreator.listY);
                    b = RemoveSlice.RemoveNotCollidingSlices(SliceCreator.listY, SliceCreator.listX);
                }
                //creating notches
                StartOfGenerate = DateTime.Now;
                NotchesCreator.CreateNotches(SliceCreator.listX, SliceCreator.listY);
                //when notches are created, we must again triangulate slices
                //Triangulate(SliceCreator.listX,SliceCreator.listY);
                Test.NewLine();
                Test.NewData("Creating notches end", "ID", GenerateID, "Ticks", (DateTime.Now - StartOfGenerate).Ticks);
                #region createNotchesOld
                /*for (int i = 0; i < SliceCreator.listX.Count; i++)
                {
                    NotchesCreator.createNotchesOLD(SliceCreator.listX[i].listOfPoints, new Vector3[3] {
                            SliceCreator.GetPoint(1,0),
                            SliceCreator.GetPoint(1,1),
                            SliceCreator.GetPoint(1,2)
                        }, spanY, 0, true,
                            SliceCreator.shiftY);
                    NotchesCreator.createNotchesOLD(SliceCreator.listX[i].listOfPoints, new Vector3[3] {
                            SliceCreator.GetPoint(1,0),
                            SliceCreator.GetPoint(1,1),
                            SliceCreator.GetPoint(1,2)
                        }, spanY, 0, false,
                             SliceCreator.shiftY);
                    SliceCreator.listX[i].destroy();
                    SliceCreator.listX[i].AddLines();
                }*/
                #endregion createNotchesOld
                writeTimeSpan();
                Test.Print();
                break;
            default:
                break;
        }

        //nastavenie farby pre jednotlivé prierezy
        if (GenerateID < 6)
        {
            Debug.Log(GenerateID + " index:" + Sliceindex);
            SliceColor.SetColorOfSlices(SliceCreator.listX, colorOfXstart, colorOfXend);
            SliceColor.SetColorOfSlices(SliceCreator.listY, colorOfYstart, colorOfYend);
        }
    }

    private void Triangulate(List<Slice> listX, List<Slice> listY)
    {
        for (int i = 0; i < listX.Count; i++)
        {
            listX[i].Triangulate();
        }
        for (int i = 0; i < listY.Count; i++)
        {
            listY[i].Triangulate();
        }
    }

    void writeTimeSpan()
    {
        TimeSpan r = DateTime.Now - StartTimeOfGenerate;
        if (r.Hours > 0)
        {
            timer.GetComponent<Text>().text = String.Format("{0}h {1:00}m {2:00}s {3:00}ms", r.Hours, r.Minutes, r.Seconds, r.Milliseconds);//dd\.hh\:mm\:ss");
        }
        else
        {
            timer.GetComponent<Text>().text = String.Format("{0:00}m {1:00}s {2:000}ms", r.Minutes, r.Seconds, r.Milliseconds);//hh\:mm\:ss");
        }
    }
    bool timeEnable = false;
    private List<GameObject> lineList = new List<GameObject>();
    

    
    

    


    
     
    public static float DistanceToLine(Ray ray, Vector3 point)
    {
        return Vector3.Cross(ray.direction, point - ray.origin).magnitude;
    }
    /// <summary>
    /// Index prierezu,
    /// <para>pred generovanim sa vynuluje,</para>
    /// <para>pri generovani sa zvyšuje</para>
    /// </summary>
    int Sliceindex;





    /// <summary>
    /// Testing points sorter
    /// </summary>
    void Test1()
    {
        Debug.Log(SaveScript.toHex(15, 90,  240));
    }

        void Test2()
    {
            List<Vector3> a = new List<Vector3>();
        List<int> aI = new List<int>();
        a.Add( new Vector3(17,91,155));
        a.Add(new Vector3(9 , 51,83));
        a.Add(new Vector3(12, 66, 110));
        a.Add(new Vector3(13, 71, 119));
        a.Add(new Vector3(15, 81, 137));
        a.Add(new Vector3(3 , 21, 29));
        a.Add(new Vector3(5 , 31, 47));
        a.Add(new Vector3(7 , 41, 65));
        a.Add(new Vector3(10, 56, 92));
        a.Add(new Vector3(1 , 11, 11));

        aI.Add(1);
        aI.Add(2);
        aI.Add(3);
        aI.Add(4);
        aI.Add(5);
        aI.Add(6);
        aI.Add(7);
        aI.Add(8);
        aI.Add(9);
        aI.Add(10);
        PointsSorter.SortingVec3(ref a,ref aI);


        Debug.Log("a" + a.Count);
        foreach (var item in a)
        {
            Debug.Log(item);
        }
        Debug.Log("aI");
        foreach (var item in aI)
        {
            Debug.Log(item);
        }
        Debug.Log("end");

    }
    


}
