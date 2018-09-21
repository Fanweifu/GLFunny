using System;

namespace IGLObject
{
    //渲染对象基础定义
   

    /// <summary>
    /// 成像投影方式
    /// </summary>
    public enum ProjectionType
    {
        /// <summary>
        /// 透视
        /// </summary>
        Perspective,

        /// <summary>
        /// 正交
        /// </summary>
        Ortho
    }

    public interface ICamera : IShapeRPS
    {
        int Left { get; set; }

        int Top { get; set; }

        int Width { get; set; }

        int Height { get; set; }

        float Near { get; set; }

        float Far { get; set; }

        float Fov { get; set; }

        float Ratio { get; set; }

        IntPtr ProjectionMatrixPtr { get; }
        IntPtr ProjectionMatrixInversePtr { get; }

        IntPtr ViewMatrixPtr { get; }
        IntPtr ViewMatrixInversePtr { get; }

        //ProjectionType Projection { get; set; }

        void Begin();

        void End();

        void Init();

        void GetForwardVec(out float vx, out float vy, out float vz);

        void GetUpVec(out float vx, out float vy, out float vz);

        void GetRightVec(out float vx, out float vy, out float vz);

        void Move(float rightStep, float forwardStep, float upStep);
    }

    public interface ICameraControl : ICamera
    {
        void ProcMouseMove(int x, int y);

        void ProcMouseDrag(int x, int y);

        void ProcKeyDown(char c);

        void ProcMouseWheel(int delt);

        void MouseCoordToRay(int x, int y, out float rx, out float ry, out float rz);
    }
}