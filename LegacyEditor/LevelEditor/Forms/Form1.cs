using System;
using System.Drawing;
using System.Windows.Forms;
using System.Collections.Generic;
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;
using Microsoft.DirectX.DirectInput;
using System.Diagnostics;


namespace LevelEditor
{
    public partial class Editor : Form
    {

        public const float RADIANS_TO_DEGREES = ((180.0f / 3.14f)); 
        public const float DEGREES_TO_RADIANS = (1 / 180.0f * 3.14f);
        private Microsoft.DirectX.Direct3D.Device device;
        private float angleX, angleY, rotSpeed, dragSpeed;
        private Microsoft.DirectX.DirectInput.Device keyb;
        private List<ToolObject> objects = new List<ToolObject>();
        private List<ToolObject> higharchy = new List<ToolObject>();
        private List<ToolObjectColor> debugObjs = new List<ToolObjectColor>();
        private ToolObject oldSelected = null, selectedObject = null;
        private ToolObjectColor selectedCollider = null, oldSelectedCollider = null;
        private Stopwatch fpsTimer = new Stopwatch();
        private List<long> advMillisecond = new List<long>();
        private Vector3 cameraPos = new Vector3(0, 0, 0), prevHit = new Vector3(0, 0, 0), curHit = new Vector3(0, 0, 0);
        private Vector2 prevMouse, curMouse;
        private Vector3 mStartPos, mStartRot;
        private int selectedIndex = 0;
        private bool canMove = false, grab = false, snap = false, loaded = true;
        private string selectedName = string.Empty, colliderType = string.Empty, currentFile = string.Empty;
        private Texture defaultTexture;
        Matrix gizmoScale = Matrix.Identity;
        Matrix rotate = Matrix.Identity;

        // Variables added by Drew
        private string mCurrentFilename = string.Empty;
        private bool mCurrentFileChanged = false;
        private TestPositionForm mForm;
        private string mSoundbank = string.Empty;


        string Filename {
            get {
                return mCurrentFilename;
            } 
            set {
                SetFilepath(value);
            }
        }

        bool FileChanged {
            get {
                return mCurrentFileChanged;
            }
            set {
                SetFileEdited(value);
            }
        }


        public Editor()
        {
            InitializeComponent();
            InitializeDevice();
            InitializeKeyboard();
            InitializeCamera();
            defaultTexture = TextureLoader.FromFile(device, "Assets\\default.dds");
            objects.Add(new ToolObject(ref device));
            objects[0].Name = "Empty";
            objects.Add(new ToolObject("Assets\\Cube.obj", ref device));
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
            spHierarchyPanel.BorderStyle = BorderStyle.None;
            spHierarchyPanel.SplitterWidth = 1;
            splitContainer2.BorderStyle = BorderStyle.None;
            splitContainer2.SplitterWidth = 8;
            fpsTimer.Start();
            rotSpeed = 0.005f;
            dragSpeed = 2.0f;
            angleX = angleY = 0;
            rotate = Matrix.Identity;
            Tree.Nodes[0].Expand();

            // Level settings McBootleg
            mStartPos = new Vector3(0, 0, 0);
            mStartRot = new Vector3(0, 0, 0);

            mForm = new TestPositionForm();
            mForm.Show();
        }
        private void InitializeDevice()
        {
            try
            {
                PresentParameters presentParams = new PresentParameters();
                presentParams.Windowed = true;
                presentParams.SwapEffect = SwapEffect.Copy;
                presentParams.EnableAutoDepthStencil = true;
                presentParams.AutoDepthStencilFormat = DepthFormat.D16;
                device = new Microsoft.DirectX.Direct3D.Device(0, Microsoft.DirectX.Direct3D.DeviceType.Hardware, this.graphicsPanel1, CreateFlags.SoftwareVertexProcessing, presentParams);
                device.RenderState.FillMode = FillMode.Solid;
                device.RenderState.CullMode = Cull.Clockwise;
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
            device.RenderState.ZBufferEnable = true;
            device.Clear(ClearFlags.ZBuffer, 0, 1.0f, 0);
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
            if (loaded)
            {
                //Scene
                device.RenderState.AlphaBlendEnable = false;
                foreach (ToolObject tObj in higharchy)
                {
                    if (tObj.Vertices != null)
                    {
                        device.VertexFormat = CustomVertex.PositionNormalTextured.Format;
                        device.RenderState.CullMode = Cull.Clockwise;
                        device.RenderState.FillMode = tObj.IsWireFrame ? FillMode.WireFrame : FillMode.Solid;
                        device.SetStreamSource(0, tObj.VertexBuffer, 0);
                        device.Indices = tObj.IndexBuffer;
                        if (tObj.Texture == null)
                            device.SetTexture(0, defaultTexture);
                        else
                            device.SetTexture(0, tObj.Texture);
                        device.Transform.World = tObj.Transform;
                        device.DrawIndexedPrimitives(PrimitiveType.TriangleList, 0, 0, tObj.Indices.Length, 0, tObj.Indices.Length / 3);
                    }
                    if (tObj.Collider != null && tObj.Collider.Visible == true)
                    {
                        device.VertexFormat = CustomVertex.PositionNormalColored.Format;
                        device.RenderState.CullMode = Cull.None;
                        device.RenderState.FillMode = tObj.Collider.IsWireFrame ? FillMode.WireFrame : FillMode.Solid;
                        device.SetStreamSource(0, tObj.Collider.VertexBuffer, 0);
                        device.Indices = tObj.Collider.IndexBuffer;
                        device.SetTexture(0, null);
                        if (tObj.ColliderType == "Sphere")
                            device.Transform.World = Matrix.Translation(tObj.Position) * tObj.Collider.Transform;
                        else
                            device.Transform.World = tObj.Collider.Transform * tObj.Transform;
                        device.DrawIndexedPrimitives(PrimitiveType.TriangleList, 0, 0, tObj.Collider.Indices.Length, 0, tObj.Collider.Indices.Length / 3);
                    }
                }
            }
            //Axis Gizmo
            device.Clear(ClearFlags.ZBuffer, 0, 1.0f, 0);
            if (selectedObject != null)
            {
                device.VertexFormat = CustomVertex.PositionNormalColored.Format;
                device.RenderState.CullMode = Cull.None;
                device.SetTexture(0, null);
                device.RenderState.AlphaBlendEnable = true;
                float scale = (cameraPos - selectedObject.Position).Length();
                gizmoScale = Matrix.Scaling(Vector3.Maximize(new Vector3(scale, scale, scale) * 0.05f, new Vector3(0.05f, 0.05f, 0.05f)));
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
            device.RenderState.ZBufferEnable = false;
            device.RenderState.UseWBuffer = true;
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
                        mForm.SetObject(selectedObject.mComponentList);
                        selectedCollider = null;
                        selectedIndex = selection;
                        Debug.Print("Grabby hands!");
                        UpdateSelectedData();
                    }
                    else
                    {
                        selectedObject = null;
                        mForm.SetObject(null);
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


                        Matrix ViewMatrix = device.Transform.View;
                        ViewMatrix.Invert();
                        Vector2 mouseDirection = curMouse - prevMouse;
                        Vector4 mouse4 = new Vector4(mouseDirection.X, mouseDirection.Y, 0, 0);
                        mouse4.Transform(ViewMatrix);
                        mouse4.Normalize();

                        Vector3 dir3 = new Vector3(mouse4.X, mouse4.Y, mouse4.Z);
                        float Magnitude = mouseDirection.Length() * 0.01f * gizmoScale.M11;
                        dir3.Normalize();
                        mouseDirection.Normalize();
                        float dotProduct = 0;
                        switch (selectedName)
                        {
                            case "TranslateX":
                                Vector4 left4 = new Vector4(1, 0, 0, 0);
                                left4.Transform(ViewMatrix);

                                Vector3 left = new Vector3(left4.X, left4.Y, left4.Z);
                                dotProduct = Vector3.Dot(left, dir3);
                                if (Math.Abs(dotProduct) >= 0.9) {
                                    // Going in the same direction, mostly.
                                    selectedObject.Translate(new Vector3(Magnitude * Math.Sign(mouse4.X), 0, 0));
                                }
                                break;
                            case "TranslateY":
                                Vector4 up4 = new Vector4(0, 1, 0, 0);
                                up4.Transform(ViewMatrix);

                                Vector3 up = new Vector3(up4.X, up4.Y, up4.Z);
                                dotProduct = Vector3.Dot(up, dir3);
                                if (Math.Abs(dotProduct) >= 0.9f) {
                                    selectedObject.Translate(new Vector3(0, Magnitude * -Math.Sign(mouse4.Y), 0));
                                }
                           
                                break;
                            case "TranslateZ":
                                Vector4 forward4 = new Vector4(1, 0, 0, 0);
                                forward4.Transform(ViewMatrix);

                                Vector3 forward = new Vector3(forward4.X, forward4.Y, forward4.Z);
                                dotProduct = Vector3.Dot(forward, dir3);
                                if (Math.Abs(dotProduct) >= 0.9f) {
                                    selectedObject.Translate(new Vector3(0, 0, Magnitude * Math.Sign(mouse4.Z)));
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
                    Debug.Print("Mouse move on graphics panel.");
                    UpdateSelectedData();
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
            if(Form.ActiveForm != this) {
                return;
            }
            KeyboardState keys = keyb.GetCurrentKeyboardState();
            if (canMove)
            {
                Matrix pos = Matrix.Translation(cameraPos);
                Matrix rotate = Matrix.RotationYawPitchRoll(angleY, angleX, 0);
                Vector3 look = cameraPos + GetVector3(Vector3.Transform(new Vector3(0, 0, 1), rotate));
                device.Transform.View = Matrix.LookAtRH(cameraPos, look, new Vector3(0, 1, 0));
                float MovementScale = 1;
                if(keys[Key.LeftControl]) {
                    MovementScale *= 0.2f;
                }
                if(keys[Key.LeftAlt]) {
                    MovementScale *= 5;
                }
                if (keys[Key.W]) {
                    Vector3 movement = new Vector3(0, 0, 0.5f);
                    movement.Multiply(MovementScale);
                    cameraPos += GetVector3(Vector3.Transform(movement, rotate));
                }
                if (keys[Key.S]) {
                    Vector3 movement = new Vector3(0, 0, 0.5f);
                    movement.Multiply(MovementScale);
                    cameraPos -= GetVector3(Vector3.Transform(movement, rotate));
                }
                if (keys[Key.A]) {
                    Vector3 movement = new Vector3(0.5f, 0, 0);
                    movement.Multiply(MovementScale);
                    cameraPos += GetVector3(Vector3.Transform(movement, rotate));
                }
                if (keys[Key.D]) {
                    Vector3 movement = new Vector3(0.5f, 0, 0);
                    movement.Multiply(MovementScale);
                    cameraPos -= GetVector3(Vector3.Transform(movement, rotate));
                }

                MovementScale *= 0.5f;

                if (keys[Key.LeftShift])
                    cameraPos.Y -= MovementScale;
                if (keys[Key.Space])
                    cameraPos.Y += MovementScale;
                snap = keys[Key.LeftAlt];
                if ((selectedObject != null || selectedCollider != null) && canMove)
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
                    if (dMove.LengthSq() > 0.01)
                        UpdateSelectedData();
                }
            }
            if (keys[Key.Delete] && selectedObject != null)
            {
                Tree.Nodes[1].Nodes.RemoveAt(higharchy.IndexOf(selectedObject));
                higharchy.Remove(selectedObject);
                selectedObject = null;
                mForm.SetObject(null);
                UpdateSelectedData();
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
            spHierarchyPanel.Panel1Collapsed = !spHierarchyPanel.Panel1Collapsed;
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            openLevel();
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (currentFile != string.Empty)
            {
                saveLevel(currentFile);
            } else {
                SaveFileDialog file = new SaveFileDialog();
                file.InitialDirectory = Application.StartupPath;
                file.Filter = "XML Level Files (*.xml)|*.xml";
                file.FilterIndex = 1;
                file.RestoreDirectory = true;
                if(file.ShowDialog() == DialogResult.OK) {
                    saveLevel(file.FileName);
                }
            }
        }

        private void changeTexture_Click(object sender, EventArgs e)
        {
            if (selectedObject != null)
            {
                OpenFileDialog file = new OpenFileDialog();
                file.InitialDirectory = Application.StartupPath;
                file.Filter = "Texture files (*.png)|*.png";
                file.FilterIndex = 1;
                file.RestoreDirectory = true;
                if (file.ShowDialog() == DialogResult.OK)
                {
                    selectedObject.loadTexture(file.FileName);
                    textureFileBox.Text = selectedObject.TextureFile;
                }
            }
        }

        private void colorSelect_Click(object sender, EventArgs e)
        {
            if (selectedCollider != null)
            {
                ColorDialog newColor = new ColorDialog();
                newColor.Color = selectedCollider.ObjectColor;
                if (newColor.ShowDialog() == DialogResult.OK)
                {
                    selectedCollider.ObjectColor = newColor.Color;
                    UpdateSelectedData();
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
                currentFile = saveFile.FileName;
                saveLevel(saveFile.FileName);
            }
        }

        private void componentEditorToolStripMenuItem_Click(object sender, EventArgs e)
        {
            
        }

        private void optionsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OptionsForm o = new OptionsForm();
            o.tbSoundbankText.Text = mSoundbank;
            if(o.ShowDialog() == DialogResult.OK)
            {
                mSoundbank = o.tbSoundbankText.Text;
            }
        }

        private void levelSettingsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Forms.LevelSettingsForm settings = new Forms.LevelSettingsForm();
            settings.SetPosition(mStartPos);
            settings.SetRotation(mStartRot);
            if(settings.ShowDialog() == DialogResult.OK)
            {
                mStartPos = settings.GetPosition();
                mStartRot = settings.GetRotation();
            }
        }

        private void Trigger_CheckedChanged(object sender, EventArgs e)
        {
            if (selectedCollider != null && oldSelectedCollider == selectedCollider)
            {
                selectedCollider.IsSolid = !Trigger.Checked;
                selectedCollider.CanMove = MoveCheck.Checked;
                selectedCollider.Visible = visibleCheck.Checked;
            }
        }

        private void componetsCheck_ItemCheck(object sender, ItemCheckEventArgs e)
        {
            if (selectedObject != null)
            {
                // TODO: Make it not rely on the index of the list.
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
                    case 2:
                        if (selectedObject.Collider == null && e.NewValue == CheckState.Checked)
                        {
                            selectedObject.Collider = new ToolObjectColor("Assets\\Plane.obj", Color.Red, ref device);
                            selectedObject.ColliderType = "Plane";
                            selectedObject.Collider.IsWireFrame = true;
                            selectedObject.Collider.Name = "Collider";
                            Tree.Nodes[1].Nodes[selectedIndex].Nodes.Add("Collider");
                        }
                        if (selectedObject.Collider != null && e.NewValue == CheckState.Unchecked && selectedObject.ColliderType == "Plane")
                        {
                            selectedObject.Collider = null;
                            Tree.Nodes[1].Nodes[selectedIndex].Nodes.RemoveAt(0);
                        }
                        break;
                    case 3:
                        if (selectedObject.Collider == null && e.NewValue == CheckState.Checked)
                        {
                            selectedObject.Collider = new ToolObjectColor("Assets\\Cube.obj", Color.Red, ref device);
                            selectedObject.ColliderType = "Button";
                            selectedObject.Collider.IsWireFrame = true;
                            selectedObject.Collider.Name = "Collider";
                            Tree.Nodes[1].Nodes[selectedIndex].Nodes.Add("Collider");
                        }
                        if (selectedObject.Collider != null && e.NewValue == CheckState.Unchecked && selectedObject.ColliderType == "Button")
                        {
                            selectedObject.Collider = null;
                            Tree.Nodes[1].Nodes[selectedIndex].Nodes.RemoveAt(0);
                        }
                        break;
                    case 4:
                        // Particle Emitter
                        if(selectedObject == null)
                        {
                            break;
                        }
                        if(e.NewValue == CheckState.Checked)
                        {
                            // Create and add a new particle emitter component to the selected object.
                            new ParticleEmitterComponent(selectedObject.mComponentList);
                            e.NewValue = CheckState.Unchecked;
                            mForm.Refresh();
                        }
                        break;
                    default:
                        if (e.NewValue == CheckState.Checked)
                            selectedObject.Components.Add((string)componetsCheck.Items[e.Index]);
                        else if (e.NewValue == CheckState.Unchecked)
                            selectedObject.Components.Remove((string)componetsCheck.Items[e.Index]);
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
            if (Tree.SelectedNode != null && Tree.SelectedNode.Parent != null && Tree.SelectedNode.Parent.Text == "Hierarchy")
            {
                selectedIndex = Tree.SelectedNode.Index;
                selectedObject = higharchy[selectedIndex];
                mForm.SetObject(selectedObject.mComponentList);
                selectedCollider = null;
                UpdateSelectedData();
            }
            else if (Tree.SelectedNode != null && Tree.SelectedNode.Parent != null && Tree.SelectedNode.Parent.Text != "Hierarchy" && Tree.SelectedNode.Parent.Text != "Objects")
            {
                if (higharchy[Tree.SelectedNode.Parent.Index].Name == Tree.SelectedNode.Parent.Text)
                {
                    selectedIndex = Tree.SelectedNode.Parent.Index;
                    selectedObject = null;
                    mForm.SetObject(null);
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
            else if (Tree.SelectedNode != null && Tree.SelectedNode.Parent != null && Tree.SelectedNode.Parent.Text == "Hierarchy" && Tree.SelectedNode.Index < higharchy.Count)
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
            // Hide Yams' controls so we can start adding in the component model.
            //groupBox1.Visible = false;
            //groupBox2.Visible = false;
            //groupBox3.Visible = false;
            //groupBox4.Visible = false;
            //groupBox5.Visible = false;
            //groupBox6.Visible = false;
            //groupBox7.Visible = false;
            //TextureBox.Visible = false;
            //return;
            if (selectedCollider != null)
            {
                groupBox5.Visible = false;
                TextureBox.Visible = false;
                colorSelect.Visible = true;
                Trigger.Visible = true;
                MoveCheck.Visible = true;
                visibleCheck.Visible = true;
                Physics.Visible = true;
                ExtraVector.Visible = true;
                if (colliderType == "Plane")
                    ExtraVector.Text = "Normal";
                else if (colliderType == "Button")
                    ExtraVector.Text = "Push Normal";
                else
                    ExtraVector.Text = "Gravity";
                Trigger.Checked = !selectedCollider.IsSolid;
                nameBox.Text = selectedCollider.Name;
                posX.Value =        (decimal)selectedCollider.Position.X;
                posY.Value =        (decimal)selectedCollider.Position.Y;
                posZ.Value =        (decimal)selectedCollider.Position.Z;

                rotX.Value =        (decimal)(selectedCollider.Rotation.X * ToolObject.RADIANS_TO_DEGREES);
                rotY.Value =        (decimal)(selectedCollider.Rotation.Y * ToolObject.RADIANS_TO_DEGREES);
                rotZ.Value =        (decimal)(selectedCollider.Rotation.Z * ToolObject.RADIANS_TO_DEGREES);

                ExtraX.Value =      (decimal)selectedCollider.Gravity.X;
                ExtraY.Value =      (decimal)selectedCollider.Gravity.Y;
                ExtraZ.Value =      (decimal)selectedCollider.Gravity.Z;

                MoveCheck.Checked = selectedCollider.CanMove;
                visibleCheck.Checked = selectedCollider.Visible;
                colorSelect.BackColor = selectedCollider.ObjectColor;

                if (colliderType == "Button")
                {
                    Mass.Visible = MassL.Visible = true;
                    StaticL.Text = "Normal Force";
                    StaticL.Visible = StaticF.Visible = true;
                    ElasticityL.Visible = Elasticity.Visible = false;
                    KeneticL.Visible = KeneticF.Visible = false;
                    DragL.Visible = Drag.Visible = false;
                }
                else
                {
                    StaticL.Text = "Static Force";
                    Mass.Visible = MassL.Visible = true;
                    StaticL.Visible = StaticF.Visible = true;
                    ElasticityL.Visible = Elasticity.Visible = true;
                    KeneticL.Visible = KeneticF.Visible = true;
                    DragL.Visible = Drag.Visible = true;
                }
                Mass.Value =        (decimal)selectedCollider.Mass;
                Elasticity.Value =  (decimal)selectedCollider.Elasticity;
                StaticF.Value =     (decimal)selectedCollider.StaticF;
                KeneticF.Value =    (decimal)selectedCollider.KeneticF;
                Drag.Value =        (decimal)selectedCollider.Drag;

                if (colliderType == "OBB")
                {
                    groupBox1.Visible = true;
                    groupBox6.Visible = false;
                    scaleX.Value = (decimal)selectedCollider.Scale.X;
                    scaleY.Value = (decimal)selectedCollider.Scale.Y;
                    scaleZ.Value = (decimal)selectedCollider.Scale.Z;
                }
                else if (colliderType == "Sphere")
                {
                    groupBox1.Visible = false;
                    groupBox6.Visible = true;
                    Radius.Value = (decimal)selectedCollider.Scale.X;
                }
            }
            else if (selectedObject != null)
            {
                TextureBox.Visible = true;
                groupBox6.Visible = false;
                groupBox5.Visible = true;
                groupBox1.Visible = true;
                Trigger.Visible = false;
                colorSelect.Visible = false;
                Physics.Visible = false;
                visibleCheck.Visible = false;
                MoveCheck.Visible = false;
                nameBox.Text = selectedObject.Name;
                posX.Value =    (decimal)selectedObject.Position.X;
                posY.Value =    (decimal)selectedObject.Position.Y;
                posZ.Value =    (decimal)selectedObject.Position.Z;

                rotX.Value =    (decimal)(selectedObject.Rotation.X * ToolObject.RADIANS_TO_DEGREES);
                rotY.Value =    (decimal)(selectedObject.Rotation.Y * ToolObject.RADIANS_TO_DEGREES);
                rotZ.Value =    (decimal)(selectedObject.Rotation.Z * ToolObject.RADIANS_TO_DEGREES);

                scaleX.Value =  (decimal)selectedObject.Scale.X;
                scaleY.Value =  (decimal)selectedObject.Scale.Y;
                scaleZ.Value =  (decimal)selectedObject.Scale.Z;
                textureFileBox.Text = selectedObject.Texture != null ? selectedObject.TextureFile : "None";
                for (int i = 4; i < 14; i++)
                    if (selectedObject.Components.Contains((string)componetsCheck.Items[i]))
                        componetsCheck.SetItemChecked(i, true);
                    else
                        componetsCheck.SetItemChecked(i, false);
                if (selectedObject.Collider != null)
                {
                    componetsCheck.SetItemChecked(0, selectedObject.ColliderType == "OBB" ?    true : false);
                    componetsCheck.SetItemChecked(1, selectedObject.ColliderType == "Sphere" ? true : false);
                    componetsCheck.SetItemChecked(2, selectedObject.ColliderType == "Plane" ?  true : false);
                    componetsCheck.SetItemChecked(3, selectedObject.ColliderType == "Button" ? true : false);
                }
                else
                {
                    for (int i = 0; i < 14; i++)
                        componetsCheck.SetItemChecked(i, false);
                }
            }
            else
            {
                TextureBox.Visible = true;
                groupBox6.Visible = false;
                groupBox5.Visible = true;
                groupBox1.Visible = true;
                colorSelect.Visible = false;
                Trigger.Visible = false;
                visibleCheck.Visible = false;
                Physics.Visible = false;
                MoveCheck.Visible = false;
                componetsCheck.ClearSelected();
                for (int i = 0; i < 14; i++)
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
                textureFileBox.Text = "None";
            }
            if (oldSelected != selectedObject)
                oldSelected = selectedObject;
            if (oldSelectedCollider != selectedCollider)
                oldSelectedCollider = selectedCollider;
            
            FileChanged = true;
        }

        private void nameBox_KeyDown(object sender, KeyEventArgs e)
        {
            if (!canMove)
            {
                if (e.KeyCode == Keys.Enter)
                    e.Handled = e.SuppressKeyPress = true;
            }
            else
                e.Handled = e.SuppressKeyPress = true;
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
            else if (selectedCollider != null && oldSelectedCollider == selectedCollider)
            {
                Tree.Nodes[1].Nodes[selectedIndex].Nodes[0].Text = nameBox.Text;
                selectedCollider.Name = nameBox.Text;
                selectedCollider.Gravity = new Vector3((float)ExtraX.Value, (float)ExtraY.Value, (float)ExtraZ.Value);
                selectedCollider.Mass = (float)Mass.Value;
                selectedCollider.Elasticity = (float)Elasticity.Value;
                selectedCollider.StaticF = (float)StaticF.Value;
                selectedCollider.KeneticF = (float)KeneticF.Value;
                selectedCollider.Drag = (float)Drag.Value;
                selectedCollider.SetPosition(new Vector3((float)posX.Value, (float)posY.Value, (float)posZ.Value));
                if (colliderType == "Sphere")
                    selectedCollider.SetScale(new Vector3((float)Radius.Value, (float)Radius.Value, (float)Radius.Value));
                else
                    selectedCollider.SetScale(new Vector3((float)scaleX.Value, (float)scaleY.Value, (float)scaleZ.Value));
                selectedCollider.SetRotate(new Vector3((float)rotX.Value * ToolObject.DEGREES_TO_RADIANS, (float)rotY.Value * ToolObject.DEGREES_TO_RADIANS, (float)rotZ.Value * ToolObject.DEGREES_TO_RADIANS));
            }
        }


        private void supress_KeyDown(object sender, KeyPressEventArgs e)
        {
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
            graphicsPanel1.Invalidate();
            fpsTimer.Stop();
            advMillisecond.Add(fpsTimer.ElapsedMilliseconds);
            if (advMillisecond.Count >= 5) {
                long adv = 0;
                foreach (long l in advMillisecond)
                    adv += l;
                adv /= advMillisecond.Count;
                //FpsCount.Text = "FPS: " + (1000 / adv);
                //Debug.Print("FPS: " + (1000 / adv));
                advMillisecond.Clear();
            }
            fpsTimer.Reset();
            fpsTimer.Start();
        }


        public void SetFilepath(string _filepath) {
            mCurrentFilename = _filepath;
            if(_filepath.Length > 45) {
                _filepath = "..." + _filepath.Substring(_filepath.Length - 45);
            }
            _filepath = "Level Editor - " + _filepath;
            Text = _filepath;
        }

        public void SetFileEdited(bool _edited) {
            if(_edited && !mCurrentFileChanged) {
                SetFilepath(mCurrentFilename);
                Text += "*";
            } else if(!_edited && mCurrentFileChanged) {
                SetFilepath(mCurrentFilename);
            }
            mCurrentFileChanged = _edited;
        }

    } // Class
} // Namespace
