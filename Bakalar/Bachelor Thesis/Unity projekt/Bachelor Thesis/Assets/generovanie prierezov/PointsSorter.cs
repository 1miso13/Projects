/*
 Autor: Michal Ondrejó
 Project: Bachelor Thesis - Tvorba řezů z 3D modelů pro vystřihovánky "sliceforms"
*/
using System.Collections.Generic;
using UnityEngine;

namespace Assets.generovanie_prierezov
{
    public static class PointsSorter
    {
        public static void SortingVec3(ref List<Vector3> najdene, ref List<int> najdeneIndex, bool reverse = false)
        {
            List<Vector3> n = new List<Vector3>();
            List<int> nI = new List<int>();
            if (najdene.Count==0)
            {
                return;
            }
                 
            n.Add(najdene[0]);
            nI.Add(najdeneIndex[0]);
            for (int i = 1; i < najdene.Count; i++)
            {
                int j = 0;
                for (; j < n.Count && IsBigger(najdene[i], n[j]); j++)
                {
                }
                n.Insert(j, najdene[i]);
                nI.Insert(j,najdeneIndex[i]);
            }
            if (reverse)
            {
                n.Reverse();
                nI.Reverse();
            }
            najdene = n;
            najdeneIndex = nI;
        }
        public static void SortingVec3(ref List<Vector3> najdene)
        {
            List<Vector3> n = new List<Vector3>();
            if (najdene.Count == 0)
            {
                return;
            }

            n.Add(najdene[0]);
            for (int i = 1; i < najdene.Count; i++)
            {
                int j = 0;
                for (; j < n.Count && IsBigger(najdene[i], n[j]); j++)
                {
                }
                n.Insert(j, najdene[i]);
            }
            najdene = n;
        }
        public static void SortingVec3OLD(ref List<Vector3> najdene, ref List<int> najdeneIndex)
        {
            List<Vector3> n = new List<Vector3>();
            List<int> nI = new List<int>();
            List<Vector3> tmpList = najdene.GetRange(0, najdene.Count);
            List<int> tmpIList = najdeneIndex.GetRange(0, najdeneIndex.Count);
            for (int i = 0; i < najdene.Count; i++)
            {
                int maxIndex = 0;
                for (int j = 1; j < tmpList.Count; j++)
                {//najdeme naj
                    if (najdene[maxIndex].x > najdene[j].x)
                    {
                        maxIndex = j;
                    }
                    else
                    {
                        if (najdene[maxIndex].x == najdene[j].x)
                        {
                            if (najdene[maxIndex].y > najdene[j].y)
                            {
                                maxIndex = j;
                            }
                            else
                            {
                                if (najdene[maxIndex].y == najdene[j].y && najdene[maxIndex].z > najdene[j].z)
                                {
                                    maxIndex = j;
                                }

                            }
                        }
                    }
                }
                n.Add(tmpList[maxIndex]);
                nI.Add(tmpIList[maxIndex]);
                tmpList.RemoveAt(maxIndex);
                tmpIList.RemoveAt(maxIndex);
            }
            najdene = n;
            najdeneIndex = nI;

        }

        public  static bool IsBigger(Vector3 value, Vector3 than)
        {
            if (value.x == than.x)
            {
                if (value.y == than.y)
                {
                    return value.z > than.z;
                }
                else
                {
                    return value.y > than.y;
                }
            }
            else
            {
                return value.x > than.x;
            }
            
        }
    }
}
