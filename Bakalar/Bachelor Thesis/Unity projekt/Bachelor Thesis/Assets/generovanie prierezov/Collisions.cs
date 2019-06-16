/*
 Autor: Michal Ondrejó
 Project: Bachelor Thesis - Tvorba řezů z 3D modelů pro vystřihovánky "sliceforms"
*/
using Assets.generovanie_prierezov.pomocne;
using System;
using System.Collections.Generic;
using UnityEngine;

namespace Assets.generovanie_prierezov
{
    public static class Collisions
    {
        static List<twoPoints> twoPointsList = new List<twoPoints>();

        public static List<twoPoints> FindCollide(Vector3[] PlanePoints, Mesh mesh)
        {
            twoPointsList.Clear();
            int count = mesh.triangles.Length;
            for (int i = 0; i < count; i += 3)
            {

                Vector3 IntersectingPoint1 = new Vector3(0, 0, 0);
                Vector3 IntersectingPoint2 = new Vector3(0, 0, 0);
                Vector3 IntersectingPoint3 = new Vector3(0, 0, 0);
                int A = mesh.triangles[i];
                int B = mesh.triangles[i + 1];
                int C = mesh.triangles[i + 2];
                Vector3 point1 = mesh.vertices[A];
                Vector3 point2 = mesh.vertices[B];
                Vector3 point3 = mesh.vertices[C];

                bool one = false;
                bool two = false;
                bool three = false;
                bool t = false;
                //one
                if (t = LineIntersectPlane(PlanePoints, point1, point2, out IntersectingPoint1, out one))
                {
                    twoPointsList.Add(new twoPoints(point1, new Line(point1, point3), point2, new Line(point2, point3)));
                }
                //two
                if (t = LineIntersectPlane(PlanePoints, point2, point3, out IntersectingPoint2, out two))
                {
                    twoPointsList.Add(new twoPoints(point2, new Line(point1, point3), point3, new Line(point2, point3)));
                }
                //three
                if (t = LineIntersectPlane(PlanePoints, point3, point1, out IntersectingPoint3, out three))
                {
                    twoPointsList.Add(new twoPoints(point3, new Line(point1, point3), point1, new Line(point2, point3)));
                }
                if (t)
                {
                    continue;
                }
                //ak prechadza cez 3:
                if (one && two && three)
                {
                    if (CompareVectorsWithDeviation(IntersectingPoint1, IntersectingPoint2, 0.00001f))
                    {
                        twoPointsList.Add(new twoPoints(IntersectingPoint3, new Line(point1, point3), IntersectingPoint1, new Line(point2, point3)));
                    }
                    if (CompareVectorsWithDeviation(IntersectingPoint2, IntersectingPoint3, 0.00001f))
                    {
                        twoPointsList.Add(new twoPoints(IntersectingPoint1, new Line(point1, point3), IntersectingPoint2, new Line(point2, point3)));
                    }
                    if (CompareVectorsWithDeviation(IntersectingPoint3, IntersectingPoint1, 0.00001f))
                    {
                        twoPointsList.Add(new twoPoints(IntersectingPoint2, new Line(point1, point3), IntersectingPoint3, new Line(point2, point3)));
                    }
                    continue;
                }
                int countOfTrue=0;
                if (one)
                {
                    countOfTrue++;
                }
                if (two)
                {
                    countOfTrue++;
                }
                if (three)
                {
                    countOfTrue++;
                }
                //ak prechadza cez 2:
                if (countOfTrue==2)
                {
                    if (one)
                    {
                        if (two)
                        {
                            twoPointsList.Add(new twoPoints(IntersectingPoint1, new Line(point1, point3), IntersectingPoint2, new Line(point2, point3)));
                        }
                        else
                        {
                            twoPointsList.Add(new twoPoints(IntersectingPoint3, new Line(point1, point3), IntersectingPoint1, new Line(point2, point3)));
                        }
                    }
                    else
                    {
                        twoPointsList.Add(new twoPoints(IntersectingPoint2, new Line(point1, point3), IntersectingPoint3, new Line(point2, point3)));
                    }
                }
                //ak prechadza cez 1:

            }
            return twoPointsList;
        }

        /// <summary>
        /// Prejde cely objekt a nájde všetky body ktoré pretína
        /// </summary>
        /// <param name="PlanePoints">Three or more points of plane</param>
        /// <returns>Returns list of 2-points which collide with entered plane</returns>
        public static List<twoPoints> FindCollideOLD(Vector3[] PlanePoints, Mesh mesh)
        {

            twoPointsList.Clear();
            int count = mesh.triangles.Length;
            for (int i = 0; i < count; i += 3)
            {

                Vector3 IntersectingPoint1 = new Vector3(0, 0, 0);
                Vector3 IntersectingPoint2 = new Vector3(0, 0, 0);
                Vector3 IntersectingPoint3 = new Vector3(0, 0, 0);
                
                int A = mesh.triangles[i];
                int B = mesh.triangles[i + 1];
                int C = mesh.triangles[i + 2];
                Vector3 point1 = mesh.vertices[A];
                Vector3 point2 = mesh.vertices[B];
                Vector3 point3 = mesh.vertices[C];
                Line line1 = new Line(); ;
                Line line2 = new Line();
                bool oneSet = false;
                bool secountSet = false;
                bool isOnPlane = false;
                bool t;
                if (isOnPlane = LineIntersectPlane(PlanePoints, point1, point2, out IntersectingPoint1, out oneSet))
                {
                    twoPointsList.Add(new twoPoints(point1, new Line(point1, point3), point2, new Line(point2, point3)));
                }
                else
                {
                    line1 = new Line(point1, point2);
                }
                if (isOnPlane = LineIntersectPlane(PlanePoints, point2, point3, out IntersectingPoint3, out t))
                {
                    twoPointsList.Add(new twoPoints(point2, new Line(point2, point1), point3, new Line(point3, point1)));
                }
                if (!isOnPlane)
                {
                    if (!oneSet)
                    {
                        if (isOnPlane = LineIntersectPlane(PlanePoints, point3, point1, out IntersectingPoint1, out oneSet))
                        {
                            twoPointsList.Add(new twoPoints(point3, new Line(point3, point2), point1, new Line(point1, point2)));
                        }

                        line1 = new Line(point3, point1);
                    }
                    else
                    {
                        if (isOnPlane = LineIntersectPlane(PlanePoints, point3, point1, out IntersectingPoint2, out secountSet))
                        {
                            twoPointsList.Add(new twoPoints(point3, new Line(point3, point2), point1, new Line(point1, point2)));
                        }
                        line2 = new Line(point3, point1);
                    }
                    if (((oneSet && !secountSet) || CompareVectorsWithDeviation(IntersectingPoint1, IntersectingPoint2, 0.0000001f)) && !isOnPlane)
                    {
                        if (isOnPlane = LineIntersectPlane(PlanePoints, point2, point3, out IntersectingPoint2, out secountSet))
                        {
                            twoPointsList.Add(new twoPoints(point2, new Line(point2, point1), point3, new Line(point3, point1)));
                        }
                        line2 = new Line(point2, point3);
                    }
                    if (oneSet && secountSet && !isOnPlane)
                        twoPointsList.Add(new twoPoints(IntersectingPoint1, line1, IntersectingPoint2, line2));
                }   
            }
            

            return twoPointsList;
        }
        private static bool samePoints(Vector3 A,Vector3 B)
        {
            return A.x == B.x && A.y == B.y && A.z == B.z;
        }
        /// <summary>
        /// Test či čiara pretína plochu
        /// </summary>
        /// <param name="PlanePoints">Body plochy</param>
        /// <param name="point1">Začiatok čiary</param>
        /// <param name="point2">Koniec čiary</param>
        /// <param name="intersection">Vráti bod v ktorom sa pretínajú</param>
        /// <param name="ret">Nájdené pretnutie</param>
        /// <returns>Čiara je paralelná s plochou</returns>
        public static bool LineIntersectPlane(Vector3[] PlanePoints, Vector3 point1, Vector3 point2, out Vector3 intersection, out bool ret,Vector3 planenorm= default(Vector3),bool isdefPlane=false)
        {
            intersection = Vector3.zero;

            if (samePoints(point1,point2))
            {
                ret = false;
                return false;
            }
            ret = false;
            
            float length;
            float dotNumerator;
            float dotDenominator;
            Vector3 vector;
            Vector3 planeNormal;
            if (isdefPlane)
            {
                planeNormal = planenorm;
            }
            else
            {
                planeNormal = Slice.getNormal(PlanePoints);
            }
            Vector3 lineVec = (point2 - point1).normalized;
            //calculate the distance between the linePoint and the line-plane intersection point
            dotNumerator = Vector3.Dot((PlanePoints[1] - point1), planeNormal);
            dotDenominator = Vector3.Dot(lineVec, planeNormal);

            //line and plane are not parallel
            if (Math.Abs(dotDenominator) > 0.000001)
            {
                length = dotNumerator / dotDenominator;

                //create a vector from the linePoint to the intersection point
                vector = Vector3.Normalize(lineVec) * (float)length;

                //get the coordinates of the line-plane intersection point
                intersection = point1 + vector;
                if (IsOnOtherSides( planeNormal,  PlanePoints[1],  point1,  point2)||
                    (point1.x > point2.x ?
                    intersection.x <= point1.x && intersection.x >= point2.x :
                    intersection.x >= point1.x && intersection.x <= point2.x)
                    &&
                    (point1.y > point2.y ?
                    intersection.y <= point1.y && intersection.y >= point2.y :
                    intersection.y >= point1.y && intersection.y <= point2.y)
                    &&
                    (point1.z > point2.z ?
                    intersection.z <= point1.z && intersection.z >= point2.z :
                    intersection.z >= point1.z && intersection.z <= point2.z)
                    )
                {

                    ret = true;
                }
            }
            else
            {
                float d1 = Vector3.Dot((PlanePoints[1] - point1), planeNormal) / planeNormal.magnitude;
                float d2 = Vector3.Dot((PlanePoints[1] - point2), planeNormal) / planeNormal.magnitude;
                
                if (Math.Abs(d1) < 0.000001f && Math.Abs(d2) < 0.000001f)
                {
                    return true;
                }
            }
            
            return false;
        }
        static public bool CompareVectorsWithDeviation(Vector3 point1, Vector3 point2, float dev)
        {
            bool t = true;
            if (Math.Abs(point1.x - point2.x) > dev || Math.Abs(point1.y - point2.y) > dev || Math.Abs(point1.z - point2.z) > dev)
            {
                t = false;
            }


            return t;
        }


        public static bool IsOnOtherSides(Vector3 planeNormal, Vector3 planePoint, Vector3 point1, Vector3 point2)
        {
            float a = planeNormal.x;
            float b = planeNormal.y;
            float c = planeNormal.z;
            float d = Vector3.Dot(new Vector3(-planeNormal.x, -planeNormal.y, -planeNormal.z), planePoint);

            float d1 = a * point1.x + b * point1.y + c * point1.z + d;
            float d2 = a * point2.x + b * point2.y + c * point2.z + d;
            
            if (Math.Sign(d1) != Math.Sign(d2) )
            {
                return true;
            }
            return false;
        }
        public static bool IsBetween(Vector3 point1, Vector3 point2, Vector3 intersection)
        {
            return (point1.x > point2.x ?
                    intersection.x <= point1.x && intersection.x >= point2.x :
                    intersection.x >= point1.x && intersection.x <= point2.x)
                    &&
                    (point1.y > point2.y ?
                    intersection.y <= point1.y && intersection.y >= point2.y :
                    intersection.y >= point1.y && intersection.y <= point2.y)
                    &&
                    (point1.z > point2.z ?
                    intersection.z <= point1.z && intersection.z >= point2.z :
                    intersection.z >= point1.z && intersection.z <= point2.z);
        }

        internal static bool IsBetweenMaxDistance(Vector3 planeMax, Vector3 planeMin, Vector3 mid)
        {

            float x = planeMax.x - planeMin.x;
            float y = planeMax.y - planeMin.y;
            float z = planeMax.z - planeMin.z;
            float min = 0, max = 0, m = 0;
            switch (IsGreater(x,y,z))
            {
                case 1:
                    min = planeMax.x > planeMin.x ? planeMin.x : planeMax.x;
                    max = planeMax.x > planeMin.x ? planeMax.x : planeMin.x;
                    m = mid.x;
                    break;
                case 2:
                    min = planeMax.y > planeMin.y ? planeMin.y : planeMax.y;
                    max = planeMax.y > planeMin.y ? planeMax.y : planeMin.y;
                    m = mid.y;
                    break;
                case 3:
                    min = planeMax.z > planeMin.z ? planeMin.z : planeMax.z;
                    max = planeMax.z > planeMin.z ? planeMax.z : planeMin.z;
                    m = mid.z;
                    break;
            }
            return (min <= m && m <= max) 
                || CompareVectorsWithDeviation(planeMin, mid, 0.01f)
                || CompareVectorsWithDeviation(planeMax, mid, 0.01f);
        }
        public static int MaxAxisDistance(Vector3 PointMax, Vector3 PointMin)
        {
            float x = Math.Abs(PointMax.x - PointMin.x);
            float y = Math.Abs(PointMax.y - PointMin.y);
            float z = Math.Abs(PointMax.z - PointMin.z);
            return IsGreater(x, y, z);

        }
        private static int IsGreater(float x, float y, float z)
        {
            if (x > y && x > z)
            {
                return 1;
            }
            if (y > z && y > x)
            {
                return 2;
            }
            return 3;
        }
    }
}
