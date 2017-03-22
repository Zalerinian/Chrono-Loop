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
		private float mRotationSpeed;
		private Microsoft.DirectX.DirectInput.Device mKeyboard;
		private List<ToolObject> objects = new List<ToolObject>();
		private List<ToolObject> higharchy = new List<ToolObject>();
		private List<ToolObjectColor> debugObjs = new List<ToolObjectColor>();
		private Stopwatch mFPSTimer = new Stopwatch();
		private List<long> advMillisecond = new List<long>();
		private Vector3 cameraPos = new Vector3(0, 0, 0), prevHit = new Vector3(0, 0, 0), curHit = new Vector3(0, 0, 0);
		private Vector2 prevMouse, curMouse;
		private Vector3 mStartPos, mStartRot;
		private string selectedName = string.Empty, colliderType = string.Empty, currentFile = string.Empty;
		Matrix gizmoScale = Matrix.Identity;
		Matrix rotate = Matrix.Identity;

		// Variables added by Drew
		private string mCurrentFilename = string.Empty;
		private bool mCurrentFileChanged = false;
		private MouseEventArgs mMouseState;

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
			InitializeKeyboard();
			Renderer.Instance.AttachToControl(graphicsPanel1);

			mGrid = new ColoredShape();
			mGrid.MakeGrid();
			mGrid.FillMode = FillMode.WireFrame;
			mGrid.Color = Color.White;
			Renderer.Instance.AddShape(mGrid);

			mFPSTimer.Start();
			mRotationSpeed = 0.005f;
			mMouseState = new MouseEventArgs(MouseButtons.None, 0, 0, 0, 0);

			// Level settings McBootleg
			mStartPos = new Vector3(0, 0, 0);
			mStartRot = new Vector3(0, 0, 0);

			spWorldView.Panel2.ControlAdded += ReorderComponents;
			spWorldView.Panel2.ControlRemoved += ReorderComponents;
			spWorldView.Panel2.AutoScroll = true;

			btnFocus.Size = new Size(0, 0);
			btnFocus.Select();

			spWorldView.Panel2.Controls.Add(btnComponentAdd);
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
			//device.RenderState.ZBufferEnable = true;
			//device.Clear(ClearFlags.ZBuffer, 0, 1.0f, 0);
			//device.Clear(ClearFlags.Target, Color.Black, 1.0f, 0);
			//
			//device.BeginScene();
			////Grid
			//device.VertexFormat = CustomVertex.PositionNormalColored.Format;
			//device.RenderState.CullMode = Cull.None;
			//device.SetTexture(0, null);
			//device.RenderState.AlphaBlendEnable = false;
			//device.RenderState.FillMode = debugObjs[0].IsWireFrame ? FillMode.WireFrame : FillMode.Solid;
			//device.SetStreamSource(0, debugObjs[0].VertexBuffer, 0);
			//device.Indices = debugObjs[0].IndexBuffer;
			//device.Transform.World = debugObjs[0].Transform;
			//device.DrawIndexedPrimitives(PrimitiveType.TriangleList, 0, 0, debugObjs[0].Indices.Length, 0, debugObjs[0].Indices.Length / 3);
			//if (loaded)
			//{
			//	//Scene
			//	device.RenderState.AlphaBlendEnable = false;
			//	foreach (ToolObject tObj in higharchy)
			//	{
			//		if (tObj.Vertices != null)
			//		{
			//			device.VertexFormat = CustomVertex.PositionNormalTextured.Format;
			//			device.RenderState.CullMode = Cull.Clockwise;
			//			device.RenderState.FillMode = tObj.IsWireFrame ? FillMode.WireFrame : FillMode.Solid;
			//			device.SetStreamSource(0, tObj.VertexBuffer, 0);
			//			device.Indices = tObj.IndexBuffer;
			//			if (tObj.Texture == null)
			//				device.SetTexture(0, null);
			//			else
			//				device.SetTexture(0, tObj.Texture);
			//			device.Transform.World = tObj.Transform;
			//			device.DrawIndexedPrimitives(PrimitiveType.TriangleList, 0, 0, tObj.Indices.Length, 0, tObj.Indices.Length / 3);
			//		}
			//		if (tObj.Collider != null && tObj.Collider.Visible == true)
			//		{
			//			device.VertexFormat = CustomVertex.PositionNormalColored.Format;
			//			device.RenderState.CullMode = Cull.None;
			//			device.RenderState.FillMode = tObj.Collider.IsWireFrame ? FillMode.WireFrame : FillMode.Solid;
			//			device.SetStreamSource(0, tObj.Collider.VertexBuffer, 0);
			//			device.Indices = tObj.Collider.IndexBuffer;
			//			device.SetTexture(0, null);
			//			if (tObj.ColliderType == "Sphere")
			//				device.Transform.World = Matrix.Translation(tObj.Position) * tObj.Collider.Transform;
			//			else
			//				device.Transform.World = tObj.Collider.Transform * tObj.Transform;
			//			device.DrawIndexedPrimitives(PrimitiveType.TriangleList, 0, 0, tObj.Collider.Indices.Length, 0, tObj.Collider.Indices.Length / 3);
			//		}
			//	}
			//}
			////Axis Gizmo
			//device.Clear(ClearFlags.ZBuffer, 0, 1.0f, 0);
			//if (selectedObject != null)
			//{
			//	device.VertexFormat = CustomVertex.PositionNormalColored.Format;
			//	device.RenderState.CullMode = Cull.None;
			//	device.SetTexture(0, null);
			//	device.RenderState.AlphaBlendEnable = true;
			//	float scale = (cameraPos - selectedObject.Position).Length();
			//	gizmoScale = Matrix.Scaling(Vector3.Maximize(new Vector3(scale, scale, scale) * 0.05f, new Vector3(0.05f, 0.05f, 0.05f)));
			//	foreach (ToolObjectColor tObj2 in debugObjs[1].Children)
			//	{
			//		device.RenderState.FillMode = FillMode.Solid;
			//		device.SetStreamSource(0, tObj2.VertexBuffer, 0);
			//		device.Indices = tObj2.IndexBuffer;
			//		device.Transform.World = gizmoScale * Matrix.Translation(selectedObject.Position) * debugObjs[1].Transform * tObj2.Transform;
			//		device.DrawIndexedPrimitives(PrimitiveType.TriangleList, 0, 0, tObj2.Indices.Length, 0, tObj2.Indices.Length / 3);
			//	}
			//}
			//device.EndScene();
			//
			//device.Present();
			//device.RenderState.ZBufferEnable = false;
			//device.RenderState.UseWBuffer = true;
			//UpdateKeyboardInput();
		}

		private void graphicsPanel1_MouseLeave(object sender, EventArgs e)
		{
		}

		private void graphicsPanel1_MouseClick(object sender, MouseEventArgs e)
		{
			// Give the Focus textbox focus so that we can gladly accept input for the graphics panel.
			btnFocus.Select();
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

		private void graphicsPanel1_MouseMove(object sender, MouseEventArgs e)
		{
			// Update Mouse Input
			prevMouse = curMouse;
			curMouse = new Vector2(e.X, e.Y);

			if(ActiveForm != this)
			{
				// Ensure we're the active window.
				return;
			}
			Vector2 delta = curMouse - prevMouse;
			switch(e.Button)
			{
				case MouseButtons.Middle:
					Renderer.Instance.AngleY -= delta.X * mRotationSpeed;
					Renderer.Instance.AngleX += delta.Y * mRotationSpeed;
					Renderer.Instance.RebuildViewMatrix();
					break;
				case MouseButtons.Right:
					// Minus-equals because Right handed systems have positive X going out of the screen.
					Renderer.Instance.CameraPosition -= Renderer.Instance.Forward * 0.1f * delta.X;

					// TODO: When the cursor approaches the corners of the current monitor, wrap its position.
					break;
			}

			mMouseState = e;
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

			if (btnFocus.ContainsFocus)
			{
				// Lateral movement, only if no Control key is held down.
				if (!keys[Key.LeftControl] && !keys[Key.RightControl])
				{
					if (keys[Key.W])
					{
						Renderer.Instance.CameraPosition -= Renderer.Instance.Forward * 0.1f;
					}
					if (keys[Key.S])
					{
						Renderer.Instance.CameraPosition += Renderer.Instance.Forward * 0.1f;
					}
					if (keys[Key.A])
					{
						Renderer.Instance.CameraPosition -= Renderer.Instance.Right * 0.1f;
					}
					if (keys[Key.D])
					{
						Renderer.Instance.CameraPosition += Renderer.Instance.Right * 0.1f;
					}
					if (keys[Key.Space])
					{
						Renderer.Instance.CameraPosition += Renderer.Instance.Up * 0.1f;
					}
					if (keys[Key.LeftShift] || keys[Key.RightShift])
					{
						Renderer.Instance.CameraPosition -= Renderer.Instance.Up * 0.1f;
					}
				} // END Not holding control keys
			} // Ensure the Grapics Panel's focus textbox is selected.
		}

		private void RightToggle_Click(object sender, EventArgs e)
		{
			RightToggle.Text = spWorldView.Panel2Collapsed ? ">" : "<";
			spWorldView.Panel2Collapsed = !spWorldView.Panel2Collapsed;
		}

		private void LeftToggle_Click(object sender, EventArgs e)
		{
			LeftToggle.Text = spHierarchyPanel.Panel1Collapsed ? "<" : ">";
			spHierarchyPanel.Panel1Collapsed = !spHierarchyPanel.Panel1Collapsed;
		}

		private void AddComponentHandler(object sender, EventArgs e)
		{
			if(!(sender is ToolStripMenuItem))
			{
				Debug.Print("Trying to add a component, but the sender is not valid.");
				return;
			}
			if(Tree.SelectedNode == null)
			{
				Debug.Print("Trying to add a component, but the tree has no selected node!");
				return;
			}
			if(!(Tree.SelectedNode.Tag is BaseObject))
			{
				Debug.Print("Trying to add a component, but the selected noe doesn't have an associated BaseObject!");
				return;
			}
			BaseObject obj = Tree.SelectedNode.Tag as BaseObject;
			string tag = (string)((ToolStripMenuItem)sender).Tag;
			switch(tag)
			{
				// Colliders
				case "ColBox":
					obj.AddComponent(new BoxCollider());
					break;
				case "ColButton":
					obj.AddComponent(new ButtonCollider());
					break;
				case "ColPlane":
					obj.AddComponent(new PlaneCollider());
					break;
				case "ColSphere":
					obj.AddComponent(new SphereCollider());
					break;
				case "ColController":
					//obj.AddComponent(new ControllerCollider());
					break;

				// Particle Emitters
				case "PEVolume":
					break;
				case "PERadial":
					Debug.Print("Is this not just a spherical volume emitter...?");
					break;
				case "PETeleport":
					Debug.Print("Honestly this should be configurable through a generic particle or volume emitter.");
					break;

				// Non-Grouped components that *aren't* code components
				case "Mesh":
					break;
				case "Audio":
					break;

				// Code Components


				default:
					break;
			}
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
			btnFocus.Select();
		}

		private void mCreateMenuAdd_Click(object sender, EventArgs e)
		{
			Tree.SelectedNode = ConstructTreeObject(null);
			btnFocus.Select();
		}

		private void Tree_AfterSelect(object sender, TreeViewEventArgs e)
		{
			spWorldView.Panel2.Controls.Clear();
			// Needed to keep the component button on the side panel.
			spWorldView.Panel2.Controls.Add(btnComponentAdd);
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


		private void mCreateMenuAddChild_Click(object sender, EventArgs e)
		{
			TreeNode n = ConstructTreeObject(Tree.SelectedNode);
			if (Tree.SelectedNode != null)
			{
				// TODO: Add a setting in the editor for Expanding nodes on adding a child instead of assuming.
				Tree.SelectedNode.Expand();
			}
			Tree.SelectedNode = n;
			btnFocus.Select();
		}

		private TreeNode ConstructTreeObject(TreeNode _parent)
		{
			TreeNode n = new TreeNode();
			n.ContextMenuStrip = mObjectStrip;
			BaseObject b = new BaseObject(n, "Empty Object");
			b.ComponentAdded += ObjectAddComponent;
			b.ComponentRemoved += ObjectRemoveComponent;
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

		private void ObjectAddComponent(Component _c)
		{
			spWorldView.Panel2.Controls.Add(_c.GetGroupbox());
			ReorderComponents(spWorldView.Panel2, EventArgs.Empty);
		}

		private void ObjectRemoveComponent(Component _c)
		{
			spWorldView.Panel2.Controls.Remove(_c.GetGroupbox());
			ReorderComponents(spWorldView.Panel2, EventArgs.Empty);
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
			dimensions.Width = ControlWidth;
			for (int i = 0; i < obj.GetComponents().Count; ++i)
			{
				GroupBox box = obj.GetComponents()[i].GetGroupbox();
				box.Location = position;
				position.Y += box.Size.Height + 3;

				dimensions.Height = box.Height;
				box.Size = dimensions;
			}

			position.Y += 15;
			position.X = btnComponentAdd.Location.X;
			dimensions.Height = btnComponentAdd.Size.Height;
			dimensions.Width /= 2;
			btnComponentAdd.Location = position;
			btnComponentAdd.Size = dimensions;
		}

		private void graphicsPanel1_MouseUp(object sender, MouseEventArgs e)
		{
		}

		private void timer1_Tick(object sender, EventArgs e)
		{
			graphicsPanel1.Invalidate();
			mFPSTimer.Stop();
			advMillisecond.Add(mFPSTimer.ElapsedMilliseconds);
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
			mFPSTimer.Reset();
			mFPSTimer.Start();
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
