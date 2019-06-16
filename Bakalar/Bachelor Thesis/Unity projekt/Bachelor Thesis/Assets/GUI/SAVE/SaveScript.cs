/*
 Autor: Michal Ondrejó
 Project: Bachelor Thesis - Tvorba řezů z 3D modelů pro vystřihovánky "sliceforms"
*/
using UnityEngine;
using UnityEngine.UI;
using System.Collections.Generic;
using System.IO;
using System;
using System.Linq;
//using sharpPDF;
//using sharpPDF.Enumerators;
using System.Collections;
using Assets.generovanie_prierezov;
using iTextSharp;
using iTextSharp.text;
using iTextSharp.text.pdf;
public class SaveScript : MonoBehaviour {

    // Use this for initialization
    public GameObject NazovSuboru;
    Button myButton;
    public GameObject generovanie;
    float nasobok = 1;
    string path;
    bool takeScreenshot = false;
    public GameObject canvas;
    float maxSize =2048f;
    public GameObject selectFormatDropdown;
    public GameObject panel;
    public GameObject setColorable;

    public GameObject PDFStrokeColorGO;
    float s5;
   // bool Xshift=false; //na X/Y bude aplikovany posuv na X/Y 
    void Start () {
        NazovSuboru.GetComponent<InputField>().text = generovanie.GetComponent<generate>().NameOfFile+"-Sliceforms";
        myButton = GetComponent<Button>();
        myButton.onClick.AddListener(() => { myFunctionForOnClickEvent(); });
        PDFStrokeColorGO.GetComponent<UnityEngine.UI.Image>().color=Color.black;
    }
    bool takescreenshot2 = false;
    bool takescreenshot3 = false;
    IEnumerator takescreenshot()
    {

        yield return new WaitForEndOfFrame();
    }
    // Update is called once per frame
    void Update () {
        myButton.interactable = GameObject.Find("FileExplorerScrollview").GetComponent<FileExplorer>().path!="";

        if (takescreenshot3)
        {
            takescreenshot();
            Application.CaptureScreenshot(path + "/"+ NazovSuboru.GetComponent<InputField>().text + ".png");
            canvas.GetComponent<Canvas>().enabled = true;
            //canvas.SetActive(true);
            takescreenshot3 = false;
            //znovu skryjeme okno
            panel.SetActive(!panel.activeSelf);
        }
        if (takescreenshot2)
        {
            takescreenshot2 = false;
            takescreenshot3 = true;
        }
        if (takeScreenshot)
        {
            Debug.Log("Hotovo");
            canvas.GetComponent<Canvas>().enabled = false;
            //canvas.SetActive(false);
            takeScreenshot = false;
            takescreenshot2 = true;
            nasobok = 1;
        }
        
    }
    List<Slice> listY;
    List<Slice> listX;
    void myFunctionForOnClickEvent()
    {
        path=GameObject.Find("FileExplorerScrollview").GetComponent<FileExplorer>().path;
        //path=EditorUtility.SaveFolderPanel("Select folder to save pictures...", Environment.GetFolderPath(System.Environment.SpecialFolder.Desktop),"");
        if (path!="")
        {

            generovanie = GameObject.Find("GenerovaniePrierezov");
            listX = SliceCreator.listX;
            listY = SliceCreator.listY;
            var X = convertFrom3Dto2D(listX);
            var Y = convertFrom3Dto2D(listY);
           // List<Texture2D> TexList=new List<Texture2D>();
            Vector2 s1 = getSize(X);
            Vector2 s2 = getSize(Y);
            float s3 = (s2.x > s2.y ? s2.x : s2.y);
            float s4 = (s1.x > s1.y ? s1.x : s1.y);
            s5 = (s3 > s4 ? s3 : s4);
            nasobok = (maxSize) / s5;
            Debug.Log("nasobok:"+nasobok +" : "+ s1);
            s1 *= nasobok;
            s2 *= nasobok;

            //v akom formate sa ma ulozit
            switch (selectFormatDropdown.GetComponent<Dropdown>().value)
            {
                case 0: //pdf
                    createPDF(X, Y, s1, s2);
                    break;
                case 1: //jpg
                    createPNGorJPG(X, s1, false, true);
                    createPNGorJPG(Y, s2, false, false);
                    break;
                case 2: //svg
                    createSVG(X, s1, true);
                    createSVG(Y, s2, false);
                    break;
                case 3: //png
                    createPNGorJPG(X, s1, true, true);
                    createPNGorJPG(Y, s2, true, false);
                    break;
            }
            X = null;
            Y = null;
            s5 = 0;
            // Texture2D
            takeScreenshot = true;
            ObjExporter objexporter = new ObjExporter();
            
            for (int i = 0; i < listX.Count; i++)
            {
                objexporter.AddSlice(
                    listX[i].gameobject.GetComponent<MeshFilter>().mesh.vertices.ToList(),
                    listX[i].gameobject.GetComponent<MeshFilter>().mesh.triangles,
                    "X" + i);
            }
            for (int i = 0; i < listY.Count; i++)
            {
                objexporter.AddSlice(listY[i].gameobject.GetComponent<MeshFilter>().mesh.vertices.ToList()
                    , listY[i].gameobject.GetComponent<MeshFilter>().mesh.triangles
                    , "Y" + i);
            }
            objexporter.Export(path, NazovSuboru.GetComponent<InputField>().text);
            
        }
    }
    void createPDFt(Document myDoc, PdfContentByte cb, List<List<Vector2>> x, Vector2 velkostDokumentu, Vector2 countOfSlicesOnPageV, Vector2 sizeOfSlices, bool isX, string text="")
    {
        int countOfSlicesOnPage = (int)(countOfSlicesOnPageV.x * countOfSlicesOnPageV.y);
        int ostava = countOfSlicesOnPage;
        //pdfDocument doc = new pdfDocument("","");
        //pdfPage myPage = myDoc.addPage((int)velkostDokumentu.y, (int)velkostDokumentu.x);
        myDoc.NewPage();

        int m = 0, n = (int)countOfSlicesOnPageV.y - 1;
        //Debug.Log("s5:" + s5);
        //Debug.Log("a:" + a);
        s5 = a;
        //Debug.Log("s52:" + s5);

        for (int i = 0; i < x.Count; i++)
        {//jednotlive prierezy
            //Debug.Log("Prierez:" + i);
            bool colorable = setColorable.GetComponent<Toggle>().isOn;
            ///pdfColor color;
            Color c=Color.black;
            Color strokecolor = PDFStrokeColorGO.GetComponent<UnityEngine.UI.Image>().color;
            BaseColor strokeBaseColor = new BaseColor(strokecolor.r, strokecolor.g, strokecolor.b);
            cb.SetColorStroke(strokeBaseColor);
            
            
            Vector4 size = getSize(x[i])/nasobok;
            
            if (m >= countOfSlicesOnPageV.x)
            {
                m = 0;
                n--;
            }
            if (ostava == 0)//ak je vecsie
            {
                //myPage = myDoc.addPage((int)velkostDokumentu.y, (int)velkostDokumentu.x);
                myDoc.NewPage();
                cb.SetColorStroke(strokeBaseColor);
                cb.Rectangle(0, 0, (int)velkostDokumentu.x, (int)velkostDokumentu.y);
                cb.Stroke();
                //myPage.drawRectangle(0,0, (int)velkostDokumentu.x, (int)velkostDokumentu.y, new pdfColor(1, 1, 1), new pdfColor(255, 255, 255), 1, predefinedLineStyle.csNormal);
                ostava = countOfSlicesOnPage;
                m = 0;
                n = (int)countOfSlicesOnPageV.y-1;
            }

            ///myPage.drawRectangle((int)(m * sizeOfSlices.x), (int)(n * sizeOfSlices.y), (int)((m + 1) * sizeOfSlices.x), (int)((n + 1) * sizeOfSlices.y), new pdfColor(1, 1, 1), new pdfColor(255, 255, 255), 1, predefinedLineStyle.csNormal);
            cb.Rectangle((m * sizeOfSlices.x), (int)(n * sizeOfSlices.y), (int)((m + 1) * sizeOfSlices.x), (int)((n + 1) * sizeOfSlices.y));
            cb.Stroke();
            //vypis cisla prierezu
            if (true)
                AddText(cb,text + i.ToString(), (int)((m) * sizeOfSlices.x), (int)((n) * sizeOfSlices.y), 10, 2, 3);
            //myPage.addText(text+i.ToString(), (int)((m) * sizeOfSlices.x), (int)((n) * sizeOfSlices.y), myDoc.getFontReference("Helvetica"), 10);

            BaseColor FillColor=BaseColor.WHITE;
            if (colorable)
            {
                c = (isX ? listX : listY)[i].gameobject.GetComponent<Renderer>().material.color;
                cb.SetRGBColorFill((int)(c.r * 255), (int)(c.g * 255), (int)(c.b * 255));
                FillColor = new BaseColor((int)(c.r * 255), (int)(c.g * 255), (int)(c.b * 255));
            }
            else
            {
                cb.SetRGBColorFill(255,255,255);
            }

            #region drawSlice
            //float x2;
            //float y2;
            //float x1 = -(x[i][x[i].Count - 1].x) /*- shift.x*/;
            //float y1 = -(x[i][x[i].Count - 1].y) /*- shift.y*/;

            //for (int j = 0; j < x[i].Count; j++)
            //{//jednotlive casti prierezu
            //    //Debug.Log("Cast:" + j);
            //    float x2old = x2 = -(x[i][j].x) /*- shift.x*/;
            //    float y2old = y2 = -(x[i][j].y)/* - shift.y*/;

            //    x1 -= size.z - size.x/2 ;
            //    x2 -= size.z - size.x/2 ;
            //    y1 -= size.w - size.y/2 ;
            //    y2 -= size.w - size.y/2 ;

            //    float resize = 25 / 20;
            //    //zmensenie
            //    x1 /= (s5 * resize);
            //    y1 /= (s5 * resize);
            //    x2 /= (s5 * resize);
            //    y2 /= (s5 * resize);
            //    //zvacsenie
            //    x1 *= (sizeOfSlices.x);
            //    y1 *= (sizeOfSlices.y);
            //    x2 *= (sizeOfSlices.x);
            //    y2 *= (sizeOfSlices.y);
            //    //shift = Vector2.zero;
            //    //posuv
            //    x1 += (m * sizeOfSlices.x + sizeOfSlices.x/2 );
            //    y1 += (n * sizeOfSlices.y + sizeOfSlices.y/2 );
            //    x2 += (m * sizeOfSlices.x + sizeOfSlices.x/2 );
            //    y2 += (n * sizeOfSlices.y + sizeOfSlices.y/2 );

            //    //Debug.Log("point1:" + x1 + ":" + y1);
            //    //Debug.Log("point2:" + x2 + ":" + y2);
            //    myPage.drawLine((int)x1, (int)y1, (int)x2, (int)y2, predefinedLineStyle.csNormal, color,1);
            //    x1 = x2old;
            //    y1 = y2old;
            //}
            #endregion

            #region Označenie každého zárezu podla rezu
            //Add Intersecting point index
            var d = (isX ? listX : listY)[i].GetIPoints();
            var intersectIndex = (isX ? listX : listY)[i].IntersectingIndex;
            /*Debug.Log("Slice: " + i);
            for (int z = 0; z < d.Count; z++)
            {
                Debug.Log(d[z] + " :" + intersectIndex[z]);
            }*/
            //treba preklopit z 3d do 2d 
            var d2d=TransformFrom3Dto2D(d, Slice.getNormal((isX? AuxiliaryPlanes.X : AuxiliaryPlanes.Y).GetComponent<MeshFilter>().mesh.vertices));
            #endregion
            DrawSlice(cb,x[i],size,sizeOfSlices,m,n,intersectIndex);
            if (colorable)
            {
               // PdfPatternPainter pattern = cb.CreatePattern(3,3   );
               // pattern.SetLineWidth(0.5);
               // pattern.SetColorFill(FillColor);
               // pattern.SetColorStroke(FillColor);
               // /*pattern.MoveTo(1, 0);
               // pattern.LineTo(1, 10);
               // pattern.MoveTo(0, 1);
               // pattern.LineTo(10, 1);*/
               // pattern.Rectangle(0.5,0.5,2,2);
               // pattern.FillStroke();
               ////S cb.SetPatternFill(pattern);

                cb.ClosePathFillStroke();
            }
            else
            {
                cb.ClosePathStroke();
            }
            cb.SetColorFill(strokeBaseColor);//Set color for index
            DrawIndexOfCollision(cb, x[i], size, sizeOfSlices, m, n, d2d, intersectIndex);
            ostava--;
            m++;

        }

    }

    private void DrawIndexOfCollision(PdfContentByte cb, List<Vector2> x, Vector4 size, Vector2 sizeOfSlices, int m, int n, List<Vector2> d, List<int> intersectIndex)
    {

        float resize = 25 / 20;
        #region vypis cisiel
        for (int k = 0; k < d.Count; k++)
        {
            var q = d[k];
            var p = intersectIndex[k];
            q.x += size.z - size.x / 2;
            q.y += size.w - size.y / 2;

            //zmensenie
            q.x /= (s5 * resize);
            q.y /= (s5 * resize);
            //zvacsenie
            q.x *= (sizeOfSlices.x);
            q.y *= (sizeOfSlices.y);
            //shift = Vector2.zero;
            //posuv
            q.x += (m * sizeOfSlices.x + sizeOfSlices.x / 2);
            q.y += (n * sizeOfSlices.y + sizeOfSlices.y / 2);

            //myPage.addText(p.ToString(), (int)q.x, (int)q.y, font, 6);
            AddText(cb, p.ToString(), (int)q.x, (int)q.y, 6);
        }



        #endregion vypis cisiel
        //myPage.addText(text + i.ToString(), (int)((m) * sizeOfSlices.x), (int)((n) * sizeOfSlices.y), myDoc.getFontReference("Helvetica"), 10);
        
    }
    private void AddText(PdfContentByte cb, string text, int positionX, int positionY,int size= 10, int offsetY = 2, int offsetX = 0)
    {
        cb.BeginText();
        BaseFont f_cn = BaseFont.CreateFont();
        cb.SetFontAndSize(f_cn, size);
        cb.SetTextMatrix(positionX+ offsetX, positionY + offsetY);  //(xPos, yPos)
        cb.ShowText(text);
        cb.EndText();
    }
    void DrawSlice(PdfContentByte cb,List<Vector2> x,Vector4 size,Vector2 sizeOfSlices,int m,int n, List<int> intersectIndex)
    {
        float x2;
        float y2;
        float x1 = (x[x.Count - 1].x) /*- shift.x*/;
        float y1 = (x[x.Count - 1].y) /*- shift.y*/;
        float resize = 25 / 20;
        bool first = true;
        for (int j = 0; j < x.Count; j++)
        {//jednotlive casti prierezu
         //Debug.Log("Cast:" + j);
            float x2old = x2 = (x[j].x) /*- shift.x*/;
            float y2old = y2 = (x[j].y)/* - shift.y*/;
            
#region some magic
            x1 += size.z - size.x / 2;
            x2 += size.z - size.x / 2;
            y1 += size.w - size.y / 2;
            y2 += size.w - size.y / 2;

            //zmensenie
            x1 /= (s5 * resize);
            y1 /= (s5 * resize);
            x2 /= (s5 * resize);
            y2 /= (s5 * resize);
            //zvacsenie
            x1 *= (sizeOfSlices.x);
            y1 *= (sizeOfSlices.y);
            x2 *= (sizeOfSlices.x);
            y2 *= (sizeOfSlices.y);
            //shift = Vector2.zero;
            //posuv
            x1 += (m * sizeOfSlices.x + sizeOfSlices.x / 2);
            y1 += (n * sizeOfSlices.y + sizeOfSlices.y / 2);
            x2 += (m * sizeOfSlices.x + sizeOfSlices.x / 2);
            y2 += (n * sizeOfSlices.y + sizeOfSlices.y / 2);
            #endregion some magic
            //Debug.Log("point1:" + x1 + ":" + y1);
            //Debug.Log("point2:" + x2 + ":" + y2);
            ///myPage.drawLine((int)x1, (int)y1, (int)x2, (int)y2, predefinedLineStyle.csNormal, color, 1);
            if (first)
            {
                cb.MoveTo(x1, y1);
                first = false;
            }
            cb.LineTo(x2, y2);
            x1 = x2old;
            y1 = y2old;
        }

    }
    public static string toHex(int r, int g, int b)
    {
        return "" + toHexH(r) + toHexH(g) + toHexH(b);
    }
    private static string toHexH(int color)
    {

        int r1 = color / 16;
        int r2 = (color % 16);
        string s = string.Empty;
        if (r1 < 10)
        {
            s += r1.ToString();
        }
        else
        {
            switch (r1)
            {
                case 10:
                    s += 'A';
                    break;
                case 11:
                    s += 'B';
                    break;
                case 12:
                    s += 'C';
                    break;
                case 13:
                    s += 'D';
                    break;
                case 14:
                    s += 'E';
                    break;
                case 15:
                    s += 'F';
                    break;
                default:
                    break;
            }
        }
        if (r2 < 10)
        {
            s += r2.ToString();
        }
        else
        {
            switch (r2)
            {
                case 10:
                    s += 'A';
                    break;
                case 11:
                    s += 'B';
                    break;
                case 12:
                    s += 'C';
                    break;
                case 13:
                    s += 'D';
                    break;
                case 14:
                    s += 'E';
                    break;
                case 15:
                    s += 'F';
                    break;
                default:
                    break;
            }
        }
        return s;
    }

    float a;
    private void createPDF(List<List<Vector2>> x, List<List<Vector2>> y, Vector2 s1, Vector2 s2)
    {
        Vector2 velkostDokumentu; //= new Vector2(2480, 3508);//A4
        Rectangle PSize;
        switch (GameObject.Find("DropdownSizeOfPaper").GetComponent<Dropdown>().value)
        {
            case 0:
                //velkostDokumentu = new Vector2(420, 595);//A5
                PSize = PageSize.A5;
                break;
            case 1:
               // velkostDokumentu = new Vector2(595, 842);//A4
                PSize = PageSize.A4;
                break;
            default:
                //velkostDokumentu = new Vector2(842, 1191);//A3
                PSize = PageSize.A3;
                break;
        }
        velkostDokumentu = new Vector2(PSize.Width, PSize.Height);
        //pdfDocument myDoc = new pdfDocument("Sliceforms", "Michal Ondrejo");
        Document document = new Document(PSize, 0, 0, 50, 50);
        string NameOfFile = path + (NazovSuboru.GetComponent<InputField>().text != "" ? NazovSuboru.GetComponent<InputField>().text : "SliceForms") + ".pdf";
        System.IO.FileStream fs = new FileStream(NameOfFile, FileMode.Create);
        PdfWriter writer = PdfWriter.GetInstance(document, fs);
        document.AddAuthor("Michal Ondrejó");
        document.AddCreator("Sliceforms");
        document.AddKeywords("Sliceforms");
        document.AddSubject("Created Sliceforms");
        document.AddTitle("Sliceforms");
        document.Open();

        
        //moznosti rozlozenia: 1x1,2x1,2x2,3x2,3x3,4x2,4x3,4x4,5x2,5x3,5x4,6x3,6x4
        Vector2 countOfSlicesOnPageV;
        switch (GameObject.Find("DropdownRozlozenie").GetComponent<Dropdown>().value)
        {
            case 0:
                countOfSlicesOnPageV = new Vector2(1, 1);
                break;
            case 1:
                countOfSlicesOnPageV = new Vector2(1, 2);
                break;
            case 2:
                countOfSlicesOnPageV = new Vector2(2, 3);
                break;
            case 3:
                countOfSlicesOnPageV = new Vector2(3, 4);
                break;
            case 4:
                countOfSlicesOnPageV = new Vector2(4, 5);
                break;
            case 5:
                countOfSlicesOnPageV = new Vector2(4, 6);
                break;
            case 6:
                countOfSlicesOnPageV = new Vector2(5, 7);
                break;
            default:
                countOfSlicesOnPageV = new Vector2(1, 1);
                break;
        }
        Vector2 sizeOfSlices = new Vector2(velkostDokumentu.x/countOfSlicesOnPageV.x, velkostDokumentu.y / countOfSlicesOnPageV.y);
        if (sizeOfSlices.x>sizeOfSlices.y)
        {
            sizeOfSlices.x = sizeOfSlices.y;
        }
        else
        {
            sizeOfSlices.y = sizeOfSlices.x;
        }

        Debug.Log("sizeOfSlices:" + sizeOfSlices);
        
        Vector4 vector4X =size(x);
        a = ReturnBiggestDistance(vector4X); //Math.Abs((Math.Abs(vector4X.x-vector4X.y)>Math.Abs(vector4X.z-vector4X.y))? (vector4X.x - vector4X.y) : (vector4X.z - vector4X.y));
        Vector4 vector4Y =size(y);
        float a1 = ReturnBiggestDistance(vector4Y);//Math.Abs((Math.Abs(vector4Y.x - vector4Y.y) > Math.Abs(vector4Y.z - vector4Y.y)) ? (vector4Y.x - vector4Y.y) : (vector4Y.z - vector4Y.y));
        if (a1>a)
        {
            //Xshift = true;
            a = a1;
        }

        PdfContentByte cb = writer.DirectContent;
        createPDFt(document, cb, x,  velkostDokumentu,  countOfSlicesOnPageV, sizeOfSlices,true,"X");
        createPDFt(document, cb, y,  velkostDokumentu,  countOfSlicesOnPageV, sizeOfSlices,false,"Y");



        document.Close();
        writer.Close();
        fs.Close();
        try
        {
           
            //myDoc.createPDF(NameOfFile);
            System.Diagnostics.Process.Start(NameOfFile);
        }
        catch (Exception)
        {

            
        }
    }
    private float ReturnBiggestDistance(Vector4 a)
    {
        if (Math.Abs(a.x - a.y) > Math.Abs(a.z - a.w)) {
            return Math.Abs(a.x - a.y);
        }
        else
        {
            return Math.Abs(a.z - a.w);
        }
    }

    private void createSVG(List<List<Vector2>> XorY,Vector2 size,bool type)
    {
        /*<svg width="580" height="400" xmlns="http://www.w3.org/2000/svg">
 <g>
  <title>background</title>
  <rect fill="#fff" id="canvas_background" height="402" width="582" y="-1" x="-1"/>
  <g display="none" overflow="visible" y="0" x="0" height="100%" width="100%" id="canvasGrid">
   <rect fill="url(#gridpattern)" stroke-width="0" y="0" x="0" height="100%" width="100%"/>
  </g>
 </g>
 <g>
  <title>Layer 1</title>
  <line stroke-linecap="undefined" stroke-linejoin="undefined" id="svg_1" y2="300.949997" x2="499.5" y1="67.949997" x1="118.5" stroke-width="1.5" stroke="#000" fill="none"/>
 </g>
</svg>*/for (int i = 0; i < XorY.Count; i++)
        {
            
            string title = (type?"X":"Y")+i;
            string Header = "<svg width=\"" + size.x + "\" height=\"" + size.y + "\" xmlns=\"http://www.w3.org/2000/svg\" >\n<g>\n\t<title>" + title + "</title>\n";
            // <rect fill=\"#fff\" id=\"canvas_background\" height=\"402\" width=\"582\" y=\"-1\" x=\"-1\"/>\n< g display = \"none\" overflow = \"visible\" y = \"0\" x = \"0\" height = \"100%\" width = \"100%\" id = \"canvasGrid\" >\n< rect fill = \"url(#gridpattern)\" stroke - width = \"0\" y = \"0\" x = \"0\" height = \"100%\" width = \"100%\" /> ";
            string t = Header;
            for (int j = 0; j < XorY[i].Count-1; j++)
            {
                t += "  <line stroke-linecap=\"undefined\" stroke-linejoin=\"undefined\" id=\"svg_"+j+"\" x1=\"" + (XorY[i][j].x * nasobok+size.x/2) + "\" y1=\"" + (XorY[i][j].y * nasobok + size.y / 2) + "\" x2=\"" + (XorY[i][j+1].x * nasobok + size.x / 2) + "\" y2=\"" + (XorY[i][j+1].y * nasobok + size.y / 2) + "\"  stroke-width=\"1.5\" stroke=\"#000\" fill=\"none\" />\n";
            }
            //posledna ciara z posledneho na prve
            t += "  <line stroke-linecap=\"undefined\" stroke-linejoin=\"undefined\" id=\"svg_" + (XorY[i].Count-1) + "\" x1=\"" + (XorY[i][XorY[i].Count - 1].x * nasobok + size.x / 2) + "\" y1=\"" + (XorY[i][XorY[i].Count - 1].y * nasobok + size.y / 2) + "\" x2=\"" + (XorY[i][0].x * nasobok + size.x / 2) + "\" y2=\"" + (XorY[i][0].y * nasobok + size.y / 2) + "\"  stroke-width=\"1.5\" stroke=\"#000\" fill=\"none\" />\n";
            t += " </g>\n</svg>";
            File.WriteAllText(path + (type ? "/X" : "/Y") + i + ".svg", t);
        }


    }

    void createPNGorJPG(List<List<Vector2>> XorY,Vector2 s,bool isPNG,bool type)
    {
        for (int i = 0; i < XorY.Count; i++)
        {
            Vector4 size = getSize(XorY[i]);
            Texture2D tex = createTex(XorY[i], size, s);
            byte[] bytes;
            if (isPNG)
            {
                bytes = tex.EncodeToPNG();
            }
            else
            {
                bytes = tex.EncodeToJPG();
            }
            File.WriteAllBytes(path + (type?"/X":"/Y") + i + (isPNG?".png":".jpg"), bytes);
            // TexList.Add(tex);
        }
        /*for (int i = 0; i < Y.Count; i++)
        {
            Texture2D tex = createTex(Y[i], getSize(Y[i]), s2 * nasobok);
            byte[] bytes = tex.EncodeToPNG();
            File.WriteAllBytes(path + "/Y" + i + ".png", bytes);
            //TexList.Add();
        }*/
    }

    Texture2D createTex(List<Vector2> points,Vector4 vec,Vector2 s)
    {
        //Vector2 size = new Vector2(vec.x, vec.y);
        Vector2 shift = new Vector2(vec.z, vec.w);
        Debug.Log(vec);
        Texture2D t=new Texture2D((int)s.x,(int)s.y);
        //t.alphaIsTransparency=true;
       // Color[] f = new Color[(int)s.x + 1 * (int)s.y + 1];
        //Array.(f, new Color(1, 1, 1, 0), f.Length);
        t.SetPixels(Enumerable.Repeat(new Color(1, 1, 1, 0), (int)s.x * (int)s.y).ToArray());
      /*  for (int j = 0; j < (int)s.y; j++)
        {
            for (int i = 0; i < (int)s.x; i++)
            {
                t.SetPixel(i, j, new Color(1,1,1,0));
            }
        }*/
        int p1X = (int)((points[points.Count -1].x * nasobok) + (shift.x)/* + size.x / 2*/);
        int p1Y = (int)((points[points.Count -1].y * nasobok) + (shift.y)/* + size.y / 2*/);
        int p2X = (int)((points[0].x * nasobok) + (shift.x)/* + size.x / 2*/);
        int p2Y = (int)((points[0].y * nasobok) + (shift.y)/* + size.y / 2*/);
        DrawLine(t, p1X, p1Y, p2X, p2Y,//, (int)points[points.Count-1].x+(int)size.x/2, (int)points[points.Count-1].y + (int)size.y /2, (int)points[0].x + (int)size.x /2, (int)points[0].y + (int)size.y /2,
              Color.black/*new Color(Random.Range(0.25f, 1.0f), Random.Range(0.25f, 1.0f), Random.Range(0.25f, 1.0f))*/);
        for (int i = 0; i < points.Count-1; i++)
        {
            p1X = (int)((points[i].x * nasobok) + (shift.x)/* + size.x / 2*/);
            p1Y = (int)((points[i].y * nasobok + (shift.y))/* + size.y / 2*/);
            p2X = (int)((points[i+1].x * nasobok) + (shift.x)/* + size.x / 2*/);
            p2Y = (int)((points[i+1].y * nasobok) + (shift.y)/* + size.y / 2*/);
            DrawLine(t, p1X, p1Y, p2X, p2Y,Color.black
           /* new Color(Random.Range(0.25f, 1.0f), Random.Range(0.25f, 1.0f), Random.Range(0.25f, 1.0f))*/);
        }
        t.Apply();
        return t;
    }
    List<List<Vector2>> convertFrom3Dto2D(List<Slice> s)
    {
        List< List < Vector2 > > list=new List<List<Vector2>>();
        for (int i = 0; i < s.Count; i++)
        {
            //List<Vector2> m_points = new List<Vector2>();


            //Quaternion quad = Quaternion.FromToRotation(s[i].getNormal(), Vector3.forward);

            //foreach (Vector3 v in s[i].getPoints())
            //    m_points.Add(quad * v);

            var m_points = TransformFrom3Dto2D(s[i].getPoints(), s[i].getNormal());

            list.Add(m_points);
        }
        return list;
    }
    List<Vector2> TransformFrom3Dto2D(List<Vector3> points,Vector3 normal)
    {
        List<Vector2> m_points = new List<Vector2>();


        Quaternion quad = Quaternion.FromToRotation(normal, Vector3.forward);

        foreach (Vector3 v in points)
            m_points.Add(quad * v);

        return m_points;
    }

    Vector4 size(List<List<Vector2>> x)
    {
        float Xmax = 0;
        float Xmin = 0;
        float Ymax = 0;
        float Ymin = 0;
        float Xdiff = 0;
        float Ydiff = 0;
        for (int j = 0; j < x.Count; j++)
        {
            float Xmaxtmp = x[j][0].x;
            float Xmintmp = x[j][0].x;
            float Ymaxtmp = x[j][0].y;
            float Ymintmp = x[j][0].y;
            var points = x[j];
            for (int i = 1; i < points.Count; i++)
            {
                if (Xmaxtmp < points[i].x)
                {
                    Xmaxtmp = points[i].x;
                }
                if (Xmintmp > points[i].x)
                {
                    Xmintmp = points[i].x;
                }
                if (Ymaxtmp < points[i].y)
                {
                    Ymaxtmp = points[i].y;
                }
                if (Ymintmp > points[i].y)
                {
                    Ymintmp = points[i].y;
                }
            }
            if (Xdiff < Math.Abs(Xmaxtmp - Xmintmp))
            {
                Xdiff = Math.Abs(Xmaxtmp - Xmintmp);
                Xmax = Xmaxtmp;
                Xmin = Xmintmp;
                /*Ymax = Ymaxtmp;
                Ymin = Ymintmp;*/
            }
            if (Ydiff < Math.Abs(Ymaxtmp - Ymintmp))
            {
                Ydiff = Math.Abs(Ymaxtmp - Ymintmp);
                /*Xmax = Xmaxtmp;
                Xmin = Xmintmp;*/
                Ymax = Ymaxtmp;
                Ymin = Ymintmp;
            }
        }
        /*return (Xdiff > Ydiff?Xdiff : Ydiff);*/
        return new Vector4(Xmax,Xmin,Ymax,Ymin);
    }
    Vector4 getSize(List<Vector2> points)
    {
        float Xmax = points[0].x;
        float Xmin = points[0].x;
        float Ymax = points[0].y;
        float Ymin = points[0].y;
        for (int i = 1; i < points.Count; i++)
        {
            if (Xmax < points[i].x)
            {
                Xmax = points[i].x;
            }
            if (Xmin > points[i].x)
            {
                Xmin = points[i].x;
            }
            if (Ymax < points[i].y)
            {
                Ymax = points[i].y;
            }
            if (Ymin > points[i].y)
            {
                Ymin = points[i].y;
            }
        }

        Vector4 size = new Vector4(Mathf.Abs(Xmax- Xmin) * nasobok, Mathf.Abs(Ymax-Ymin) * nasobok ,-Xmin * nasobok, -Ymin * nasobok);
        return size;
    }
    private Vector4 getSize(List<List<Vector2>> x)
    {
        float Xmax = x[0][0].x;
        float Xmin = x[0][0].x;
        float Ymax = x[0][0].y;
        float Ymin = x[0][0].y;
        for (int j = 0; j < x.Count; j++)
        {
            var points = x[j];
            for (int i = 0; i < points.Count; i++)
            {
                if (Xmax < points[i].x)
                {
                    Xmax = points[i].x;
                }
                if (Xmin > points[i].x)
                {
                    Xmin = points[i].x;
                }
                if (Ymax < points[i].y)
                {
                    Ymax = points[i].y;
                }
                if (Ymin > points[i].y)
                {
                    Ymin = points[i].y;
                }
            }
        }
        
        return new Vector4(Mathf.Abs(Xmax - Xmin) * nasobok, Mathf.Abs(Ymax - Ymin) * nasobok, -Xmin * nasobok, -Ymin * nasobok);
    }
    void DrawLine(Texture2D tex, int x1, int y1, int x2, int y2, Color col)
    {
       // Debug.Log(x1+","+y1+";"+x2+","+y2);
        int dy = (int)(y1 - y2);
        int dx = (int)(x1 - x2);
        int stepx, stepy;

        if (dy < 0) { dy = -dy; stepy = -1; }
        else { stepy = 1; }
        if (dx < 0) { dx = -dx; stepx = -1; }
        else { stepx = 1; }
        dy <<= 1;
        dx <<= 1;

        float fraction = 0;

        tex.SetPixel(x2, y2, col);
        if (dx > dy)
        {
            fraction = dy - (dx >> 1);
            while (Mathf.Abs(x2 - x1) > 1)
            {
                if (fraction >= 0)
                {
                    y2 += stepy;
                    fraction -= dx;
                }
                x2 += stepx;
                fraction += dy;
                tex.SetPixel(x2, y2, col);
            }
        }
        else
        {
            fraction = dx - (dy >> 1);
            while (Mathf.Abs(y2 - y1) > 1)
            {
                if (fraction >= 0)
                {
                    x2 += stepx;
                    fraction -= dy;
                }
                y2 += stepy;
                fraction += dx;
                tex.SetPixel(x2, y2, col);
            }
        }
    }
}
