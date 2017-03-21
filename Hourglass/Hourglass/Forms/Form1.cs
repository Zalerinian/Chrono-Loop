using System;
using System.Drawing;
using System.Windows.Forms;
using System.Collections.Generic;
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;
using Microsoft.DirectX.DirectInput;
using System.Diagnostics;


namespace Hourglass
{
	public partial class Editor : Form
	{

		public const float RADIANS_TO_DEGREES = ((180.0f / 3.14f));
		public const float DEGREES_TO_RADIANS = (1 / 180.0f * 3.14f);
		private Microsoft.DirectX.Direct3D.Device device;
		private float angleX, angleY, rotSpeed;
		private Microsoft.DirectX.DirectInput.Device mKeyboard;
		private List<ToolObject> objects = new List<ToolObject>();
		private List<ToolObject> higharchy = new List<ToolObject>();
		private List<ToolObjectColor> debugObjs = new List<ToolObjectColor>();
		private ToolObject selectedObject = null;
		private Stopwatch fpsTimer = new Stopwatch();
		private List<long> advMillisecond = new List<long>();
		private Vector3 cameraPos = new Vector3(0, 0, 0), prevHit = new Vector3(0, 0, 0), curHit = new Vector3(0, 0, 0);
		private Vector2 prevMouse, curMouse;
		private Vector3 mStartPos, mStartRot;
		private bool loaded = true;
		private string selectedName = string.Empty, colliderType = string.Empty, currentFile = string.Empty;
		Matrix gizmoScale = Matrix.Identity;
		Matrix rotate = Matrix.Identity;

		// Variables added by Drew
		private string mCurrentFilename = string.Empty;
		private bool mCurrentFileChanged = false;
		private bool mFormResizing = false, mDeviceNeedsReset = false;

		private ColoredShape mGrid;

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
			//InitializeDevice();
			Renderer.Instance.AttachToControl(graphicsPanel1);
			InitializeKeyboard();
			//InitializeCamera();

			mGrid = new ColoredShape();
			mGrid.MakeGrid();
			mGrid.Color = Color.White;

			TexturedShape cube = new TexturedShape("Assets\\Cube.obj");
			cube.SetTexture(0, TextureLoader.FromFile(Renderer.Instance.Device, "Assets\\missing.dds"));
			Renderer.Instance.AddShape(mGrid);
			Renderer.Instance.AddShape(cube);

			//debugObjs.Add(new ToolObjectColor(ref device));
			//debugObjs[0].MakeGrid();
			//debugObjs[0].ObjectColor = Color.GhostWhite;
			//debugObjs[0].IsWireFrame = true;
			//debugObjs.Add(new ToolObjectColor("Assets\\AxisGizmo.obj", ref device));
			spHierarchyPanel.BorderStyle = BorderStyle.None;
			spHierarchyPanel.SplitterWidth = 8;
			spWorldView.BorderStyle = BorderStyle.None;
			spWorldView.SplitterWidth = 8;
			fpsTimer.Start();
			rotSpeed = 0.005f;
			angleX = angleY = 0;
			rotate = Matrix.Identity;


			// Level settings McBootleg
			mStartPos = new Vector3(0, 0, 0);
			mStartRot = new Vector3(0, 0, 0);

			spWorldView.Panel2.ControlAdded += ReorderComponents;
			spWorldView.Panel2.ControlRemoved += ReorderComponents;
			spWorldView.Panel2.AutoScroll = true;
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
				device = new Microsoft.DirectX.Direct3D.Device(0, Microsoft.DirectX.Direct3D.DeviceType.Hardware, this.graphicsPanel1, CreateFlags.HardwareVertexProcessing, presentParams);
				device.RenderState.FillMode = FillMode.Solid;
				device.RenderState.CullMode = Cull.Clockwise;
				device.DeviceReset += HandleResetEvent;
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
			if (mFormResizing)
			{
				Debug.Print("Device lost context, but the form is resizing!");
				mDeviceNeedsReset = true;
				return;
			}
			else
			{
				Debug.Print("Device reset, rebuilding it!");
				mDeviceNeedsReset = false;
				device.RenderState.FillMode = FillMode.Solid;
				device.RenderState.CullMode = Cull.Clockwise;
				device.RenderState.ZBufferEnable = true;
				InitializeCamera();
				foreach (ToolObjectColor to in debugObjs)
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
		}

		private void InitializeCamera()
		{
			device.Transform.Projection = Matrix.PerspectiveFovRH((float)Math.PI / 4.0f, (float)graphicsPanel1.Width / (float)graphicsPanel1.Height, 0.1f, 1000);
			Matrix rotate = Matrix.RotationYawPitchRoll(angleY, angleX, 0);
			Vector3 look = cameraPos + GetVector3(Vector3.Transform(new Vector3(0, 0, -1), rotate));
			device.Transform.View = Matrix.LookAtRH(cameraPos, look, new Vector3(0, 1, 0));
			device.RenderState.Lighting = false;
			device.RenderState.ZBufferEnable = true;
			device.RenderState.CullMode = Cull.Clockwise;
		}

		public void InitializeKeyboard()
		{
			mKeyboard = new Microsoft.DirectX.DirectInput.Device(SystemGuid.Keyboard);
			mKeyboard.SetCooperativeLevel(this, CooperativeLevelFlags.Background | CooperativeLevelFlags.NonExclusive);
			mKeyboard.Acquire();
		}

		private void OnPaint(object sender, PaintEventArgs e)
		{
			UpdateKeyboardInput();

			Renderer.Instance.Render();
			return;
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
							device.SetTexture(0, null);
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
					device.RenderState.FillMode = FillMode.Solid;
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
			UpdateKeyboardInput();
		}

		private void graphicsPanel1_MouseLeave(object sender, EventArgs e)
		{
		}

		private void graphicsPanel1_MouseClick(object sender, MouseEventArgs e)
		{
			if (e.Button == MouseButtons.Left)
			{
				// TODO: Raycast to hit an object. Be sure to apply the camera's rotation in there somehow.
				// I.E. make an identity matrix, put the mouse X/Y in the position spots, and multiply by the inverse view matrix

				//Vector3 near, far, hit;
				//hit = new Vector3(1000.0f, 1000.0f, 1000.0f);
				//int selection = -1;
				//
				//if (selection == -1)
				//{
				//	hit = new Vector3(1000.0f, 1000.0f, 1000.0f);
				//	for (int i = 0; i < higharchy.Count; i++)
				//	{
				//		Vector3 testHit = new Vector3();
				//		near = new Vector3(curMouse.X, curMouse.Y, 0);
				//		far = new Vector3(curMouse.X, curMouse.Y, 1);
				//		near.Unproject(device.Viewport, device.Transform.Projection, device.Transform.View, higharchy[i].Transform);
				//		far.Unproject(device.Viewport, device.Transform.Projection, device.Transform.View, higharchy[i].Transform);
				//		if (higharchy[i].RayHit(out testHit, near, far))
				//			if ((testHit - cameraPos).LengthSq() < (hit - cameraPos).LengthSq())
				//			{
				//				hit = testHit;
				//				selection = i;
				//			}
				//	}
				//}
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
			// Update Mouse Input
			prevMouse = curMouse;
			curMouse = new Vector2(e.X, e.Y);

			switch(e.Button)
			{
				case MouseButtons.Middle:
					Vector2 delta = curMouse - prevMouse;
					Renderer.Instance.AngleY -= delta.X * rotSpeed;
					Renderer.Instance.AngleX += delta.Y * rotSpeed;
					Renderer.Instance.RebuildViewMatrix();
					break;
			}
		}

		private Vector3 GetVector3(Vector4 v)
		{
			return new Vector3(v.X, v.Y, v.Z);
		}

		private void UpdateKeyboardInput()
		{
			// Ensure this window is selected.
			if (ActiveForm != this)
			{
				return;
			}
			// Todo add movement controls.
			KeyboardState keys = mKeyboard.GetCurrentKeyboardState();

			// Lateral movement
			if(keys[Key.W])
			{
				Renderer.Instance.View *= Matrix.Translation(0, 0, 0.1f);
			}
			if(keys[Key.S])
			{
				Renderer.Instance.View *= Matrix.Translation(0, 0, -0.1f);
			}
			if(keys[Key.A])
			{
				Renderer.Instance.View *= Matrix.Translation(0.1f, 0, 0);
			}
			if(keys[Key.D])
			{
				Renderer.Instance.View *= Matrix.Translation(-0.1f, 0, 0);
			}
			if(keys[Key.Space])
			{
				Renderer.Instance.View *= Matrix.Translation(0, -0.1f, 0);
			}
			if(keys[Key.LeftShift] || keys[Key.RightShift])
			{
				Renderer.Instance.View *= Matrix.Translation(0, 0.1f, 0);
			}
		}

		private void OnResize(object sender, EventArgs e)
		{
			graphicsPanel1.Invalidate();
		}

		private void RightToggle_Click(object sender, EventArgs e)
		{
			RightToggle.Text = spWorldView.Panel2Collapsed ? ">" : "<";
			spWorldView.Panel2Collapsed = !spWorldView.Panel2Collapsed;
		}

		private void LeftToggle_Click(object sender, EventArgs e)
		{
			LeftToggle.Text = spHierarchyPanel.Panel1Collapsed ? ">" : "<";
			spHierarchyPanel.Panel1Collapsed = !spHierarchyPanel.Panel1Collapsed;
		}

		private void AddColliderComponent(object sender, EventArgs e)
		{
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
			}
			else
			{
				SaveFileDialog file = new SaveFileDialog();
				file.InitialDirectory = Application.StartupPath;
				file.Filter = "XML Level Files (*.xml)|*.xml";
				file.FilterIndex = 1;
				file.RestoreDirectory = true;
				if (file.ShowDialog() == DialogResult.OK)
				{
					saveLevel(file.FileName);
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

		private void mMenuButton_Click(object sender, EventArgs e)
		{
			Tree.SelectedNode = ConstructTreeObject(null);
		}

		private void mCreateMenuAdd_Click(object sender, EventArgs e)
		{
			Tree.SelectedNode = ConstructTreeObject(null);
		}

		private void Tree_AfterSelect(object sender, TreeViewEventArgs e)
		{
			spWorldView.Panel2.Controls.Clear();
			BaseObject obj = (BaseObject)Tree.SelectedNode.Tag;
			if (obj == null)
			{
				Debug.Print("AfterSelect object is null!");
			}
			for (int i = 0; i < obj.GetComponents().Count; ++i)
			{
				spWorldView.Panel2.Controls.Add(obj.GetComponents()[i].GetGroupbox());
			}
		}

		private void Editor_ResizeBegin(object sender, EventArgs e)
		{
			mFormResizing = true;
		}

		private void Editor_ResizeEnd(object sender, EventArgs e)
		{
			mFormResizing = false;
			if(mDeviceNeedsReset)
			{
				HandleResetEvent(null, null);
			}
		}

		private void mCreateMenuAddChild_Click(object sender, EventArgs e)
		{
			TreeNode n = ConstructTreeObject(Tree.SelectedNode);
			if (Tree.SelectedNode != null)
			{
				// TODO: Add a setting in the editor for Expanding nodes on adding a child instead of assuming.
				Tree.SelectedNode.Expand();
			}
			Tree.SelectedNode = n;
		}

		private TreeNode ConstructTreeObject(TreeNode _parent)
		{
			TreeNode n = new TreeNode();
			n.ContextMenuStrip = mObjectStrip;
			BaseObject b = new BaseObject(n, "Empty Object");
			n.Tag = b;
			if (_parent != null)
			{
				_parent.Nodes.Add(n);
			}
			else
			{
				Tree.Nodes.Add(n);
			}
			return n;
		}

		private void levelSettingsToolStripMenuItem_Click(object sender, EventArgs e)
		{
			Forms.LevelSettingsForm settings = new Forms.LevelSettingsForm();
			settings.SetPosition(mStartPos);
			settings.SetRotation(mStartRot);
			if (settings.ShowDialog() == DialogResult.OK)
			{
				mStartPos = settings.GetPosition();
				mStartRot = settings.GetRotation();
			}
		}

		private void ReorderComponents(object sender, EventArgs e)
		{
			Point position = new Point(AutoScrollPosition.X, AutoScrollPosition.Y);
			int ControlWidth = ((Control)sender).ClientSize.Width;
			HorizontalScroll.Value = 0;
			HorizontalScroll.Maximum = ControlWidth;
			BaseObject obj = (BaseObject)Tree.SelectedNode.Tag;
			if (obj == null)
			{
				Debug.Print("Cannot reorder components: No selected object!");
				return;
			}
			Size dimensions = new Size();
			for (int i = 0; i < obj.GetComponents().Count; ++i)
			{
				GroupBox box = obj.GetComponents()[i].GetGroupbox();
				box.Location = position;
				position.Y += box.Size.Height + 3;

				dimensions.Width = ControlWidth;
				dimensions.Height = box.Height;
				box.Size = dimensions;
			}
		}

		private void graphicsPanel1_MouseUp(object sender, MouseEventArgs e)
		{
		}

		private void timer1_Tick(object sender, EventArgs e)
		{
			graphicsPanel1.Invalidate();
			fpsTimer.Stop();
			advMillisecond.Add(fpsTimer.ElapsedMilliseconds);
			if (advMillisecond.Count >= 5)
			{
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

		public void SetFilepath(string _filepath)
		{
			mCurrentFilename = _filepath;
			if (_filepath.Length > 45)
			{
				_filepath = "..." + _filepath.Substring(_filepath.Length - 45);
			}
			_filepath = "Level Editor - " + _filepath;
			Text = _filepath;
		}

		public void SetFileEdited(bool _edited)
		{
			if (_edited && !mCurrentFileChanged)
			{
				SetFilepath(mCurrentFilename);
				Text += "*";
			}
			else if (!_edited && mCurrentFileChanged)
			{
				SetFilepath(mCurrentFilename);
			}
			mCurrentFileChanged = _edited;
		}

	} // Class
} // Namespace
