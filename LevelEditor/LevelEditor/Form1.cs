using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;
using System.IO;
using Microsoft.DirectX.DirectInput;
using System.Windows.Forms;
using System.Diagnostics;

namespace LevelEditor
{
    public partial class Editor : Form
    {
        private int WIDTH = 64;
        private int HEIGHT = 64;

        private Microsoft.DirectX.Direct3D.Device device;
        private float angle = 0f;
        private CustomVertex.PositionColored[] vertices;
        private int[,] heightData;
        private int[] indices;
        private IndexBuffer ib;
        private VertexBuffer vb;
        private Microsoft.DirectX.DirectInput.Device keyb;

        public Editor()
        {
            InitializeComponent();
            LoadHeightData();
            InitializeDevice();
            InitializeKeyboard();
            InitializeCamera();
            VertexDeclaration();
            IndicesDeclaration();
            splitContainer1.BorderStyle = BorderStyle.None;
            splitContainer1.SplitterWidth = 1;
            splitContainer2.BorderStyle = BorderStyle.None;
            splitContainer2.SplitterWidth = 1;
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
            VertexDeclaration();
            IndicesDeclaration();
        }
        private void InitializeCamera()
        {
            device.Transform.Projection = Matrix.PerspectiveFovLH((float)Math.PI / 4.0f, (float)graphicsPanel1.Width / (float)graphicsPanel1.Height, 1f, 150f);
            device.Transform.View = Matrix.LookAtLH(new Vector3(0, -40, 50), new Vector3(0, -5, 0), new Vector3(0, 1, 0));
            device.RenderState.Lighting = false;
            device.RenderState.CullMode = Cull.None;
        }
        public void InitializeKeyboard()
        {
            keyb = new Microsoft.DirectX.DirectInput.Device(SystemGuid.Keyboard);
            keyb.SetCooperativeLevel(this, CooperativeLevelFlags.Background | CooperativeLevelFlags.NonExclusive);
            keyb.Acquire();
        }
        private void VertexDeclaration()
        {
            vb = new VertexBuffer(typeof(CustomVertex.PositionColored), WIDTH * HEIGHT, device, Usage.Dynamic | Usage.WriteOnly, CustomVertex.PositionColored.Format, Pool.Default);
            vertices = new CustomVertex.PositionColored[WIDTH * HEIGHT];

            for (int x = 0; x < WIDTH; x++)
            {

                for (int y = 0; y < HEIGHT; y++)
                {
                    vertices[x + y * WIDTH].Position = new Vector3(x, y, heightData[x, y]);
                    vertices[x + y * WIDTH].Color = Color.White.ToArgb();
                }
            }

            vb.SetData(vertices, 0, LockFlags.None);
        }
        private void IndicesDeclaration()
        {
            ib = new IndexBuffer(typeof(int), (WIDTH - 1) * (HEIGHT - 1) * 6, device, Usage.WriteOnly, Pool.Default);
            indices = new int[(WIDTH - 1) * (HEIGHT - 1) * 6];

            for (int x = 0; x < WIDTH - 1; x++)
            {

                for (int y = 0; y < HEIGHT - 1; y++)
                {
                    indices[(x + y * (WIDTH - 1)) * 6] = (x + 1) + (y + 1) * WIDTH;
                    indices[(x + y * (WIDTH - 1)) * 6 + 1] = (x + 1) + y * WIDTH;
                    indices[(x + y * (WIDTH - 1)) * 6 + 2] = x + y * WIDTH;

                    indices[(x + y * (WIDTH - 1)) * 6 + 3] = (x + 1) + (y + 1) * WIDTH;
                    indices[(x + y * (WIDTH - 1)) * 6 + 4] = x + y * WIDTH;
                    indices[(x + y * (WIDTH - 1)) * 6 + 5] = x + (y + 1) * WIDTH;
                }
            }
            ib.SetData(indices, 0, LockFlags.None);
        }

        private void Paint(object sender, PaintEventArgs e)
        {
            device.Clear(ClearFlags.Target, Color.Black, 1.0f, 0);

            device.BeginScene();
            device.VertexFormat = CustomVertex.PositionColored.Format;
            device.SetStreamSource(0, vb, 0);
            device.Indices = ib;
            
            device.Transform.World = Matrix.Translation(-HEIGHT / 2, -WIDTH / 2, 0) * Matrix.RotationZ(angle);
            device.DrawIndexedPrimitives(PrimitiveType.TriangleList, 0, 0, WIDTH * HEIGHT, 0, indices.Length / 3);
            device.EndScene();

            device.Present();
            ReadKeyboard();
        }
        private void ReadKeyboard()
        {
            KeyboardState keys = keyb.GetCurrentKeyboardState();
            if (keys[Key.RightArrow])
            {
                angle += 0.03f;
            }
            if (keys[Key.LeftArrow])
            {
                angle -= 0.03f;
            }
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

        private void timer1_Tick(object sender, EventArgs e)
        {
            graphicsPanel1.Invalidate();
        }
    }
}
