/*
 Autor: Michal Ondrejó
 Project: Bachelor Thesis - Tvorba řezů z 3D modelů pro vystřihovánky "sliceforms"
*/
using UnityEngine;

namespace Assets.generovanie_prierezov.pomocne
{
    public struct Line
    {
        public Vector3 point1;
        public Vector3 point2;

        

        public Line(Vector3 point1, Vector3 point2)
        {
            this.point2 = point2;
            this.point1 = point1;
        }

        public static bool compareTwoLines(Line line1, Line line2)
        {
            if (line1.point1==line2.point1 && line1.point2==line2.point2)
            {
                return true;
            }
            if (line1.point1==line2.point2 && line1.point2==line2.point1)
            {

                return true;
            }

            return false;
        }
        public static GameObject DrawLine(Vector3 start, Vector3 end, Color color, float duration = 0.2f)
        {
            GameObject myLine = new GameObject(start.ToString() + ";" + end.ToString());
            myLine.transform.position = start;
            myLine.AddComponent<LineRenderer>();
            LineRenderer lr = myLine.GetComponent<LineRenderer>();
            lr.materials[0] = Resources.Load("New Material 1") as Material;
            lr.startColor = color;
            lr.endColor = color;
            lr.startWidth = 0.002f;
            lr.endWidth = 0.002f;
            lr.SetPosition(0, start);
            lr.SetPosition(1, end);
            return myLine;
        }
    }
}
