using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.Collections.Generic;
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;
using System.IO;
using Microsoft.DirectX.DirectInput;
using System.Diagnostics;

namespace LevelEditor
{
    public partial class Editor : Form
    {
        private int WIDTH = 64;
        private int HEIGHT = 64;

        private Microsoft.DirectX.Direct3D.Device device;
        private float angle, rotSpeed;
        private int[,] heightData;
        private CustomVertex.PositionColored[] vertices;
        private int[] indices;
        private IndexBuffer ib;
        private VertexBuffer vb;
        private Microsoft.DirectX.DirectInput.Device keyb;
        private List<ToolObject> objects = new List<ToolObject>();
        private Stopwatch fpsTimer = new Stopwatch();
        private List<long> advMillisecond = new List<long>();
        private Vector3 cameraPos = new Vector3(0, 0, 0);
        private Vector2 prevMouse, curMouse;
        Matrix rotate = Matrix.Identity;

        public Editor()
        {
            InitializeComponent();
            LoadHeightData();
            InitializeDevice();
            InitializeKeyboard();
            InitializeCamera();
            objects.Add(new ToolObject(ref device));
            objects.Add(new ToolObject("Assets/Cube.obj", ref device));
            objects.Add(new ToolObject("Assets/Sphere.obj", ref device));
            objects[0].Scale(new Vector3(10, 10, 10));
            objects[1].Scale(new Vector3(10, 10, 10));
            objects[1].Translate(new Vector3(0, 1, 0));
            objects[2].Scale(new Vector3(5, 5, 5));
            objects[2].Translate(new Vector3(3.0f, 1, -1.0f));
            splitContainer1.BorderStyle = BorderStyle.None;
            splitContainer1.SplitterWidth = 1;
            splitContainer2.BorderStyle = BorderStyle.None;
            splitContainer2.SplitterWidth = 1;
            fpsTimer.Start();
            rotSpeed = 0.01f;
        }
        private void InitializeDevice()
        {
            try
            {
                PresentParameters presentParams = new PresentParameters();
                presentParams.Windowed = true;
                presentParams.SwapEffect = SwapEffect.Discard;
                device = new Microsoft.DirectX.Direct3D.Device(0, Microsoft.DirectX.Direct3D.DeviceType.Hardware, this.graphicsPanel1, CreateFlags.SoftwareVertexProcessing, presentParams);
                device.RenderState.FillMode = FillMode.WireFrame;
                device.RenderState.CullMode = Cull.None;
                device.DeviceReset += new EventHandler(HandleResetEvent);
            }
            catch (GraphicsException exception)
            {

                //Trace is in System.Diagnostics
                //WriteLine outputs to the IDE's Output window
                Trace.WriteLine("Error Code:" + exception.ErrorCode);
                Trace.WriteLine("Error String:" + exception.ErrorString);
                Trace.WriteLine("Message:" + exception.Message);
                Trace.WriteLine("StackTrace:" + exception.StackTrace);
            }
        }
        private void HandleResetEvent(object caller, EventArgs args)
        {
            device.RenderState.FillMode = FillMode.WireFrame;
            device.RenderState.CullMode = Cull.None;
            InitializeCamera();
            foreach (ToolObject to in objects)
            {
                to.VertexDeclaration();
                to.IndicesDeclaration();
            }
        }
        private void InitializeCamera()
        {
            device.Transform.Projection = Matrix.PerspectiveFovLH((float)Math.PI / 4.0f, (float)graphicsPanel1.Width / (float)graphicsPanel1.Height, 1f, 1000);
            device.Transform.View = Matrix.LookAtLH(new Vector3(0, -40, 50), new Vector3(0, -5, 0), new Vector3(0, 1, 0)) * Matrix.RotationZ(angle);
            device.RenderState.Lighting = false;
            device.RenderState.CullMode = Cull.None;
        }
        public void InitializeKeyboard()
        {
            keyb = new Microsoft.DirectX.DirectInput.Device(SystemGuid.Keyboard);
            keyb.SetCooperativeLevel(this, CooperativeLevelFlags.Background | CooperativeLevelFlags.NonExclusive);
            keyb.Acquire();
        }

        private void Paint(object sender, PaintEventArgs e)
        {
            device.Clear(ClearFlags.Target, Color.Black, 1.0f, 0);

            device.BeginScene();
            device.VertexFormat = CustomVertex.PositionNormalTextured.Format;
            foreach (ToolObject tObj in objects)
            {
                device.SetStreamSource(0, tObj.VertexBuffer, 0);
                device.Indices = tObj.IndexBuffer;
            
                device.Transform.World = tObj.Transform;
                device.DrawIndexedPrimitives(PrimitiveType.TriangleList, 0, 0, tObj.Indices.Length, 0, tObj.Indices.Length / 3);
            }
            device.EndScene();

            device.Present();
            ReadKeyboard();
        }
        private void ReadKeyboard()
        {
            KeyboardState keys = keyb.GetCurrentKeyboardState();
            if (keys[Key.RightArrow])
                angle += 0.03f;
            if (keys[Key.LeftArrow])
                angle -= 0.03f;
            if (keys[Key.W])
                cameraPos.Z += 1;
            if (keys[Key.S])
                cameraPos.Z -= 1;
            if (keys[Key.A])
                cameraPos.X -= 1;
            if (keys[Key.D])
                cameraPos.X += 1;
            if (keys[Key.LeftShift])
                cameraPos.Y -= 1;
            if (keys[Key.Space])
                cameraPos.Y += 1;
            Matrix pos = Matrix.Translation(cameraPos);
            Vector4 temp = Vector3.Transform(new Vector3(0, 0, 1), rotate);
            Vector3 look = cameraPos + new Vector3(temp.X, temp.Y, temp.Z);
            device.Transform.View = Matrix.LookAtLH(cameraPos, look, new Vector3(0, 1, 0));
        }
        private void LoadHeightData()
        {
            heightData = new int[WIDTH, HEIGHT];

            FileStream fs = new FileStream("Assets/heightdata.raw", FileMode.Open, FileAccess.Read);
            BinaryReader r = new BinaryReader(fs);

            for (int i = 0; i < HEIGHT; i++)
            {

                for (int y = 0; y < WIDTH; y++)
                {
                    int height = (int)(r.ReadByte() / 50);
                    heightData[WIDTH - 1 - y, HEIGHT - 1 - i] = height;
                }
            }
            r.Close();
        }
        private void Resize(object sender, EventArgs e)
        {
            graphicsPanel1.Invalidate();
        }

        private void RightToggle_Click(object sender, EventArgs e)
        {
            RightToggle.Text = RightToggle.Text == "<" ? ">" : "<";
            splitContainer2.Panel2Collapsed = !splitContainer2.Panel2Collapsed;
        }

        private void LeftToggle_Click(object sender, EventArgs e)
        {
            LeftToggle.Text = LeftToggle.Text == "<" ? ">" : "<";
            splitContainer1.Panel1Collapsed = !splitContainer1.Panel1Collapsed;
        }

        private void graphicsPanel1_MouseClick(object sender, MouseEventArgs e)
        {
        }

        private void graphicsPanel1_MouseMove(object sender, MouseEventArgs e)
        {
            prevMouse = curMouse;
            curMouse = new Vector2(e.X, e.Y);
            switch (e.Button)
            {
                case MouseButtons.Left:
                    break;
                case MouseButtons.None:
                    break;
                case MouseButtons.Right:
                    break;
                case MouseButtons.Middle:
                    float dx = curMouse.X - prevMouse.X;
                    float dy = curMouse.Y - prevMouse.Y;
                    rotate *= Matrix.RotationY(dx * rotSpeed);
                    rotate *= Matrix.RotationX(dy * rotSpeed);
                    break;
                case MouseButtons.XButton1:
                    break;
                case MouseButtons.XButton2:
                    break;
                default:
                    break;
            }
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            fpsTimer.Stop();
            advMillisecond.Add(fpsTimer.ElapsedMilliseconds);
            if (advMillisecond.Count >= 5)
            {
                long adv = 0;
                foreach (long l in advMillisecond)
                    adv += l;
                adv /= advMillisecond.Count;
                FpsCount.Text = "FPS: " + (1000 / adv);
            }
            fpsTimer.Reset();
            fpsTimer.Start();
            graphicsPanel1.Invalidate();
        }
    }
}
