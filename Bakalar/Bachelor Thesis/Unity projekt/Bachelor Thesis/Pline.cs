using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

namespace Pline
{
    class Pline
    {
        public Vector3 X;
        public Vector3 Y;
        public Vector3 Z;
        private Vector3 Normal;
        Pline(Vector3 X, Vector3 Y, Vector3 Z)
        {
            this.X = X;
            this.Y = Y;
            this.Z = Z;

            Normal = Vector3.Normalize(Vector3.Cross(Y - X, Z - X));
        }



    }
}
