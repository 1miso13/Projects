/*
 Autor: Michal Ondrejó
 Project: Bachelor Thesis - Tvorba řezů z 3D modelů pro vystřihovánky "sliceforms"
*/
using UnityEngine;
using System.IO;
using System;
using UnityEngine.UI;
using UnityEngine.EventSystems;
using System.Collections.Generic;

public class FileExplorer : MonoBehaviour {
    public bool showFile = false;
    public string path, pathold;
    List<GameObject> fileExplorer;
    public GameObject FileExplorerPath;
    public GameObject OKButton;
    public GameObject Content;
    public static char delimiter;

    Platform platform;
    // Use this for initialization
    void Start () {
        PlatformID platformID = Environment.OSVersion.Platform;
        switch ((int) platformID)
        {
            case 4:
            case 6:
            case 128:
                platform = Platform.LINUX_MAC;
                delimiter = '/';
                break;
            default:
                platform = Platform.WINDOWS;
                delimiter = '\\';
                break;

        }



        path = "";
        pathold = ".";//musi byt rozdielne path a pathold
        fileExplorer = new List<GameObject>();
        FileExplorerPath.GetComponent<InputField>().text= path;
        FileExplorerPath.GetComponent<InputField>().onEndEdit.AddListener(delegate { SetPath(FileExplorerPath.GetComponent<InputField>()); });
    }
    
    private void SetPath(InputField mainInputField)
    {
        if(platform == Platform.WINDOWS)
            mainInputField.text.Replace('/','\\');
        if (Directory.Exists(mainInputField.text))
        {
            path = mainInputField.text + (mainInputField.text[mainInputField.text.Length - 1] == delimiter ? "": delimiter.ToString());
            selected = -1;
        }
        else
        {
            if (showFile && File.Exists(mainInputField.text))
            {
                path = mainInputField.text;
                DirectoryInfo dir = new DirectoryInfo(path.Substring(0,path.LastIndexOf(delimiter)));
                DirectoryInfo[] subdir = dir.GetDirectories();
                FileInfo[] info = dir.GetFiles("*.obj");
                int i = 0;
                for (; i < info.Length; i++)
                {
                    if (info[i].Name==path.Substring(path.LastIndexOf(delimiter) +1))
                    {
                        break;
                    }
                }
                //oznacime zadany subor
                selected = subdir.Length/*pocet zloziek*/+(i+1)/*zaciname od 0*/+1/*...*/;
            }
            else
            {
                mainInputField.text = path;
            }
        }
    }
    bool refresh=false;
    // Update is called once per frame
    void Update () {
        if (OKButton!=null)
        {
            if (File.Exists(path))
            {
                OKButton.GetComponent<Button>().interactable = true;
            }
            else
            {
                OKButton.GetComponent<Button>().interactable = false;
            }
        }
        if (EventSystem.current.currentSelectedGameObject!=null/* && EventSystem.current.currentSelectedGameObject!=selectedOld*/)
        {
            string s;
            //Debug.Log(EventSystem.current.currentSelectedGameObject);
            if ((s=EventSystem.current.currentSelectedGameObject.name).Substring(0,3)== "FE:")
            {
                //selectedOld = EventSystem.current.GetComponent<EventSystem>().currentSelectedGameObject;
                EventSystem.current.SetSelectedGameObject(null);
                string a=s.Substring(3);
                int d;
                string qwerty= a.Substring(0,a.IndexOf(':'));
                Debug.Log(qwerty+" "+selected+";");
                bool selectedFile = false;
                bool k = true;
                if ((d = int.Parse(qwerty)) != selected)
                {
                    k = false;
                    selected = d;
                    Debug.Log(qwerty + " A " + selected);
                    refresh = true;
                    //Debug.Log("qwerty :"+ (File.Exists(path) ? path.Substring(0, path.LastIndexOf('\\')) : path)  + a.Substring(a.IndexOf(':') + 1));
                    if (File.Exists((File.Exists(path) ? path.Substring(0,path.LastIndexOf(delimiter)) : path) + delimiter + a.Substring(a.IndexOf(':') + 1)))
                    {
                        selectedFile = true;
                    }
                }
                if (selectedFile || k)
                {
                    selectedFile = false;
                    s = a.Substring(a.IndexOf(':') + 1);
                    Debug.Log(s);
                    if (s == "...")
                    {
                        selected = -1;
                        path = path.Substring(0, path.Length - 2);
                        if (path.Contains(delimiter.ToString()))
                        {//sme v priecinku disku
                            int index = path.LastIndexOf(delimiter);
                            if (index > 0)
                                path = path.Substring(0, index + 1);
                            if (!path.Contains(delimiter.ToString()))
                            {
                                path = "";
                            }
                        }
                        else
                        {
                            //dostavame sa na vypis diskov
                            path = "";
                        }

                    }
                    else
                    {
                        if (path == "")
                        {
                            selected = -1;
                            //disky
                            path = s;
                        }
                        else
                        {//ostatne
                            string path1 = path + s;
                            try
                            {
                                if (Directory.Exists(path1))
                                {
                                    selected = -1;
                                    path1 += delimiter;
                                    new DirectoryInfo(path1).GetDirectories();//testuje sa ci je pristup do zlozky
                                }
                                if (File.Exists(path))
                                {
                                    int index = path.LastIndexOf(delimiter);
                                    if (index > 0)
                                        path1 = path.Substring(0, index + 1) + s;
                                }
                                path = path1;
                            }
                            catch
                            {
                                Debug.Log("Access denied");
                            }

                        }

                    }
                }
            }
        }
        if (path != pathold || refresh)
        {
            refresh = false;
            FileExplorerPath.GetComponent<InputField>().text = path;
            //najprv musime vymazat vsetky
            for (int i = 0; i < fileExplorer.Count; i++)
            {
                Destroy(fileExplorer[i].gameObject);
            }
            fileExplorer.Clear();
            try
            {

                if (Directory.Exists(path) || File.Exists(path) || path =="")
                {//test ci je pristupne

                    pathold = path;
                }
                else
                {
                    path = pathold;
                }
            }
            catch
            {
                path = pathold;
                Debug.Log("chybna adresa");
            }
            if (path != "")
            {
                string dirPath=path;
                if (File.Exists(path))
                {
                    int index = path.LastIndexOf(delimiter);
                    if (index > 0)
                        dirPath = path.Substring(0, index);
                }
                DirectoryInfo dir = new DirectoryInfo(dirPath);
                DirectoryInfo[] subdir = dir.GetDirectories();
                FileInfo[] info = dir.GetFiles("*.obj");
                int count = 1 + subdir.Length + (OKButton != null ? info.Length : 0);
                //nastavime velkost okna
                Content.GetComponent<RectTransform>().sizeDelta=new Vector2(/*GameObject.Find("FileExplorerContent")*/Content.GetComponent<RectTransform>().sizeDelta.x, count * 30f);
            
                string text = "...";
                CreateButton(text,true);
                foreach (DirectoryInfo d in subdir)
                {
                    CreateButton(d.Name, false);
                }
                if (OKButton != null)
                {

                    foreach (FileInfo f in info)
                    {
                        CreateButton(f.Name, true);
                    }
                }
            }
            else
            {

                //vypiseme jednotlive disky
                string[] drives = Directory.GetLogicalDrives();
                int count = drives.Length+2;
                //nastavime velkost okna
                Content.GetComponent<RectTransform>().sizeDelta = new Vector2(/*GameObject.Find("FileExplorerContent")*/Content.GetComponent<RectTransform>().sizeDelta.x, count * 30f);

                //CreateButton(Environment.GetFolderPath(Environment.SpecialFolder.Desktop)+"\\", false);
                /*string history = Environment.GetFolderPath(Environment.SpecialFolder.History);
                if (Directory.Exists(history))
                {
                    CreateButton(history + "\\", false);
                }*/
                CreateButton(Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments) + delimiter, false);
                CreateButton(Environment.GetFolderPath(Environment.SpecialFolder.DesktopDirectory) + delimiter, false);
                foreach (string d in drives)
                {
                    CreateButton(d, false);
                }
            }
        }

    }
    void method()
    {
       // path = path + "/" + ;
    }
    int selected=-1;
    void CreateButton(string text, bool alignment)
    {
        GameObject button = new GameObject();
        fileExplorer.Add(button);
        button.transform.parent = Content.transform;
        float PositionY = (Content.GetComponent<RectTransform>().sizeDelta.y / 2) - (fileExplorer.Count * 30)+15;
        button.transform.localPosition = new Vector2(10, PositionY);// GameObject.Find("FileExplorerContent").transform.position;
        button.AddComponent<RectTransform>();
        button.AddComponent<Button>();
        //
        //button.transform.position = Vector3.zero;
        //gameObject.transform.SetParent(gameObject.transform, false);
        button.GetComponent<RectTransform>().sizeDelta = (new Vector2(gameObject.GetComponent<RectTransform>().sizeDelta.x, 30));
        button.GetComponent<RectTransform>().localScale = new Vector3(1,1,1);
        button.GetComponent<Button>().onClick.AddListener(method);
        Image image = button.AddComponent<Image>();
        image.sprite = (Sprite)Resources.Load("UI/Skin/UISprite.psd");
        if (alignment)
        {
            image.color = new Color(0.9f, 0.9f, 0.9f, 0.8f);
        }
        else
        {
            Color a = image.color;
            a.a = 0.8f;
            image.color = a;
        }
        if (fileExplorer.Count == selected)
        {
            image.color = Color.green;
        }
        button.name = "FE:"+ fileExplorer.Count+":" + text;
        GameObject t = new GameObject();
        t.AddComponent<Text>();
        //t.transform.parent = button.transform;
        t.transform.SetParent(button.transform);
        t.transform.position = button.transform.position;
        t.GetComponent<RectTransform>().localScale = new Vector3(1, 1, 1);
        //t.AddComponent<RectTransform>();
        t.GetComponent<RectTransform>().sizeDelta = (new Vector2(gameObject.GetComponent<RectTransform>().sizeDelta.x, 30));
        button.GetComponentInChildren<Text>().text = "   "+ (alignment?"      ":"") +text;
        button.GetComponentInChildren<Text>().color = Color.black;
        button.GetComponentInChildren<Text>().font = Resources.GetBuiltinResource<Font>("Arial.ttf");
        button.GetComponentInChildren<Text>().alignment = TextAnchor.MiddleLeft;

        //button.GetComponentInChildren<Text>().alignment = TextAnchor.MiddleCenter;


    }
    enum Platform
    {
        WINDOWS,
        LINUX_MAC
    }
}
