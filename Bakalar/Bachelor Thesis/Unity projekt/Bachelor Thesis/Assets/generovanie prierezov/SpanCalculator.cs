/*
 Autor: Michal Ondrejó
 Project: Bachelor Thesis - Tvorba řezů z 3D modelů pro vystřihovánky "sliceforms"
*/
using UnityEngine;

namespace Assets.generovanie_prierezov
{
    public static class SpanCalculator
    {
        /// <summary>
        /// Vypočíta potrebný rozmer medzi rezmi
        /// </summary>
        /// <param name="points">Body objektu
        /// <para>Prejdú sa všetky body v objekde a zistia sa ktoré sú najdalej</para>
        /// </param>
        /// <param name="countOfSlices">Počet rezov</param>
        /// <param name="PlanePoints">3 body plochy</param>
        /// <returns>Vzdialenosť medzi rezmi</returns>
        public static float CalculateSpan(Vector3[] points,int countOfSlices, Vector3[] PlanePoints,out float shift)
        {
            var distance = CalculateDistance(ref points,Slice.getNormal(PlanePoints));
            float distanceOfFirstPoint=distance.x;
            float distanceOfSecondPoint=distance.y;
            //Vypočíta sa tak, aby bola medzera od kraja 50% vzdialenosti
            float span = (distanceOfFirstPoint - distanceOfSecondPoint) / (countOfSlices );
            shift = (countOfSlices%2==0?span/2:0);
            return span ;
        }
        /// <summary>
        /// Returns vector which contains distance of farest points which colide with entered flat
        /// </summary>
        /// <param name="points">Points of object</param>
        /// <param name="PlanePoints">Points of flat</param>
        /// <returns>Returns vector which contains distance of farest points which colide with entered flat</returns>
        public static Vector2 CalculateDistance(ref Vector3[] points, Vector3[] PlanePoints)
        {
            return CalculateDistance(ref points, Slice.getNormal(PlanePoints), PlanePoints[0]);
        }
        public static Vector2 CalculateDistance(ref Vector3[] points, Vector3 planeNormal, Vector3 planepoint= default(Vector3))
        { 
            //Vector3 planeNormal = Slice.getNormal(PlanePoints);
            float a = planeNormal.x;
            float b = planeNormal.y;
            float c = planeNormal.z;
            float d = Vector3.Dot(new Vector3(-a, -b, -c), planepoint);


            float distance;
            float distanceMax = 0;
            float distanceMin = 0;
            foreach (var point in points)
            {
                distance = a * point.x + b * point.y + c * point.z + d;
                if (distance > distanceMax)
                {
                    distanceMax = distance;
                }
                if (distance < distanceMin)
                {
                    distanceMin = distance;
                }
            }

            return new Vector2(distanceMax,distanceMin);
        }
    }
}
