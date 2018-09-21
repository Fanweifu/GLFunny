using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace IGLObject
{
    public interface IShape
    {
        bool DrawAxis { get; set; }
        bool Visible { get; set; }

        void Draw();
    }

    /// <summary>
    ///支持Transform
    /// </summary>
    public interface IShapeRPS : IShape
    {
        float PosX { get; set; }
        float PosY { get; set; }
        float PosZ { get; set; }

        void SetPos(float x, float y, float z);

        float RotX { get; set; }
        float RotY { get; set; }
        float RotZ { get; set; }

        void SetRot(float x, float y, float z);

        float SclX { get; set; }
        float SclY { get; set; }
        float SclZ { get; set; }

        void SetScl(float x, float y, float z);

        IntPtr ModelMatrixPtr { get; }
        IntPtr ModelMatrixInversePtr { get; }

        void ToLocalCoord(ref float x, ref float y, ref float z);

        void ToWorldCoord(ref float x, ref float y, ref float z);
    }

    public interface IMesh : IShapeRPS {

        bool IsEmpty { get; }
        void AddVertex(float px, float py, float pz, float texcoordX, float texcoordY, float nx=1f, float ny=1f, float nz=1f, float r=1f, float g=1f, float b=1f, float a=1f);
        void RemoveAt(int index);
        void Clear();
        void GenCube(float x1 = -1, float x2 = 1, float y1 = -1, float y2 = 1, float z1 = -1, float z2 = 1);
        void GenQuad(float x1 = -1, float x2 = 1, float y1 = -1, float y2 = 1);
    }

}
