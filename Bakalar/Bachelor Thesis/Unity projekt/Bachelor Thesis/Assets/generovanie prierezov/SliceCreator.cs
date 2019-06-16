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
    public static class SliceCreator
    {
        /// <summary>
        /// posuv Y
        /// </summary>
        public static float shiftY;
        /// <summary>
        /// posuv X
        /// </summary>
        public static float shiftX;
        public static void SetShift(float shift1,float shift2)
        {
            shiftY = shift1;
            shiftX = shift2;
        }
        public static List<Slice> listX;
        public static List<Slice> listY;
        /// <summary>
        /// Pomocna premenna pre create Slices,
        /// ukončuje generovanie v danej ose 
        /// </summary>
        public static bool stop = true;
        /// <summary>
        /// Vytvorenie prierezu
        /// </summary>
        /// <param name="type">0 = X, 1=Y</param>
        /// <param name="span">Medzera medzi rezmi</param>
        /// <param name="up">Hore/Dole</param>
        /// <param name="shiftB">Posuv použitý pre tvorbu zárezov</param>
        /// <param name="i">Slice index</param>
        public static void CreateSlices(int type, float span, bool up, float shiftB,int i,Transform transform,ref GameObject objectNaSpracovanie,ref List<GameObject> lineList)
        {
            {
                stop = true;

                List<Vector3> listOfPoints = new List<Vector3>();

                //vypocet novych bodov pre generovanie dalších prierezov
                Vector3[] shiftedPlanePoints = Shift.CalculateNewPointsWithShift(
                        new Vector3[3] {
                        GetPoint(type,0),
                        GetPoint(type,1),
                        GetPoint(type,2)
                        },
                        span,
                        i,
                        (type==0?shiftY:shiftX)
                    );
                Vector3 normal = Vector3.Cross(GetPoint(type, 1) - GetPoint(type, 0), GetPoint(type, 2) - GetPoint(type, 0)).normalized;
                //naplnenie listu bodmi
                DateTime startFindCollide = DateTime.Now;
                List<twoPoints> twoPointsList = Collisions.FindCollide(shiftedPlanePoints, objectNaSpracovanie.GetComponent<MeshFilter>().mesh);
                Test.NewDataPost("FindCollide", "Ticks", (DateTime.Now-startFindCollide).Ticks,"pocet vsetkych trojuholnikov",objectNaSpracovanie.GetComponent<MeshFilter>().mesh.triangles.Length,"pocet najdenych bodov",twoPointsList.Count);

                while (twoPointsList.Count > 0)
                {
                    if (twoPoints.createArrayFromTwoPoints(twoPointsList, ref listOfPoints))
                    {
                        DeleteDuplicities(ref listOfPoints);
                        switch (type)
                        {
                            case 0://X
                                Slice s = CreateSlice(listOfPoints, type, i, Color.blue, normal,transform);
                                if (s != null)
                                {
                                    if (up)
                                    {
                                        listX.Add(s);
                                    }
                                    else
                                    {
                                        listX.Insert(0, s);
                                    }
                                }
                                break;
                            case 1://Y
                                Slice s2 = CreateSlice(listOfPoints, type, i, Color.red, normal,transform);
                                if (s2 != null)
                                {
                                    if (up)
                                    {
                                        listY.Add(s2);
                                    }
                                    else
                                    {
                                        listY.Insert(0, s2);
                                    }
                                }
                                break;
                        }
                        stop = false;//naslo sa, tak hladame aj dalsie
                    }
                    else
                    {
                        //ak sa nepodarilo vytvorit celý objekt tak vyreslíme pomocou ciar
                        for (int j = 0; j < listOfPoints.Count - 1; j++)
                        {
                            var line = Line.DrawLine(listOfPoints[j], listOfPoints[j + 1], Color.red, 2000000);
                            lineList.Add(line);
                        }
                    }
                    listOfPoints.Clear();
                }
                if (up)
                {
                    i++;
                }
                else
                {
                    i--;
                }
            }
        }
        /// <summary>
        /// Zmaže duplicitné body
        /// </summary>
        /// <param name="listOfPoints">Body rezu</param>
        /// <returns></returns>
        private static void DeleteDuplicities(ref List<Vector3> listOfPoints)
        {
            for (int i = 0; i < listOfPoints.Count; i++)
            {
                if (Collisions.CompareVectorsWithDeviation(listOfPoints[i] ,(i < listOfPoints.Count - 1 ? listOfPoints[i + 1] : listOfPoints[0]),0.00001f))
                {
                    listOfPoints.RemoveAt(i);
                    i--;
                }
            }
        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="type">0=X,1=Y</param>
        /// <param name="p">point</param>
        /// <returns></returns>
        public static Vector3 GetPoint(int type, int p)
        {
            return (type == 0 ? AuxiliaryPlanes.X : AuxiliaryPlanes.Y).GetComponent<MeshFilter>().mesh.vertices[p];
        }

        private static Slice CreateSlice(List<Vector3> points, int type, int serial, Color color, Vector3 normal,Transform transform)
        {

            if (points.Count > 2)
            {

                Slice slice = new Slice(points, color, transform.rotation, "ID" + serial + "," + type, normal);
                //return complete slice
                return slice;

            }
            return null;
        }
    }
}
