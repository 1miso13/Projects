/*
 Autor: Michal Ondrejó
 Project: Bachelor Thesis - Tvorba řezů z 3D modelů pro vystřihovánky "sliceforms"
*/
using System.Collections.Generic;
using UnityEngine;

namespace Assets.generovanie_prierezov
{
    public static class SliceColor
    {
        /// <summary>
        /// Nastavenie farby pre jednotlive prierezy
        /// </summary>
        /// <param name="list">List s rezmi</param>
        /// <param name="startColor">Pociatocna farba</param>
        /// <param name="endColor">Koncova farba</param>
        public static void SetColorOfSlices(List<Slice> list, Color startColor, Color endColor)
        {
            if (list.Count != 0)
            {
                list[list.Count - 1].SetColor(endColor);
                for (int i = 0; i < list.Count; i++)
                {
                    //Debug.Log((float)i / listX.Count);
                    list[i].SetColor(Color.Lerp(startColor, endColor, ((float)i) / ((float)list.Count - 1)));
                }
            }
        }
    }
}
