/*
 Autor: Michal Ondrejó
 Project: Bachelor Thesis - Tvorba řezů z 3D modelů pro vystřihovánky "sliceforms"
*/
using System.Collections.Generic;
using UnityEngine;

namespace Assets.generovanie_prierezov
{
    public static class RemoveSlice
    {
        internal static bool RemoveNotCollidingSlices(List<Slice> list1, List<Slice> list2)
        {
            bool removed = false; ;
            int pocetPretnuti;
            for (int i = 0; i < list1.Count; i++)
            {
                pocetPretnuti = 0;
                for (int j = 0; j < list2.Count; j++)
                {
                    if (Colliding(list1[i].listOfPoints,list2,j))
                    {
                        pocetPretnuti++;
                    }
                }
                if (pocetPretnuti<2)
                {
                    generate.DelSliceAt(list1, i);
                    removed = true;
                    i--;
                }
            }


            return removed;
        }

        private static bool Colliding(List<Vector3> listOfPoints, List<Slice> sliceList,int index)
        {
            for (int j = 0; j < listOfPoints.Count; j++)
            {
                //for (int i = 0; i < sliceList.Count; i++)
                {
                    if (Collisions.IsOnOtherSides(sliceList[index].getNormal(), sliceList[index].listOfPoints[1],listOfPoints[j], listOfPoints[j + 1 < listOfPoints.Count?j+1:0]))
                    {
                        return true;
                    }
                }
            }
            return false;
        }
    }
}