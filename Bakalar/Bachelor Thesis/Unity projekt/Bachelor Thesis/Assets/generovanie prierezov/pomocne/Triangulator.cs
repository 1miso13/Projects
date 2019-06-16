/*
 Autor: Michal Ondrejó
 Project: Bachelor Thesis - Tvorba řezů z 3D modelů pro vystřihovánky "sliceforms"
*/
using System.Collections.Generic;
using UnityEngine;

namespace Assets.generovanie_prierezov.pomocne
{
    public class Triangulator
    {
        List<int> intArray;
        public List<int> triangles;
        public Triangulator(List<Vector3> points3D, Vector3 normal)
        {
            var points2D = TransformFrom3Dto2D(points3D, normal);
            triangles = new List<int>();
            intArray = FillArray(points2D.Count, IsClockWiseRotated(points2D));

            int prev = 0;
            int mid;
            int next;
            int watchdog=intArray.Count;
            while (intArray.Count > 2)
            {
                watchdog--;
                prev = (prev + 1) % intArray.Count;
                mid = (prev+1) % intArray.Count;
                next = (mid+1) % intArray.Count;
                    
                //ak je spravne natocene a zaroven neobsahuje ziadny dalsi bod
                if (NotPointInTriangle(intArray[prev], intArray[mid], intArray[next], points2D) && 0 < CrossProduct2D(points2D[intArray[prev]], points2D[intArray[mid]], points2D[intArray[next]]))
                {
                    triangles.Add(intArray[prev]);
                    triangles.Add(intArray[mid]);
                    triangles.Add(intArray[next]);
                    intArray.RemoveAt(mid);
                    watchdog = intArray.Count*2;
                }
                if (watchdog==0)
                {
                    break;
                }
            }
        }
        private bool NotPointInTriangle(int prev, int mid, int next, List<Vector2> points2D)
        {
            for (int j = 0; j < points2D.Count; j++)
            {
                if (j != prev || j != mid || j !=next)
                {
                    if (IsPInTriangle(points2D[prev], points2D[mid], points2D[next], points2D[j]))
                    {
                        return false;
                    }
                }
            }
            return true;
        }

        /// <summary>
        /// Test if is clockwise
        /// </summary>
        /// <param name="point2D"></param>
        private bool IsClockWiseRotated(List<Vector2> point2D)
        {

            float A = 0;
            int j = point2D.Count-1;
            for (int i = 0; i < point2D.Count; i++)
            {
                var pointA = point2D[j];
                var pointB = point2D[i];
                A += pointA.x * pointB.y - pointB.x * pointA.y;
                j = i;
            }

            if (A<0)
            {
                return true;
            }
            else
            {
                return false;
            }


        }
        private List<int> FillArray(int length, bool reversed=false)
        {
            List<int> d = new List<int>();
            for (int i = 0; i < length; i++)
            {
                if (!reversed)
                {
                    d.Add(i);
                }
                else
                {
                    d.Add(length - 1 - i);
                }
            }
            return d;
        }
        public List<Vector2> TransformFrom3Dto2D(List<Vector3> points3D,Vector3 normal)
        {
            Quaternion quad = Quaternion.FromToRotation(normal, Vector3.forward);
            List<Vector2> points2D= new List<Vector2>();
            foreach (Vector3 v in points3D)
                points2D.Add(quad * v);
            return points2D;
        }

        static public  bool IsPInTriangle(Vector2 TrianglePoint1, Vector2 TrianglePoint2, Vector2 TrianglePoint3, Vector2 P)
        {
            bool a = CrossProduct2D(TrianglePoint1, TrianglePoint2, P) > 0;
            bool b = CrossProduct2D(TrianglePoint2, TrianglePoint3, P) > 0;
            bool c = CrossProduct2D(TrianglePoint3, TrianglePoint1, P) > 0;
            return a && b && c;
        }
        
        static private float CrossProduct2D(Vector2 Point1, Vector2 Point2, Vector2 point)
        {

            Vector2 a;
            a.x = Point2.x - Point1.x;
            a.y = Point2.y - Point1.y;
            Vector2 b;
            b.x = point.x - Point1.x;
            b.y = point.y - Point1.y;

            // just calculate the z-component
            return a.x * b.y - a.y * b.x;
            }
        }
}
