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
using System.Linq;

namespace LevelEditor
{
    public partial class Editor : Form
    {

        private Microsoft.DirectX.Direct3D.Device device;
        private float angleX, angleY, rotSpeed;
        private Microsoft.DirectX.DirectInput.Device keyb;
        private List<ToolObject> objects = new List<ToolObject>();
        private List<ToolObject> higharchy = new List<ToolObject>();
        private ToolObject oldSelected = null, selectedObject = null;
        private Stopwatch fpsTimer = new Stopwatch();
        private List<long> advMillisecond = new List<long>();
        private Vector3 cameraPos = new Vector3(0, 0, 0);
        private Vector2 prevMouse, curMouse;
        Matrix rotate = Matrix.Identity;

        public Editor()
        {
            InitializeComponent();
            InitializeDevice();
            InitializeKeyboard();
            InitializeCamera();
            objects.Add(new ToolObject("Assets\\Cube.obj", "Assets\\skybox.dds", ref device));
            objects.Add(new ToolObject("Assets\\Sphere.obj", ref device));
            for (int i = 0; i < objects.Count; i++)
            {
                Tree.Nodes[0].Nodes.Add(objects[i].Name);
            }
            higharchy.Add(new ToolObject(ref device));
            splitContainer1.BorderStyle = BorderStyle.None;
            splitContainer1.SplitterWidth = 1;
            splitContainer2.BorderStyle = BorderStyle.None;
            splitContainer2.SplitterWidth = 1;
            fpsTimer.Start();
            rotSpeed = 0.005f;
            angleX = angleY = 0;
            rotate = Matrix.Identity;
        }
        private void InitializeDevice()
        {
            try
            {
                PresentParameters presentParams = new PresentParameters();
                presentParams.Windowed = true;
                presentParams.SwapEffect = SwapEffect.Discard;
                device = new Microsoft.DirectX.Direct3D.Device(0, Microsoft.DirectX.Direct3D.DeviceType.Hardware, this.graphicsPanel1, CreateFlags.SoftwareVertexProcessing, presentParams);
                device.RenderState.FillMode = FillMode.Solid;
                device.RenderState.CullMode = Cull.Clockwise;
                device.RenderState.ZBufferEnable = true;
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
            device.RenderState.FillMode = FillMode.Solid;
            device.RenderState.CullMode = Cull.Clockwise;
            device.RenderState.ZBufferEnable = true;
            InitializeCamera();
            foreach (ToolObject to in higharchy)
            {
                to.VertexDeclaration();
                to.IndicesDeclaration();
            }
        }
        private void InitializeCamera()
        {
            device.Transform.Projection = Matrix.PerspectiveFovRH((float)Math.PI / 4.0f, (float)graphicsPanel1.Width / (float)graphicsPanel1.Height, 1f, 1000);
            Matrix rotate = Matrix.RotationYawPitchRoll(angleY, angleX, 0);
            Vector3 look = cameraPos + GetVector3(Vector3.Transform(new Vector3(0, 0, 1), rotate));
            device.Transform.View = Matrix.LookAtRH(cameraPos, look, new Vector3(0, 1, 0));
            device.RenderState.Lighting = false;
                device.RenderState.ZBufferEnable = true;
            device.RenderState.CullMode = Cull.Clockwise;
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
            foreach (ToolObject tObj in higharchy)
            {
                device.RenderState.FillMode = tObj.IsWireFrame ? FillMode.WireFrame : FillMode.Solid;
                device.SetStreamSource(0, tObj.VertexBuffer, 0);
                device.Indices = tObj.IndexBuffer;
                device.SetTexture(0, tObj.Texture);
                device.Transform.World = tObj.Transform;
                device.DrawIndexedPrimitives(PrimitiveType.TriangleList, 0, 0, tObj.Indices.Length, 0, tObj.Indices.Length / 3);
            }
            device.EndScene();

            device.Present();
            ReadKeyboard();
        }
        private Vector3 GetVector3(Vector4 v)
        {
            return new Vector3(v.X, v.Y, v.Z);
        }
        private void ReadKeyboard()
        {
            if (true)
            {
                Matrix pos = Matrix.Translation(cameraPos);
                Matrix rotate = Matrix.RotationYawPitchRoll(angleY, angleX, 0);
                Vector3 look = cameraPos + GetVector3(Vector3.Transform(new Vector3(0, 0, 1), rotate));
                device.Transform.View = Matrix.LookAtRH(cameraPos, look, new Vector3(0, 1, 0));
                KeyboardState keys = keyb.GetCurrentKeyboardState();
                //objects[1].SetPosition(cameraPos);
                if (keys[Key.W])
                    cameraPos += GetVector3(Vector3.Transform(new Vector3(0, 0, 1), rotate));
                if (keys[Key.S])
                    cameraPos -= GetVector3(Vector3.Transform(new Vector3(0, 0, 1), rotate));
                if (keys[Key.A])
                    cameraPos += GetVector3(Vector3.Transform(new Vector3(1, 0, 0), rotate));
                if (keys[Key.D])
                    cameraPos -= GetVector3(Vector3.Transform(new Vector3(1, 0, 0), rotate));
                if (keys[Key.LeftShift])
                    cameraPos.Y -= 1;
                if (keys[Key.Space])
                    cameraPos.Y += 1;
            }
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

        private void Tree_Click(object sender, EventArgs e)
        {
            if (Tree.SelectedNode != null && Tree.SelectedNode.Parent != null && Tree.SelectedNode.Parent.Text == "Higharchy")
            {
                for (int i = 0; i < higharchy.Count; i++)
                {
                    if (Tree.SelectedNode.Text == higharchy[i].Name)
                    {
                        selectedObject = higharchy[i];
                        UpdateSelectedData();
                    }
                }
            }
            graphicsPanel1.Focus();
        }

        private void UpdateSelectedData()
        {
            if (oldSelected != selectedObject)
            {
                nameBox.Text = selectedObject.Name;
                posX.Value = (decimal)selectedObject.Position.X;
                posY.Value = (decimal)selectedObject.Position.Y;
                posZ.Value = (decimal)selectedObject.Position.Z;

                rotX.Value = (decimal)selectedObject.Rotation.X;
                rotY.Value = (decimal)selectedObject.Rotation.Y;
                rotZ.Value = (decimal)selectedObject.Rotation.Z;

                scaleX.Value = (decimal)selectedObject.Scale.X;
                scaleY.Value = (decimal)selectedObject.Scale.Y;
                scaleZ.Value = (decimal)selectedObject.Scale.Z;
                oldSelected = selectedObject;
            }
        }

        private void transform_ValueChanged(object sender, EventArgs e)
        {
            if (selectedObject != null && oldSelected == selectedObject)
            {
                selectedObject.Name = nameBox.Text;
                selectedObject.SetPosition(new Vector3((float)posX.Value, (float)posY.Value, (float)posZ.Value));
                selectedObject.SetScale(new    Vector3((float)scaleX.Value, (float)scaleY.Value, (float)scaleZ.Value));
                selectedObject.SetRotate(new   Vector3((float)rotX.Value, (float)rotY.Value, (float)rotZ.Value));
                selectedObject.VertexDeclaration();
                selectedObject.IndicesDeclaration();
            }
        }

        private void Tree_DoubleClick(object sender, EventArgs e)
        {
            for (int i = 0; i < objects.Count; i++)
            {
                if (Tree.SelectedNode != null && Tree.SelectedNode.Parent != null && Tree.SelectedNode.Parent.Text == "Objects" && Tree.SelectedNode.Text == objects[i].Name)
                {
                    higharchy.Add(new ToolObject(objects[i]));
                    higharchy[higharchy.Count - 1].VertexDeclaration();
                    higharchy[higharchy.Count - 1].IndicesDeclaration();
                    Tree.Nodes[1].Nodes.Add(objects[i].Name);
                    break;
                } 

            }
        }

        private void supress_KeyDown(object sender, KeyEventArgs e)
        {
            e.SuppressKeyPress = true;
        }

        private void Editor_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.T)
                LeftToggle_Click(sender, null);
            if (e.KeyCode == Keys.N)
                RightToggle_Click(sender, null);
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
                    angleY += -dx * rotSpeed;
                    angleX += dy * rotSpeed;
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
