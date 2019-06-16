/*
 Autor: Michal Ondrejó
 Project: Bachelor Thesis - Tvorba řezů z 3D modelů pro vystřihovánky "sliceforms"
*/
using UnityEngine;

namespace Assets.generovanie_prierezov
{
    public static class Shift
    {
        /// <summary>
        /// Calculate new points shifted by (shift*j)
        /// </summary>
        /// <param name="points">Počiatočné body</param>
        /// <param name="span">Veľkosť medzery medzi rezmi</param>
        /// <param name="j">Násobok</param>
        /// <param name="shift">Veľkosť medzery medzi rezmi</param>
        /// <returns>Returns new points</returns>
        public static Vector3[] CalculateNewPointsWithShift(Vector3[] points, float span, int j, float shift)
        {
            Vector3 normal = Slice.getNormal(points);
            Vector3[] pointsNew = new Vector3[points.Length];
            for (int i = 0; i < points.Length; i++)
            {
                pointsNew[i].x = points[i].x + (normal.x * (span * j + shift));
                pointsNew[i].y = points[i].y + (normal.y * (span * j + shift));
                pointsNew[i].z = points[i].z + (normal.z * (span * j + shift));
                
            }
            return pointsNew;
        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="span"></param>
        /// <param name="percent">Percent of shift
        /// <para>range (-0.5f;0.5f)</para>
        /// </param>
        /// <returns></returns>
        public static float SetShift(float span, float percent)
        {
            return span * percent;
        }
    }
}
