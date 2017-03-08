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
using System.Xml;

namespace LevelEditor
{
    public partial class Editor : Form
    {

        private Microsoft.DirectX.Direct3D.Device device;
        private float angleX, angleY, rotSpeed, dragSpeed;
        private Microsoft.DirectX.DirectInput.Device keyb;
        private List<ToolObject> objects = new List<ToolObject>();
        private List<ToolObject> higharchy = new List<ToolObject>();
        private List<ToolObjectColor> debugObjs = new List<ToolObjectColor>();
        private ToolObject oldSelected = null, selectedObject = null;
        private ToolObjectColor selectedCollider = null;
        private Stopwatch fpsTimer = new Stopwatch();
        private List<long> advMillisecond = new List<long>();
        private Vector3 cameraPos = new Vector3(0, 0, 0), prevHit = new Vector3(0, 0, 0), curHit = new Vector3(0, 0, 0);
        private Vector2 prevMouse, curMouse;
        private int selectedIndex = 0;
        private bool canMove = false, grab = false, snap = false;
        private string selectedName = string.Empty, colliderType = string.Empty;
        Matrix gizmoScale = Matrix.Identity;
        Matrix rotate = Matrix.Identity;

        public Editor()
        {
            InitializeComponent();
            InitializeDevice();
            InitializeKeyboard();
            InitializeCamera();
            objects.Add(new ToolObject(ref device));
            objects[0].Name = "Empty";
            objects.Add(new ToolObject("Assets\\Cube.obj", "Assets\\skybox.dds", ref device));
            objects.Add(new ToolObject("Assets\\Sphere.obj", ref device));
            for (int i = 0; i < objects.Count; i++)
            {
                Tree.Nodes[0].Nodes.Add(objects[i].Name);
            }
            debugObjs.Add(new ToolObjectColor(ref device));
            debugObjs[0].MakeGrid();
            debugObjs[0].ObjectColor = Color.GhostWhite;
            debugObjs[0].IsWireFrame = true;
            debugObjs.Add(new ToolObjectColor("Assets\\AxisGizmo.obj", ref device));
            splitContainer1.BorderStyle = BorderStyle.None;
            splitContainer1.SplitterWidth = 1;
            splitContainer2.BorderStyle = BorderStyle.None;
            splitContainer2.SplitterWidth = 1;
            fpsTimer.Start();
            rotSpeed = 0.005f;
            dragSpeed = 2.0f;
            angleX = angleY = 0;
            rotate = Matrix.Identity;
            Tree.Nodes[0].Expand();
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
            foreach(ToolObjectColor to in debugObjs)
            {
                to.VertexDeclaration();
                to.IndicesDeclaration();
            }
            foreach (ToolObject to in higharchy)
            {
                to.VertexDeclaration();
                to.IndicesDeclaration();
                if (to.Collider != null)
                {
                    to.Collider.VertexDeclaration();
                    to.Collider.IndicesDeclaration();
                }
            }
        }
        private void InitializeCamera()
        {
            device.Transform.Projection = Matrix.PerspectiveFovRH((float)Math.PI / 4.0f, (float)graphicsPanel1.Width / (float)graphicsPanel1.Height, 1f, 1000);
            Matrix rotate = Matrix.RotationYawPitchRoll(angleY, angleX, 0);
            Vector3 look = cameraPos + GetVector3(Vector3.Transform(new Vector3(0, 0, -1), rotate));
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
            //Grid
            device.VertexFormat = CustomVertex.PositionNormalColored.Format;
            device.RenderState.CullMode = Cull.None;
            device.SetTexture(0, null);
            device.RenderState.AlphaBlendEnable = false;
            device.RenderState.FillMode = debugObjs[0].IsWireFrame ? FillMode.WireFrame : FillMode.Solid;
            device.SetStreamSource(0, debugObjs[0].VertexBuffer, 0);
            device.Indices = debugObjs[0].IndexBuffer;
            device.Transform.World = debugObjs[0].Transform;
            device.DrawIndexedPrimitives(PrimitiveType.TriangleList, 0, 0, debugObjs[0].Indices.Length, 0, debugObjs[0].Indices.Length / 3);
            //Scene
            device.VertexFormat = CustomVertex.PositionNormalTextured.Format;
            device.RenderState.CullMode = Cull.Clockwise;
                    device.RenderState.AlphaBlendEnable = false;
            foreach (ToolObject tObj in higharchy)
            {
                if (tObj.Vertices != null)
                {
                    device.RenderState.FillMode = tObj.IsWireFrame ? FillMode.WireFrame : FillMode.Solid;
                    device.SetStreamSource(0, tObj.VertexBuffer, 0);
                    device.Indices = tObj.IndexBuffer;
                    device.SetTexture(0, tObj.Texture);
                    device.Transform.World = tObj.Transform;
                    device.DrawIndexedPrimitives(PrimitiveType.TriangleList, 0, 0, tObj.Indices.Length, 0, tObj.Indices.Length / 3);
                }
                if (tObj.Collider != null)
                {
                    device.VertexFormat = CustomVertex.PositionNormalColored.Format;
                    device.RenderState.CullMode = Cull.None;
                    device.RenderState.FillMode = tObj.Collider.IsWireFrame ? FillMode.WireFrame : FillMode.Solid;
                    device.SetStreamSource(0, tObj.Collider.VertexBuffer, 0);
                    device.Indices = tObj.Collider.IndexBuffer;
                    device.SetTexture(0, null);
                    if (tObj.ColliderType == "OBB")
                        device.Transform.World = tObj.Collider.Transform * tObj.Transform;
                    else
                        device.Transform.World = Matrix.Translation(tObj.Position) * tObj.Collider.Transform;
                    device.DrawIndexedPrimitives(PrimitiveType.TriangleList, 0, 0, tObj.Collider.Indices.Length, 0, tObj.Collider.Indices.Length / 3);
                    device.VertexFormat = CustomVertex.PositionNormalTextured.Format;
                    device.RenderState.CullMode = Cull.Clockwise;
                }
            }
            //Axis Gizmo
            if (selectedObject != null)
            {
                device.VertexFormat = CustomVertex.PositionNormalColored.Format;
                device.RenderState.CullMode = Cull.None;
                device.SetTexture(0, null);
                device.RenderState.AlphaBlendEnable = true;
                float scale = (cameraPos - selectedObject.Position).Length();
                gizmoScale = Matrix.Scaling(Vector3.Maximize(new Vector3(scale, scale, scale) * 0.05f, new Vector3(1,1,1)));
                foreach (ToolObjectColor tObj2 in debugObjs[1].Children)
                {
                    device.RenderState.FillMode = tObj2.IsWireFrame ? FillMode.WireFrame : FillMode.Solid;
                    device.SetStreamSource(0, tObj2.VertexBuffer, 0);
                    device.Indices = tObj2.IndexBuffer;
                    device.Transform.World = gizmoScale * Matrix.Translation(selectedObject.Position) * debugObjs[1].Transform * tObj2.Transform;
                    device.DrawIndexedPrimitives(PrimitiveType.TriangleList, 0, 0, tObj2.Indices.Length, 0, tObj2.Indices.Length / 3);
                }
            }
            device.EndScene();

            device.Present();
            ReadKeyboard();
        }

        private void graphicsPanel1_MouseLeave(object sender, EventArgs e)
        {
            canMove = false;
        }

        private void graphicsPanel1_MouseClick(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left && !grab)
            {
                Vector3 near, far, hit;
                hit = new Vector3(1000.0f, 1000.0f, 1000.0f);
                int selection = -1;

                if (selection == -1)
                {
                    hit = new Vector3(1000.0f, 1000.0f, 1000.0f);
                    for (int i = 0; i < higharchy.Count; i++)
                    {
                        Vector3 testHit = new Vector3();
                        near = new Vector3(curMouse.X, curMouse.Y, 0);
                        far = new Vector3(curMouse.X, curMouse.Y, 1);
                        near.Unproject(device.Viewport, device.Transform.Projection, device.Transform.View, higharchy[i].Transform);
                        far.Unproject(device.Viewport, device.Transform.Projection, device.Transform.View, higharchy[i].Transform);
                        if (higharchy[i].RayHit(out testHit, near, far))
                            if ((testHit - cameraPos).LengthSq() < (hit - cameraPos).LengthSq())
                            {
                                hit = testHit;
                                selection = i;
                            }
                    }
                    if (selection >= 0)
                    {
                        selectedObject = higharchy[selection];
                        selectedCollider = null;
                        selectedIndex = selection;
                        UpdateSelectedData();
                    }
                    else
                    {
                        selectedObject = null;
                        selectedCollider = null;
                        selectedIndex = selection;
                        UpdateSelectedData();
                    }
                }
            }
        }

        private Vector3 RaycastPlane(Vector3 near, Vector3 far, Vector3 position, Vector3 normal)
        {
            normal.Normalize();
            near.Unproject(device.Viewport, device.Transform.Projection, device.Transform.View, Matrix.Translation(position));
            far.Unproject(device.Viewport, device.Transform.Projection, device.Transform.View, Matrix.Translation(position));
            float D0 = Vector3.Dot(normal, near);
            float D1 = Vector3.Dot(normal, position);
            float D2 = D0 - D1;
            Vector3 L = far - near;
            float D3 = Vector3.Dot(normal, L);
            return near + (L * (-1.0f * (D2 / D3)));
        }

        private void graphicsPanel1_MouseMove(object sender, MouseEventArgs e)
        {
            canMove = true;
            prevMouse = curMouse;
            curMouse = new Vector2(e.X, e.Y);

            Vector3 near, far, hit;
            hit = new Vector3(1000.0f, 1000.0f, 1000.0f);
            int selection = -1;
            if (selectedObject != null && !grab)
            {
                for (int i = 0; i < debugObjs[1].Children.Count; i++)
                {
                    debugObjs[1].Children[i].Selected = false;
                    Vector3 testHit = new Vector3();
                    near = new Vector3(curMouse.X, curMouse.Y, 0);
                    far = new Vector3(curMouse.X, curMouse.Y, 1);
                    near.Unproject(device.Viewport, device.Transform.Projection, device.Transform.View, gizmoScale * Matrix.Translation(selectedObject.Position) * debugObjs[1].Children[i].Transform);
                    far.Unproject(device.Viewport, device.Transform.Projection, device.Transform.View, gizmoScale * Matrix.Translation(selectedObject.Position) * debugObjs[1].Children[i].Transform);
                    if (debugObjs[1].Children[i].Name != "PoleX" && debugObjs[1].Children[i].Name != "PoleY" && debugObjs[1].Children[i].Name != "PoleZ" && debugObjs[1].Children[i].RayHit(out testHit, near, far))
                        if ((testHit - cameraPos).LengthSq() < (hit - cameraPos).LengthSq())
                        {
                            hit = testHit;
                            selection = i;
                        }
                }
                if (selection >= 0)
                {
                    prevHit = curHit;
                    curHit = hit;
                    debugObjs[1].Children[selection].Selected = true;
                    selectedName = debugObjs[1].Children[selection].Name;
                }
            }
            float dx, dy;
            switch (e.Button)
            {
                case MouseButtons.Left:
                    if (selectedObject != null)
                    {
                        //this.Cursor = new Cursor(Cursor.Current.Handle);
                        //if (curMouse.X >= graphicsPanel1.Width)
                        //{
                        //    Cursor.Position = new Point(graphicsPanel1.Location.X + this.DesktopLocation.X, Cursor.Position.Y);
                        //}
                        //if (curMouse.Y >= graphicsPanel1.Width)
                        //{
                        //    Cursor.Position = new Point(Cursor.Position.X, graphicsPanel1.Top + this.DesktopLocation.Y);
                        //}

                        grab = true;
                        Vector3 dh;
                        near = new Vector3(curMouse.X, curMouse.Y, 0);
                        far = new Vector3(curMouse.X, curMouse.Y, 1);
                        Vector3 pos = selectedObject.Position;
                        switch (selectedName)
                        {
                            case "TranslateX":
                                prevHit = curHit;
                                curHit = RaycastPlane(near, far, selectedObject.Position, new Vector3(0, 1, 0));
                                dh = (prevHit - curHit) * 0.75f;
                                if (snap)
                                {
                                    dh = (prevHit - curHit);
                                    if (Math.Abs(Math.Round(dh.X, 1)) * 2 > 1)
                                    {
                                        selectedObject.SetPosition(new Vector3((float)Math.Floor(pos.X + 0.1f) + (-dh.X < 0 ? -1 : 1), pos.Y, pos.Z));
                                        prevHit = curHit;
                                    }
                                }
                                else
                                {
                                    dh = (prevHit - curHit) * 0.75f;
                                    selectedObject.Translate(new Vector3(-dh.X, 0, 0));
                                    prevHit = curHit;
                                }
                                break;
                            case "TranslateY":
                                prevHit = curHit;
                                curHit = RaycastPlane(near, far, selectedObject.Position, new Vector3(1, 0, 0));
                                dh = (prevHit - curHit) * 0.75f;
                                if (snap)
                                {
                                    dh = (prevHit - curHit);
                                    if (Math.Abs(Math.Round(dh.Y, 1)) * 2 > 1)
                                    {
                                        selectedObject.SetPosition(new Vector3(pos.X, (float)Math.Floor(pos.Y + 0.1f) + (-dh.Y < 0 ? -1 : 1), pos.Z));
                                        prevHit = curHit;
                                    }
                                }
                                else
                                {
                                    dh = (prevHit - curHit) * 0.75f;
                                    selectedObject.Translate(new Vector3(0, -dh.Y, 0));
                                    prevHit = curHit;
                                }
                                break;
                            case "TranslateZ":
                                curHit = RaycastPlane(near, far, selectedObject.Position, new Vector3(0, 1, 0));
                                dh = (prevHit - curHit) * 0.75f;
                                if (snap)
                                {
                                    dh = (prevHit - curHit);
                                    if (Math.Abs(Math.Round(dh.Z, 1)) * 2 > 1)
                                    {
                                        selectedObject.SetPosition(new Vector3(pos.X, pos.Y, (float)Math.Floor(pos.Z + 0.1f) + (-dh.Z < 0 ? -1 : 1)));
                                        prevHit = curHit;
                                    }
                                }
                                else
                                {
                                    dh = (prevHit - curHit) * 0.75f;
                                    selectedObject.Translate(new Vector3(0, 0, -dh.Z));
                                    prevHit = curHit;
                                }
                                break;
                            case "TranslateXZ":
                                break;
                            case "TranslateYZ":
                                break;
                            case "TranslateXY":
                                break;
                            case "ScaleX":
                                curHit = RaycastPlane(near, far, selectedObject.Position, new Vector3(0, 1, 0));
                                dh = (prevHit - curHit) * 0.75f;
                                selectedObject.AddScale(new Vector3(1 + -dh.X, 1, 1));
                                prevHit = curHit;
                                break;
                            case "ScaleY":
                                curHit = RaycastPlane(near, far, selectedObject.Position, new Vector3(1, 0, 0));
                                dh = (prevHit - curHit) * 0.75f;
                                selectedObject.AddScale(new Vector3(1, 1 + -dh.Y, 1));
                                prevHit = curHit;
                                break;
                            case "ScaleZ":
                                curHit = RaycastPlane(near, far, selectedObject.Position, new Vector3(0, 1, 0));
                                dh = (prevHit - curHit) * 0.75f;
                                selectedObject.AddScale(new Vector3(1, 1, 1 + -dh.Z));
                                prevHit = curHit;
                                break;
                            case "ScaleXYZ":
                                break;
                            case "RotateX":
                                curHit = RaycastPlane(near, far, selectedObject.Position, new Vector3(1, 0, 0));
                                dh = (prevHit - curHit) * 0.75f;
                                selectedObject.Rotate(new Vector3(dh.Y, 0, 0));
                                prevHit = curHit;
                                break;
                            case "RotateY":
                                curHit = RaycastPlane(near, far, selectedObject.Position, new Vector3(0, 1, 0));
                                dh = (prevHit - curHit) * 0.75f;
                                selectedObject.Rotate(new Vector3(0, dh.Z, 0));
                                prevHit = curHit;
                                break;
                            case "RotateZ":
                                curHit = RaycastPlane(near, far, selectedObject.Position, new Vector3(0, 0, 1));
                                dh = (prevHit - curHit) * 0.75f;
                                selectedObject.Rotate(new Vector3(0, 0, dh.X));
                                prevHit = curHit;
                                break;
                            default:
                                break;
                        }
                    }
                    break;
                case MouseButtons.None:
                    break;
                case MouseButtons.Right:
                    break;
                case MouseButtons.Middle:
                    dx = curMouse.X - prevMouse.X;
                    dy = curMouse.Y - prevMouse.Y;
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

        private Vector3 GetVector3(Vector4 v)
        {
            return new Vector3(v.X, v.Y, v.Z);
        }
        private void ReadKeyboard()
        {
            if (canMove)
            {
                Matrix pos = Matrix.Translation(cameraPos);
                Matrix rotate = Matrix.RotationYawPitchRoll(angleY, angleX, 0);
                Vector3 look = cameraPos + GetVector3(Vector3.Transform(new Vector3(0, 0, 1), rotate));
                device.Transform.View = Matrix.LookAtRH(cameraPos, look, new Vector3(0, 1, 0));
                KeyboardState keys = keyb.GetCurrentKeyboardState();
                if (keys[Key.W])
                    cameraPos += GetVector3(Vector3.Transform(new Vector3(0, 0, 0.5f), rotate));
                if (keys[Key.S])
                    cameraPos -= GetVector3(Vector3.Transform(new Vector3(0, 0, 0.5f), rotate));
                if (keys[Key.A])
                    cameraPos += GetVector3(Vector3.Transform(new Vector3(0.5f, 0, 0), rotate));
                if (keys[Key.D])
                    cameraPos -= GetVector3(Vector3.Transform(new Vector3(0.5f, 0, 0), rotate));
                if (keys[Key.LeftShift])
                    cameraPos.Y -= 0.5f;
                if (keys[Key.Space])
                    cameraPos.Y += 0.5f;
                snap = keys[Key.LeftAlt];
                if (selectedObject != null || selectedCollider != null)
                {
                    Vector3 dMove = new Vector3();
                    if (keys[Key.UpArrow])
                        dMove += (new Vector3(0, 0, 1));
                    if (keys[Key.Down])
                        dMove += (new Vector3(0, 0, -1));
                    if (keys[Key.Right])
                        dMove += (new Vector3(1, 0, 0));
                    if (keys[Key.Left])
                        dMove += (new Vector3(-1, 0, 0));
                    if (selectedCollider != null)
                        selectedCollider.Translate(dMove);
                    else if (selectedObject != null)
                        selectedObject.Translate(dMove);
                    UpdateSelectedData();
                }
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

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Stream myStream = null;
            OpenFileDialog openFileDialog1 = new OpenFileDialog();
            OpenFileDialog openFileDialog2 = new OpenFileDialog();

            openFileDialog1.InitialDirectory = Application.StartupPath;
            openFileDialog1.Filter = "Object files (*.obj)|*.obj";
            openFileDialog1.FilterIndex = 1;
            openFileDialog1.RestoreDirectory = true;

            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    openFileDialog2.InitialDirectory = Application.StartupPath;
                    openFileDialog2.Filter = "Texture files (*.png)|*.png";
                    openFileDialog2.FilterIndex = 1;
                    openFileDialog2.RestoreDirectory = true;
                    if (openFileDialog2.ShowDialog() == DialogResult.OK)
                    {
                        objects.Add(new ToolObject(openFileDialog1.FileName, openFileDialog2.FileName, ref device));
                        Tree.Nodes[0].Nodes.Add(objects.Last().Name);
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Error: Could not read file from disk. Original error: " + ex.Message);
                }
            }
        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog saveFile = new SaveFileDialog();
            saveFile.InitialDirectory = Application.StartupPath;
            saveFile.Filter = "XML files (*.xml)|*.xml";
            saveFile.FilterIndex = 1;
            saveFile.RestoreDirectory = true;
            if (saveFile.ShowDialog() == DialogResult.OK)
            {
                XmlWriterSettings settings = new XmlWriterSettings();
                settings.Indent = true;
                settings.OmitXmlDeclaration = true;
                using (XmlWriter writer = XmlWriter.Create(saveFile.FileName, settings))
                {
                    writer.WriteStartDocument();
                    writer.WriteStartElement("Level");
                    foreach (ToolObject tObj in higharchy)
                    {
                        writer.WriteStartElement("Object");
                        writer.WriteElementString("Name", tObj.Name);
                        if (tObj.MeshFile != null)
                            writer.WriteElementString("Mesh", tObj.MeshFile.Split('\\').Last());
                        if (tObj.TextureFile != null)
                            writer.WriteElementString("Texture", tObj.TextureFile == null ? "" : tObj.TextureFile.Split('\\').Last());
                        writer.WriteElementString("Position", tObj.Position.X + "," + tObj.Position.Y + "," + tObj.Position.Z);
                        writer.WriteElementString("Rotation", tObj.Rotation.X + "," + tObj.Rotation.Y + "," + tObj.Rotation.Z);
                        writer.WriteElementString("Scale", tObj.Scale.X + "," + tObj.Scale.Y + "," + tObj.Scale.Z);
                        writer.WriteStartElement("Components");
                        if (tObj.Collider != null)
                        {
                            writer.WriteStartElement("Collider");
                            writer.WriteElementString("Type", tObj.ColliderType);
                            writer.WriteElementString("Trigger", tObj.Collider.IsSolid ? "False" : "True");
                            writer.WriteElementString("Position", tObj.Collider.Position.X + "," + tObj.Collider.Position.Y + "," + tObj.Collider.Position.Z);
                            if (tObj.ColliderType == "OBB")
                            {
                                writer.WriteElementString("Rotation", tObj.Collider.Rotation.X + "," + tObj.Collider.Rotation.Y + "," + tObj.Collider.Rotation.Z);
                                writer.WriteElementString("Scale", tObj.Collider.Scale.X + "," + tObj.Collider.Scale.Y + "," + tObj.Collider.Scale.Z);
                            }
                            else
                                writer.WriteElementString("Radius", tObj.Collider.Scale.X.ToString());
                            writer.WriteEndElement();
                        }
                        writer.WriteEndElement();
                        writer.WriteEndElement();
                    }
                    writer.WriteEndElement();
                    writer.WriteEndDocument();
                }
            }
        }

        private void Trigger_CheckedChanged(object sender, EventArgs e)
        {
            selectedCollider.IsSolid = !Trigger.Checked;
        }

        private void componetsCheck_ItemCheck(object sender, ItemCheckEventArgs e)
        {
            if (selectedObject != null)
            {
                switch (e.Index)
                {
                    case 0:
                        if (selectedObject.Collider == null && e.NewValue == CheckState.Checked)
                        {
                            selectedObject.Collider = new ToolObjectColor("Assets\\Cube.obj", Color.Red, ref device);
                            selectedObject.ColliderType = "OBB";
                            selectedObject.Collider.IsWireFrame = true;
                            selectedObject.Collider.Name = "Collider";
                            Tree.Nodes[1].Nodes[selectedIndex].Nodes.Add("Collider");
                        }
                        if (selectedObject.Collider != null && e.NewValue == CheckState.Unchecked && selectedObject.ColliderType == "OBB")
                        {
                            selectedObject.Collider = null;
                            Tree.Nodes[1].Nodes[selectedIndex].Nodes.RemoveAt(0);
                        }
                        break;
                    case 1:
                        if (selectedObject.Collider == null && e.NewValue == CheckState.Checked)
                        {
                            selectedObject.Collider = new ToolObjectColor("Assets\\Sphere.obj", Color.Red, ref device);
                            selectedObject.ColliderType = "Sphere";
                            selectedObject.Collider.IsWireFrame = true;
                            selectedObject.Collider.Name = "Collider";
                            Tree.Nodes[1].Nodes[selectedIndex].Nodes.Add("Collider");
                        }
                        if (selectedObject.Collider != null && e.NewValue == CheckState.Unchecked && selectedObject.ColliderType == "Sphere")
                        {
                            selectedObject.Collider = null;
                            Tree.Nodes[1].Nodes[selectedIndex].Nodes.RemoveAt(0);
                        }
                        break;
                    default:
                        break;
                }
            }
            graphicsPanel1.Focus();
        }

        private void graphicsPanel1_MouseUp(object sender, MouseEventArgs e)
        {
            grab = false;
            selectedName = string.Empty;
        }

        private void Tree_Click(object sender, EventArgs e)
        {
            if (Tree.SelectedNode != null && Tree.SelectedNode.Parent != null && Tree.SelectedNode.Parent.Text == "Higharchy")
            {
                selectedIndex = Tree.SelectedNode.Index;
                selectedObject = higharchy[selectedIndex];
                selectedCollider = null;
                UpdateSelectedData();
            }
            else if (Tree.SelectedNode != null && Tree.SelectedNode.Parent != null && Tree.SelectedNode.Parent.Text != "Higharchy" && Tree.SelectedNode.Parent.Text != "Objects")
            {
                if (higharchy[Tree.SelectedNode.Parent.Index].Name == Tree.SelectedNode.Parent.Text)
                {
                    selectedIndex = Tree.SelectedNode.Parent.Index;
                    selectedObject = null;
                    selectedCollider = higharchy[Tree.SelectedNode.Parent.Index].Collider;
                    colliderType = higharchy[Tree.SelectedNode.Parent.Index].ColliderType;
                    UpdateSelectedData();
                }
            }
            graphicsPanel1.Focus();
        }

        private void Tree_DoubleClick(object sender, EventArgs e)
        {
            if (Tree.SelectedNode != null && Tree.SelectedNode.Parent != null && Tree.SelectedNode.Parent.Text == "Objects" && Tree.SelectedNode.Index < objects.Count)
            {
                higharchy.Add(new ToolObject(objects[Tree.SelectedNode.Index]));
                higharchy[higharchy.Count - 1].VertexDeclaration();
                higharchy[higharchy.Count - 1].IndicesDeclaration();
                Tree.Nodes[1].Nodes.Add(objects[Tree.SelectedNode.Index].Name);
                Tree.Nodes[1].Expand();
            }
            else if (Tree.SelectedNode != null && Tree.SelectedNode.Parent != null && Tree.SelectedNode.Parent.Text == "Higharchy" && Tree.SelectedNode.Index < higharchy.Count)
            {
                higharchy.Add(new ToolObject(higharchy[Tree.SelectedNode.Index]));
                higharchy[higharchy.Count - 1].VertexDeclaration();
                higharchy[higharchy.Count - 1].IndicesDeclaration();
                Tree.Nodes[1].Nodes.Add(higharchy[Tree.SelectedNode.Index].Name);
                Tree.Nodes[1].Expand();
            }
        }

        private void UpdateSelectedData()
        {
            if (selectedCollider != null)
            {
                groupBox5.Visible = false;
                Trigger.Visible = true;
                Trigger.Checked = !selectedCollider.IsSolid;
                nameBox.Text = selectedCollider.Name;
                posX.Value = (decimal)selectedCollider.Position.X;
                posY.Value = (decimal)selectedCollider.Position.Y;
                posZ.Value = (decimal)selectedCollider.Position.Z;

                rotX.Value = (decimal)(selectedCollider.Rotation.X * ToolObject.RADIANS_TO_DEGREES);
                rotY.Value = (decimal)(selectedCollider.Rotation.Y * ToolObject.RADIANS_TO_DEGREES);
                rotZ.Value = (decimal)(selectedCollider.Rotation.Z * ToolObject.RADIANS_TO_DEGREES);
                if (colliderType == "OBB")
                {
                    groupBox1.Visible = true;
                    groupBox6.Visible = false;
                    scaleX.Value = (decimal)selectedCollider.Scale.X;
                    scaleY.Value = (decimal)selectedCollider.Scale.Y;
                    scaleZ.Value = (decimal)selectedCollider.Scale.Z;
                }
                else
                {
                    groupBox1.Visible = false;
                    groupBox6.Visible = true;
                    Radius.Value = (decimal)selectedCollider.Scale.X;
                }
            }
            else if (selectedObject != null)
            {
                groupBox6.Visible = false;
                groupBox5.Visible = true;
                groupBox1.Visible = true;
                Trigger.Visible = false;
                nameBox.Text = selectedObject.Name;
                posX.Value = (decimal)selectedObject.Position.X;
                posY.Value = (decimal)selectedObject.Position.Y;
                posZ.Value = (decimal)selectedObject.Position.Z;

                rotX.Value = (decimal)(selectedObject.Rotation.X * ToolObject.RADIANS_TO_DEGREES);
                rotY.Value = (decimal)(selectedObject.Rotation.Y * ToolObject.RADIANS_TO_DEGREES);
                rotZ.Value = (decimal)(selectedObject.Rotation.Z * ToolObject.RADIANS_TO_DEGREES);

                scaleX.Value = (decimal)selectedObject.Scale.X;
                scaleY.Value = (decimal)selectedObject.Scale.Y;
                scaleZ.Value = (decimal)selectedObject.Scale.Z;
                if (selectedObject.Collider != null)
                {
                    componetsCheck.SetItemChecked(selectedObject.ColliderType == "OBB" ? 0 : 1, true);
                    componetsCheck.SetItemChecked(selectedObject.ColliderType != "OBB" ? 0 : 1, false);
                }
                else
                {
                    for (int i = 0; i < 3; i++)
                        componetsCheck.SetItemChecked(i, false);
                }
            }
            else
            {
                groupBox6.Visible = false;
                groupBox5.Visible = true;
                groupBox1.Visible = true;
                Trigger.Visible = false;
                componetsCheck.ClearSelected();
                for (int i = 0; i < 3; i++)
                    componetsCheck.SetItemChecked(i, false);
                nameBox.Text = string.Empty;
                posX.Value = 0;
                posY.Value = 0;
                posZ.Value = 0;

                rotX.Value = 0;
                rotY.Value = 0;
                rotZ.Value = 0;

                scaleX.Value = 0;
                scaleY.Value = 0;
                scaleZ.Value = 0;
            }
            if (oldSelected != selectedObject)
                oldSelected = selectedObject;
        }

        private void nameBox_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                e.Handled = e.SuppressKeyPress = true;
            }
        }

        private void transform_ValueChanged(object sender, EventArgs e)
        {
            if (selectedObject != null && oldSelected == selectedObject)
            {
                Tree.Nodes[1].Nodes[selectedIndex].Text = nameBox.Text;
                selectedObject.Name = nameBox.Text;
                selectedObject.SetPosition(new Vector3((float)posX.Value, (float)posY.Value, (float)posZ.Value));
                selectedObject.SetScale(new Vector3((float)scaleX.Value, (float)scaleY.Value, (float)scaleZ.Value));
                selectedObject.SetRotate(new Vector3((float)rotX.Value * ToolObject.DEGREES_TO_RADIANS, (float)rotY.Value * ToolObject.DEGREES_TO_RADIANS, (float)rotZ.Value * ToolObject.DEGREES_TO_RADIANS));
            }
            else if (selectedCollider != null && oldSelected == selectedObject)
            {
                Tree.Nodes[1].Nodes[selectedIndex].Nodes[0].Text = nameBox.Text;
                selectedCollider.Name = nameBox.Text;
                selectedCollider.SetPosition(new Vector3((float)posX.Value, (float)posY.Value, (float)posZ.Value));
                if (colliderType == "OBB")
                    selectedCollider.SetScale(new Vector3((float)scaleX.Value, (float)scaleY.Value, (float)scaleZ.Value));
                else
                    selectedCollider.SetScale(new Vector3((float)Radius.Value, (float)Radius.Value, (float)Radius.Value));
                selectedCollider.SetRotate(new Vector3((float)rotX.Value * ToolObject.DEGREES_TO_RADIANS, (float)rotY.Value * ToolObject.DEGREES_TO_RADIANS, (float)rotZ.Value * ToolObject.DEGREES_TO_RADIANS));
            }
        }


        private void supress_KeyDown(object sender, KeyPressEventArgs e)
        {
            int i = 0;
            if (!int.TryParse(e.KeyChar.ToString(), out i) && e.KeyChar != '\b')
            {
                e.Handled = true;
            }
        }

        private void Editor_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.T)
                LeftToggle_Click(sender, null);
            if (e.KeyCode == Keys.N)
                RightToggle_Click(sender, null);
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
