using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;

namespace MojEngine
{
    public class Utility
    {
        public static Matrix CreateWorld(Vector3 posun)
        {
            return Matrix.CreateTranslation(posun);
        }

        public static Matrix CreateWorld(Vector3 posun, Matrix rotace)
        {
            return rotace * Matrix.CreateTranslation(posun);
        }
        public static Matrix CreateWorld(Vector3 posun, Matrix rotace, Vector3 meritko)
        {
            return Matrix.CreateScale(meritko) * rotace * Matrix.CreateTranslation(posun);
        }
        public static Matrix CreateRotation(Vector3 rotace)
        {
            return Matrix.CreateFromYawPitchRoll(rotace.Y, rotace.X, rotace.Z);
        }


    }
}
