/*
 Autor: Michal Ondrejó
 Project: Bachelor Thesis - Tvorba řezů z 3D modelů pro vystřihovánky "sliceforms"
*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Assets
{
    //zapise pozadovane data do suboru
    public static class Test
    {
        static bool PrintEnabled=false;
        public static string text = string.Empty;
        public static string post = string.Empty;
        public static void NewLine()
        {
            text += '\n';
        }
        public static void NewData(params object[] data)
        {
            if (data.Length > 0)
            {
                text += data[0].ToString();
                for (int i = 1; i < data.Length; i++)
                {
                    text += '\t';
                    text += data[i].ToString();
                }
            }
            text += post;
            post = string.Empty;
        }
        public static void NewDataPost(params object[] data)
        {
            for (int i = 0; i < data.Length; i++)
            {
                post += '\t';
                post += data[i].ToString();
            }
            
        }
        public static void Reset()
        {
            text = string.Empty;
            post = string.Empty;
        }
        
        private static string path=@".\BP_Test.txt";
        public static void Print()
        {
            if (PrintEnabled)
            {
                try
                {
                    using (System.IO.StreamWriter file =
                       new System.IO.StreamWriter(path, true))
                    {
                        file.WriteLine(text);
                    }
                }
                catch
                {

                }
            }
            text = string.Empty;
        }

        internal static void SetPath(string Path)
        {
            path = Path;
        }
    }
}
