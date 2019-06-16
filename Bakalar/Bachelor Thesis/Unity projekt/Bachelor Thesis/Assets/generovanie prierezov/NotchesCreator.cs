/*
 Autor: Michal Ondrejó
 Project: Bachelor Thesis - Tvorba řezů z 3D modelů pro vystřihovánky "sliceforms"
*/
using System;
using System.Collections.Generic;
using UnityEngine;

namespace Assets.generovanie_prierezov
{
    public static class NotchesCreator
    {
        public static  Vector3 SizeOfObject;
        /// <summary>
        /// Vytvorí zárezy do rezov
        /// <para>OBSOLETE!</para>
        /// </summary>
        /// <param name="listOfPoints">Body rezu</param>
        /// <param name="AnotherSlices">Body kolmého rezu</param>
        /// <param name="shiftB">Posuv zárezov (posuv kolmej vrstvy)</param>
        /// <param name="type">prierezy v ose X/Y</param>
        /// <param name="up">Generovanie prierezov od stredu T=hore/F=dole</param>
        public static void createNotchesOLD(List<Vector3> listOfPoints, Vector3[] AnotherSlices, float shiftB, int type, bool up, float shift)
        {
            //List<int> indexNajdenych = new List<int>();
            int j = up ? 0 : -1;
            bool foundSomething = false;
            bool stillNotFound = true;
            do
            {
                //List<Vector3> Najdene = new List<Vector3>();
                //List<int> NajdeneIndex = new List<int>();
                /*Vector3[] AnotherSlicesnew;
                
                //indexNajdenych.Clear();
                AnotherSlicesnew = */Shift.CalculateNewPointsWithShift(AnotherSlices, shiftB, j, shift);

                #region commented code
                ////prejdeme vsetky casti a najdeme v ktorych bodoch sa pretinaju
                //var SliceNormal = Slice.getNormal(AnotherSlices);
                //for (int i = 0; i < listOfPoints.Count; i++)
                //{
                //    if (Collisions.IsOnOtherSides(SliceNormal, AnotherSlices[0], listOfPoints[i], listOfPoints[i < listOfPoints.Count - 1 ? i + 1 : 0]))
                //    {
                //        if (indexNajdenych.Count > 0 && indexNajdenych[indexNajdenych.Count - 1] == i - 1)
                //        {//ak su 2 body za sebou, tak zistijeme ci ma skutocne prejst cez oba alebo ide iba o chybu 
                //         /*
                //          *.......|X........X-_.......|.............
                //          *......./............--_....|.............
                //          *....../|...............--_.|.............
                //          *....()_|..................(|)............
                //          *.......--__................|.--_.........
                //          *.......|.. --__............|....--_......
                //          *.......|.......--X.........|.......-X....
                //          */
                //            if (!Collisions.IsOnOtherSides(SliceNormal, AnotherSlices[0], listOfPoints[i - 1], listOfPoints[i < listOfPoints.Count - 1 ? i + 1 : 0]))
                //            {

                //                foundSomething = true;
                //                indexNajdenych.Add(i);
                //            }
                //        }
                //        else
                //        {

                //            foundSomething = true;
                //            indexNajdenych.Add(i);
                //        }
                //    }
                //}
                ////
                //for (int i = 0; i < indexNajdenych.Count; i++)
                //{
                //    Vector3 intersection;
                //    bool ret;
                //    bool d = Collisions.LineIntersectPlane(AnotherSlices, listOfPoints[indexNajdenych[i]], listOfPoints[indexNajdenych[i] < listOfPoints.Count - 1 ? indexNajdenych[i] + 1 : 0], out intersection, out ret);
                //    if (ret)
                //    {
                //        Najdene.Add(intersection);
                //        NajdeneIndex.Add(indexNajdenych[i] + 1 < listOfPoints.Count ? indexNajdenych[i] + 1 : 0);
                //    }

                //}






                //zoradenie
                //Debug.Log("j:"+j);
                /*var before = Najdene.GetRange(0,Najdene.Count);
                Debug.Log("B");
                SortingVec3(ref Najdene,ref NajdeneIndex);
                 for (int i = 0; i < Najdene.Count; i++)
                 {

                     Debug.Log(Najdene[i] + "\t---\t" +before[i]);
                 }*/
                //vlozenie do pola
                //for (int i = (Najdene.Count % 2 == 0 ? Najdene.Count : Najdene.Count /*- 1*/) - 1; i > 0; i -= 2)
                //{
                //    bool t;
                //    bool failure = false;
                //    Vector3 point1 = Najdene[i];
                //    int k = 1;
                //    Vector3 point2 = Najdene[i - k];
                //    /* {
                //         if (i - k >= 0)
                //         {

                //             point2 = Najdene[i - k];
                //         }
                //         else
                //         {
                //             failure = true;
                //             break;
                //         }
                //         k++;
                //     } while (point1 == point2) ;*/
                //    if (!failure)
                //    {
                //        if (point1.x != point2.x)
                //        {
                //            if (point1.x > point2.x)
                //            {
                //                t = false;
                //            }
                //            else
                //            {
                //                t = true;
                //            }
                //        }
                //        else
                //        {
                //            if (point1.y != point2.y)
                //            {
                //                if (point1.y > point2.y)
                //                {
                //                    t = false;
                //                }
                //                else
                //                {
                //                    t = true;
                //                }
                //            }
                //            else
                //            {
                //                if (point1.z > point2.z)
                //                {
                //                    t = false;
                //                }
                //                else
                //                {
                //                    t = true;
                //                }
                //            }
                //        }




                //        if (type == 0)
                //        {
                //            t = !t;
                //        }

                //        Vector3 mid = (Najdene[i] + Najdene[i - 1]) / 2.0f;
                //        if (t)
                //        {
                //            listOfPoints.Insert(NajdeneIndex[i], point1);
                //            listOfPoints.Insert(NajdeneIndex[i], mid);
                //            listOfPoints.Insert(NajdeneIndex[i], point1);
                //        }
                //        else
                //        {
                //            listOfPoints.Insert(NajdeneIndex[i - 1], point2);
                //            listOfPoints.Insert(NajdeneIndex[i - 1], mid);
                //            listOfPoints.Insert(NajdeneIndex[i - 1], point2);
                //        }
                //    }
                //    else
                //    {
                //        break;
                //    }
                //}
                #endregion
                
                //foundSomething =  CreateNotche(listOfPoints, type, AnotherSlicesnew,Slice.getNormal(AnotherSlicesnew));
                
                if (up)
                {
                    j++;
                }
                else
                {
                    j--;
                }

                if (foundSomething)
                {
                    stillNotFound = false;
                }

            } while ((/*indexNajdenych.Count != 0*/ foundSomething || ((!foundSomething || stillNotFound) && Math.Abs(j*shiftB) < SizeOfObject.magnitude)) );

        }


        /// <summary>
        /// Create notches where collide two slices
        /// </summary>
        public static void CreateNotches(List<Slice> X, List<Slice> Y)
        {
            

            Vector3 max = default(Vector3);
            Vector3 min = default(Vector3);
            for (int i = 0; i < X.Count; i++)
            {
                for (int j = 0; j < Y.Count; j++)
                {
                    if(
                    GetSize(Y[j].getPoints(), out max, out min, X[i].getNormal()/*returnedVector*/, X[i].getPoints().ToArray()) || true)
                    {
                        CreateNotche(X[i].getPoints(), 0, Y[j].getPoints().ToArray(), Y[j].getNormal(), max, min);
                        X[i].AddIPoints(IPoints,j);
                    }
                }
                X[i].Destroy();
                X[i].AddLines();
            }
            for (int j = 0; j < Y.Count; j++)
            {

                for (int i = 0; i < X.Count; i++)
                {
                    if(
                    GetSize(X[i].getPoints(), out max, out min, Y[j].getNormal(),/* returnedVector,*/ Y[j].getPoints().ToArray()) || true)
                    {
                        CreateNotche(Y[j].getPoints(), 1, X[i].getPoints().ToArray(), X[i].getNormal(), max, min);
                        Y[j].AddIPoints(IPoints, i);
                    }
                }
                Y[j].Destroy();
                Y[j].AddLines();
            }
        }
        /// <summary>
        /// Use only after method CreateNotche;
        /// </summary>
        private static  List<Vector3> IPoints=null; 
        /// <summary>
        /// vytvorý jeden zárez do objektu
        /// </summary>
        /// <param name="listOfPoints"></param>
        /// <param name="type"></param>
        /// <param name="AnotherSlices"></param>
        /// <returns></returns>
        private static bool CreateNotche(List<Vector3> listOfPoints, int type , Vector3[] AnotherSlices, Vector3 SliceNormal,Vector3 planeMax,Vector3 planeMin)
        {
            List<Vector3> Najdene = new List<Vector3>();
            List<int> NajdeneIndex = new List<int>();

            bool foundSomething = FindCollidingPoints(listOfPoints, SliceNormal, AnotherSlices, ref Najdene, ref NajdeneIndex, planeMax, planeMin,false);

            PointsSorter.SortingVec3(ref Najdene, ref NajdeneIndex,type==0);
            IPoints = AddPoint(Najdene, listOfPoints, NajdeneIndex, type, planeMax, planeMin);
            return foundSomething;
        }
        private static bool FindCollidingPoints(List<Vector3> listOfPoints, Vector3 SliceNormal, Vector3[] AnotherSlices, ref List<Vector3> Najdene, ref List<int> NajdeneIndex, Vector3 planeMax = default(Vector3), Vector3 planeMin = default(Vector3), bool sccsDebug = true)
        {
            List<int> indexNajdenych = new List<int>();

            bool foundSomething = false;
            //prejdeme vsetky casti a najdeme v ktorych bodoch sa pretinaju
            //var SliceNormal = Slice.getNormal(AnotherSlices);
            for (int i = 0; i < listOfPoints.Count; i++)
            {
                if (Collisions.IsOnOtherSides(SliceNormal, AnotherSlices[1], listOfPoints[i], listOfPoints[i < listOfPoints.Count - 1 ? i + 1 : 0]))
                {
                    if (indexNajdenych.Count > 0 && indexNajdenych[indexNajdenych.Count - 1] == i - 1)
                    {//ak su 2 body za sebou, tak zistijeme ci ma skutocne prejst cez oba alebo ide iba o chybu 
                     /*
                      *.......|X........X-_.......|.............
                      *......./............--_....|.............
                      *....../|...............--_.|.............
                      *....()_|..................(|)............
                      *.......--__................|.--_.........
                      *.......|.. --__............|....--_......
                      *.......|.......--X.........|.......-X....
                      */
                        if (!Collisions.IsOnOtherSides(SliceNormal, AnotherSlices[1], listOfPoints[i - 1], listOfPoints[i < listOfPoints.Count - 1 ? i + 1 : 0]))
                        {

                            foundSomething = true;
                            indexNajdenych.Add(i);
                        }
                    }
                    else
                    {

                        foundSomething = true;
                        indexNajdenych.Add(i);
                    }
                }
            }
            //
            for (int i = 0; i < indexNajdenych.Count; i++)
            {
                Vector3 intersection;
                bool ret;
                bool t = Collisions.LineIntersectPlane(AnotherSlices, listOfPoints[indexNajdenych[i]], listOfPoints[indexNajdenych[i] < listOfPoints.Count - 1 ? indexNajdenych[i] + 1 : 0], out intersection, out ret, SliceNormal, true);
                if (ret&& (Collisions.IsBetweenMaxDistance(planeMax, planeMin, intersection)||sccsDebug))
                {
                    Najdene.Add(intersection);
                    NajdeneIndex.Add(indexNajdenych[i] + 1 < listOfPoints.Count ? indexNajdenych[i] + 1 : 0);
                }
                else
                if (t && (Collisions.IsBetweenMaxDistance(planeMax, planeMin, intersection) || sccsDebug))
                {
                    Najdene.Add(listOfPoints[indexNajdenych[i] < listOfPoints.Count - 1 ? indexNajdenych[i] + 1 : 0]);
                    NajdeneIndex.Add(indexNajdenych[i] + 1 < listOfPoints.Count ? indexNajdenych[i] + 1 : 0);
                    Najdene.Add(listOfPoints[indexNajdenych[i]]);
                    NajdeneIndex.Add(indexNajdenych[i] + 1 < listOfPoints.Count ? indexNajdenych[i] + 1 : 0);
                    Najdene.Add(listOfPoints[indexNajdenych[i]]);
                    NajdeneIndex.Add(indexNajdenych[i] + 1 < listOfPoints.Count ? indexNajdenych[i] + 1 : 0);
                }

            }
            return foundSomething;
        }
        private static List<Vector3> AddPoint(List<Vector3> Najdene, List<Vector3> listOfPoints, List<int> NajdeneIndex, int type, Vector3 planeMax, Vector3 planeMin)
        {
            List<Vector3> ListMidPoints = new List<Vector3>();
            var offset = 0;
            for (int i = (Najdene.Count % 2 == 0 ? Najdene.Count : Najdene.Count /*- 1*/) - 1; i > 0; i -= 2)
            {
                bool t = false;
                bool failure = false;
                Vector3 point1 = Najdene[i];
                int k = 1;
                Vector3 point2 = Najdene[i - k];

                if (!failure)
                {
                    int d = Collisions.MaxAxisDistance(point1, point2);
                    switch (d)
                    {
                        case 1:
                            if (point1.x != point2.x)
                            {
                                if (point1.x > point2.x)
                                {
                                    t = false;
                                }
                                else
                                {
                                    t = true;
                                }
                            }
                            break;
                        case 2:
                            if (point1.y != point2.y)
                            {
                                if (point1.y > point2.y)
                                {
                                    t = false;
                                }
                                else
                                {
                                    t = true;
                                }
                            }
                            break;
                        case 3:
                            if (point1.z > point2.z)
                            {
                                t = false;
                            }
                            else
                            {
                                t = true;
                            }
                            break;
                    }
                
                    /*if (point1.x != point2.x)
                    {
                        if (point1.x > point2.x)
                        {
                            t = false;
                        }
                        else
                        {
                            t = true;
                        }
                    }
                    else
                    {
                        if (point1.y != point2.y)
                        {
                            if (point1.y > point2.y)
                            {
                                t = false;
                            }
                            else
                            {
                                t = true;
                            }
                        }
                        else
                        {
                            if (point1.z > point2.z)
                            {
                                t = false;
                            }
                            else
                            {
                                t = true;
                            }
                        }
                    }*/




                    if (type == 0)
                    {
                        t = !t;
                    }


                    Vector3 mid = (Najdene[i] + Najdene[i - 1]) / 2.0f;

                    if (Collisions.IsBetweenMaxDistance(planeMax,planeMin,mid))
                    {
                        ListMidPoints.Add(mid);
                    }

                    if (t)
                    {
                        offset = CalculateOffset(NajdeneIndex, i);
                        listOfPoints.Insert(NajdeneIndex[i] + offset, point1);
                        listOfPoints.Insert(NajdeneIndex[i] + offset, mid);
                        listOfPoints.Insert(NajdeneIndex[i] + offset, point1);
                        if (!IsBiggest(NajdeneIndex,i))
                        {
                            offset += 3;
                        }
                    }
                    else
                    {
                        offset = CalculateOffset(NajdeneIndex, i-1);
                        listOfPoints.Insert(NajdeneIndex[i - 1] + offset, point2);
                        listOfPoints.Insert(NajdeneIndex[i - 1] + offset, mid);
                        listOfPoints.Insert(NajdeneIndex[i - 1] + offset, point2);
                        if (!IsBiggest(NajdeneIndex, i-1))
                        {
                            offset += 3;
                        }
                    }
                }
                else
                {
                    break;
                }
            }
            return ListMidPoints;
        }

        private static bool IsBiggest(List<int> ListOfIndex, int actualIndex)
        {
            for (int i = actualIndex - 1; i >= 0; i--)
            {
                if (ListOfIndex[actualIndex] < ListOfIndex[i])
                {
                    return false;
                }
            }
            return true;
        }
        private static int CalculateOffset(List<int> ListOfIndex, int actualIndex)
        {
            int offset = 0;
            for (int i = actualIndex + 2; i < ListOfIndex.Count; i+=2)
            {
                if (ListOfIndex[actualIndex] > ListOfIndex[i])
                {
                    offset+=3;
                }
            }
            return offset;
        }

        private static bool GetSize(List<Vector3> list,out Vector3 max, out Vector3 min,Vector3 normal,Vector3[] AnotherSlices,bool ddddebug=false)
        {
            List<Vector3> FoundID = new List<Vector3>();
            List<int> FoundIndex = new List<int>();
            var found = FindCollidingPoints(list, normal/*Slice.getNormal(AnotherSlices)*/,AnotherSlices,ref FoundID,ref FoundIndex);

            MaxMin(FoundID, out max, out min);
            
            return found;
        }

        private static bool MaxMin(List<Vector3> foundID, out Vector3 max, out Vector3 min)
        {
            if (foundID.Count > 0)
            {

                max = min = foundID[0];
                for (int i = 1; i < foundID.Count; i++)
                {
                    if (PointsSorter.IsBigger(foundID[i], max))
                    {
                        max = foundID[i];
                    }
                    if (PointsSorter.IsBigger(min, foundID[i]))
                    {
                        min = foundID[i];
                    }
                }
                return true;
            }
            else
                max = min = default(Vector3);
                return false;
        }
    }
}
