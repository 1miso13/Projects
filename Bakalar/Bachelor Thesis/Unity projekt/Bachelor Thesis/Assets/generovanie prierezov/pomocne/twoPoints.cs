/*
 Autor: Michal Ondrejó
 Project: Bachelor Thesis - Tvorba řezů z 3D modelů pro vystřihovánky "sliceforms"
*/
using Assets.generovanie_prierezov;
using Assets.generovanie_prierezov.pomocne;
using System.Collections.Generic;
using UnityEngine;


public struct twoPoints
{
    public Vector3 point1;
    public Vector3 point2;
    public Line line1;
    public Line line2;


    /**
     * Constructor pre twoPoints
     */
    public twoPoints(Vector3 point1, Line line1, Vector3 point2, Line line2)
    {
        this.point1 = point1;
        this.point2 = point2;
        this.line1 = line1;
        this.line2 = line2;
    }

    /// <summary>
    /// OBSOLETE!
    /// USE ONLY WITH Collisions.FindCollideOLD(Vector3[] PlanePoints, Mesh mesh)!!!
    /// </summary>
    /// <param name="twoPointsList"> Dvojbodovy list obsahuje body </param>
    /// <param name="pointsArray">list do ktoreho sa budu ukladat jednotlive body</param>
    /// <returns> Vrati True ak sa podarilo zo zadanych bodov vytvorit suvisli objekt</returns>
    static public bool createArrayFromTwoPointsOLD(List<twoPoints> twoPointsList, ref List<Vector3> pointsArray)
    {
        bool chybnyObjekt = true;
        List<Line> lineArray = new List<Line>();

        pointsArray.Add(twoPointsList[0].point1);
        pointsArray.Add(twoPointsList[0].point2);
        lineArray.Add(twoPointsList[0].line1);
        lineArray.Add(twoPointsList[0].line2);
        twoPointsList.RemoveAt(0);
        while (twoPointsList.Count > 0 && (!(Line.compareTwoLines(lineArray[0], lineArray[lineArray.Count - 1])) && pointsArray[0] != pointsArray[pointsArray.Count - 1])/* && !chybnyObjekt*/)
        {
            chybnyObjekt = false;
            bool NotFound = true;
            for (int i = 0; i < twoPointsList.Count && NotFound; i++)
            {

                //if (twoPointsList[i].point1 == pointsArray[pointsArray.Count - 1])
                if (Line.compareTwoLines(twoPointsList[i].line1, lineArray[lineArray.Count - 1]))
                {

                    lineArray.Add(twoPointsList[i].line2);
                    pointsArray.Add(twoPointsList[i].point2);
                    twoPointsList.RemoveAt(i);
                    NotFound = false;
                    break;
                }
                // if (twoPointsList[i].point2 == pointsArray[pointsArray.Count - 1])
                if (Line.compareTwoLines(twoPointsList[i].line2, lineArray[lineArray.Count - 1]))
                {

                    lineArray.Add(twoPointsList[i].line1);
                    pointsArray.Add(twoPointsList[i].point1);
                    twoPointsList.RemoveAt(i);
                    NotFound = false;
                    break;
                }


            }
            for (int i = 0; i < twoPointsList.Count && NotFound; i++)
            {
                if (Collisions.CompareVectorsWithDeviation(pointsArray[pointsArray.Count - 1], twoPointsList[i].point1, 0.001f))
                {
                    lineArray.Add(twoPointsList[i].line2);
                    pointsArray.Add(twoPointsList[i].point2);
                    twoPointsList.RemoveAt(i);
                    NotFound = false;
                    break;
                }
                if (Collisions.CompareVectorsWithDeviation(pointsArray[pointsArray.Count - 1], twoPointsList[i].point2, 0.001f))
                {
                    lineArray.Add(twoPointsList[i].line1);
                    pointsArray.Add(twoPointsList[i].point1);
                    twoPointsList.RemoveAt(i);
                    NotFound = false;
                    break;
                }
            }


            if (NotFound)
            {
                chybnyObjekt = true;
                //nenaslo sa pokracovanie - nieje ukonceny objekt
                break;
            }

        }
        if (!(Line.compareTwoLines(lineArray[0], lineArray[lineArray.Count - 1])) && pointsArray[0] != pointsArray[pointsArray.Count - 1])
        {
            chybnyObjekt = true;
        }
        if (pointsArray.Count < 3)
        {
            chybnyObjekt = true;
            //nenaslo sa pokracovanie - nieje ukonceny objekt

        }
        if (pointsArray[0] == pointsArray[pointsArray.Count - 1])
        {
            pointsArray.RemoveAt(pointsArray.Count - 1);
        }
        return !chybnyObjekt;
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="twoPointsList">Dvojbodovy list obsahuje body </param>
    /// <param name="pointsArray">list do ktoreho sa budu ukladat jednotlive body</param>
    /// <returns>Vrati True ak sa podarilo zo zadanych bodov vytvorit suvisli objekt</returns>
    static public bool createArrayFromTwoPoints(List<twoPoints> twoPointsList, ref List<Vector3> pointsArray)
    {
        bool chybnyObjekt = true;
        List<Line> lineArray = new List<Line>();
        if (twoPointsList.Count<1)
        {
            return false;
        }
        pointsArray.Add(twoPointsList[0].point1);
        pointsArray.Add(twoPointsList[0].point2);
        lineArray.Add(twoPointsList[0].line1);
        lineArray.Add(twoPointsList[0].line2);
        twoPointsList.RemoveAt(0);
        while (twoPointsList.Count > 0 && pointsArray[0] != pointsArray[pointsArray.Count - 1])
        {
            chybnyObjekt = false;
            bool NotFound = true;
            
            for (int i = 0; i < twoPointsList.Count && NotFound; i++)
            {
                if (Collisions.CompareVectorsWithDeviation(pointsArray[pointsArray.Count - 1], twoPointsList[i].point1, 0.00001f))
                {
                    if (!Collisions.CompareVectorsWithDeviation(pointsArray[pointsArray.Count - 2], twoPointsList[i].point2, 0.00001f))
                    {//proti duplicitam
                        lineArray.Add(twoPointsList[i].line2);
                        pointsArray.Add(twoPointsList[i].point2);
                        twoPointsList.RemoveAt(i);
                        NotFound = false;
                        break;
                    }
                }
                if (Collisions.CompareVectorsWithDeviation(pointsArray[pointsArray.Count - 1], twoPointsList[i].point2, 0.00001f))
                {
                    if (!Collisions.CompareVectorsWithDeviation(pointsArray[pointsArray.Count - 2], twoPointsList[i].point1, 0.00001f))
                    {//proti duplicitam
                        lineArray.Add(twoPointsList[i].line1);
                        pointsArray.Add(twoPointsList[i].point1);
                        twoPointsList.RemoveAt(i);
                        NotFound = false;
                        break;
                    }
                }
            }


            if (NotFound)
            {
                chybnyObjekt = true;
                //nenaslo sa pokracovanie - nieje ukonceny objekt
                break;
            }

        }
        if (pointsArray[0] != pointsArray[pointsArray.Count - 1])
        {
            chybnyObjekt = true;
        }
        if (pointsArray.Count < 3)
        {
            chybnyObjekt = true;
            //nenaslo sa pokracovanie - nieje ukonceny objekt

        }
        if (pointsArray[0] == pointsArray[pointsArray.Count - 1])
        {
            pointsArray.RemoveAt(pointsArray.Count - 1);
        }
        return !chybnyObjekt;
    }
}

