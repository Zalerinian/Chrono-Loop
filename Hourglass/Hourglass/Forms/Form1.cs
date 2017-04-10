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
		private float mRotationSpeed = 0.005f, mGizmoSpeed = 1.0f;
		private Microsoft.DirectX.DirectInput.Device mKeyboard;
		private Stopwatch mFPSTimer = new Stopwatch();
		private List<long> advMillisecond = new List<long>();
		private Vector3 cameraPos = new Vector3(0, 0, 0);
		private Vector2 prevMouse, curMouse;
		private string currentFile = string.Empty;

		// Variables added by Drew
		private string mCurrentFilename = string.Empty;
		private bool mCurrentFileChanged = false;
		private MouseEventArgs mMouseState = null;
		private List<Key> mKeys = null, mPreviousKeys = null;
		private BaseObject mGrid = null;

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

			ColoredMeshComponent Grid = new ColoredMeshComponent();
			((ColoredShape)Grid.Shape).MakeGrid();
			Grid.Shape.FillMode = FillMode.WireFrame;
			((ColoredShape)Grid.Shape).Color = Color.White;
			mGrid = new BaseObject(null);
			mGrid.AddComponent(Grid);
			Renderer.Instance.AddObject(mGrid);

			mFPSTimer.Start();
			mMouseState = new MouseEventArgs(MouseButtons.None, 0, 0, 0, 0);
			

			spWorldView.Panel2.ControlAdded += ReorderComponents;
			spWorldView.Panel2.ControlRemoved += ReorderComponents;
			spWorldView.Panel2.AutoScroll = true;

			btnFocus.Size = new Size(0, 0);
			btnFocus.Select();

			btnComponentAdd.Size = new Size(spWorldView.Panel2.ClientRectangle.Width / 2, btnComponentAdd.Height);
			btnComponentAdd.Location = new Point(spWorldView.Panel2.ClientRectangle.Width / 4, btnComponentAdd.Location.Y);
			btnComponentAdd.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right;
			btnComponentAdd.Margin = new Padding(0, 15, 0, 15);
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
		}


		private void graphicsPanel1_MouseLeave(object sender, EventArgs e)
		{
		}

		private void graphicsPanel1_MouseClick(object sender, MouseEventArgs e)
		{
			// Give the Focus textbox focus so that we can gladly accept input for the graphics panel.
			
		}

		private void graphicsPanel1_MouseDown(object sender, MouseEventArgs e)
		{
			bool hadFocus = btnFocus.ContainsFocus;
			btnFocus.Select();
			if (e.Button == MouseButtons.Left)
			{
				// Check against the gizmo first, because if that's selcted, we don't change targets
				if (RaycastGizmo())
				{
					Debug.Print("Gizmo Hit!");
					return;
				}


				TreeNode closest = null;
				float closestTime = float.MinValue;
				for (int i = 0; i < Tree.Nodes.Count; ++i)
				{
					RecursiveCheckRaycast(Tree.Nodes[i], e, ref closestTime, ref closest);
				}
				Debug.WriteLine("--------------------------------");
				if (closest != null)
				{
					Tree.SelectedNode = closest;
					BaseObject b = ((BaseObject)closest.Tag);
					Gizmo.Instance.Attach(((TransformComponent)b.GetComponents()[0]));
				}
				else
				{
					if(hadFocus) {
						Tree.SelectedNode = null;
					}
					Gizmo.Instance.Attach(null);

				}
			}
		}

		private void RecursiveCheckRaycast(TreeNode n, MouseEventArgs e, ref float closestTime, ref TreeNode closest)
		{
			Microsoft.DirectX.Direct3D.Device dev = Renderer.Instance.Device;

			BaseObject b = (BaseObject)n.Tag;
			List<Component> comps = b.GetComponents();
			for (int j = 0; j < comps.Count; ++j)
			{
				if (comps[j] is MeshComponent)
				{
					MeshComponent c = (MeshComponent)comps[j];
					Vector3 start = Vector3.Unproject(new Vector3(e.X, e.Y, 0),
						dev.Viewport,
						dev.Transform.Projection,
						dev.Transform.View,
						b.GetMatrix() * c.Shape.World);
					Vector3 end = Vector3.Unproject(new Vector3(e.X, e.Y, 1),
						dev.Viewport,
						dev.Transform.Projection,
						dev.Transform.View,
						b.GetMatrix() * c.Shape.World);
					Vector3 dir = (end - start);
					dir.Normalize();
					float time = 0;
					if (c.Shape.CheckRaycast(start, dir, out time))
					{
						// For reasons currently unbeknownst to me, the closer the object is to the camera, the *higher* the "time"
						// to hit the object we're pointing at is. This is not what anyone expects to check in order to find the
						// closest object to the camera, but since the axes here are pretty screwed up, that's what we're getting.

						// But yeah, this *is* currently the correct way to raycast to the closest object when we click on the screen.
						if (time > closestTime)
						{
							Debug.WriteLine("Shape '" + b.Name + "' is the closest!");
							closestTime = time;
							closest = n;
						}
					}
				}
			}
			for(int i = 0; i < n.Nodes.Count; ++i)
			{
				RecursiveCheckRaycast(n.Nodes[i], e, ref closestTime, ref closest);
			}
		}

		private bool RaycastGizmo()
		{
			if(!Gizmo.Instance.Valid)
			{
				return false;
			}
			ColoredShape[] gizmos = Gizmo.Instance.GetVisibleComponents();
			Microsoft.DirectX.Direct3D.Device dev = Renderer.Instance.Device;
			float time = float.MinValue;
			int closest = -1;
			for (int i = 0; i < gizmos.Length; ++i)
			{
				Vector3 start = Vector3.Unproject(new Vector3(mMouseState.X, mMouseState.Y, 0),
					dev.Viewport,
					dev.Transform.Projection,
					dev.Transform.View,
					gizmos[i].World * Gizmo.Instance.Position
				);
				Vector3 end = Vector3.Unproject(new Vector3(mMouseState.X, mMouseState.Y, 1),
					dev.Viewport,
					dev.Transform.Projection,
					dev.Transform.View,
					gizmos[i].World * Gizmo.Instance.Position
				);
				Vector3 dir = end - start;
				dir.Normalize();
				float hit = 0;
				if(gizmos[i].CheckRaycast(start, dir, out hit))
				{
					if(hit > time)
					{
						time = hit;
						closest = i;
						Debug.Print("Hit gizmo " + i);
					}
				}
			}
			if(closest >= 0)
			{
				Gizmo.Instance.Grabbed = gizmos[closest];
				return true;
			} 
			return false;
		}

		private void graphicsPanel1_MouseMove(object sender, MouseEventArgs e)
		{
			if (ActiveForm != this)
			{
				// Ensure we're the active window.
				return;
			}
			// Update Mouse Input
			prevMouse = curMouse;
			curMouse = new Vector2(e.X, e.Y);
			mMouseState = e;

			Vector2 delta = curMouse - prevMouse;
			Microsoft.DirectX.Direct3D.Device dev = Renderer.Instance.Device;
			switch(e.Button)
			{
				case MouseButtons.Middle:
					Renderer.Instance.AngleY -= delta.X * mRotationSpeed;
					Renderer.Instance.AngleX += delta.Y * mRotationSpeed;
					Renderer.Instance.RebuildViewMatrix();
					break;
				case MouseButtons.Right:
					// Minus-equals because Right handed systems have positive Z going out of the screen.
					Renderer.Instance.CameraPosition -= Renderer.Instance.Forward * 0.05f * delta.X * Gizmo.Instance.ScaleFactor;
					UpdateGizmoScale();

					// TODO: When the cursor approaches the corners of the current monitor, wrap its position.
					break;
				case MouseButtons.Left:
					if(Gizmo.Instance.Grabbed != null)
					{
						Vector3 prev = Vector3.Unproject(new Vector3(prevMouse.X, prevMouse.Y, 0),
							dev.Viewport,
							dev.Transform.Projection,
							dev.Transform.View,
							Gizmo.Instance.Grabbed.World
						);
						Vector3 curr = Vector3.Unproject(new Vector3(e.X, e.Y, 0),
							dev.Viewport,
							dev.Transform.Projection,
							dev.Transform.View,
                            Gizmo.Instance.Grabbed.World
                        );
						Vector3 pr = Renderer.Instance.RotateInto(prev, Gizmo.Instance.Grabbed.World);
						Vector3 cr = Renderer.Instance.RotateInto(curr, Gizmo.Instance.Grabbed.World);
						Vector3 deltaCast = Renderer.Instance.RotateInto(cr - pr, Renderer.Instance.View);

						Vector3 gPos = new Vector3(Gizmo.Instance.Position.M41, Gizmo.Instance.Position.M42, Gizmo.Instance.Position.M43);
						Matrix view = Renderer.Instance.View;
						view.Invert();
						Vector3 cPos = new Vector3(view.M41, view.M42, view.M43);
						float gizmoScale = (cPos - gPos).LengthSq();
						deltaCast.Multiply(mGizmoSpeed * gizmoScale);
						Gizmo.Instance.Apply(deltaCast);
						UpdateGizmoScale();
					}
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
			mPreviousKeys = mKeys;
			mKeys = new List<Key>(mKeyboard.GetPressedKeys());
			if (btnFocus.ContainsFocus)
			{
				// Lateral movement, only if no Control key is held down.
				if (!mKeys.Contains(Key.LeftControl) && !mKeys.Contains(Key.RightControl))
				{
					bool CameraMoved = false;
					if (mKeys.Contains(Key.W))
					{
						Renderer.Instance.CameraPosition -= Renderer.Instance.Forward * 0.1f;
						CameraMoved = true;
					}
					if (mKeys.Contains(Key.S))
					{
						Renderer.Instance.CameraPosition += Renderer.Instance.Forward * 0.1f;
						CameraMoved = true;
					}
					if (mKeys.Contains(Key.A))
					{
						Renderer.Instance.CameraPosition -= Renderer.Instance.Right * 0.1f;
						CameraMoved = true;
					}
					if (mKeys.Contains(Key.D))
					{
						Renderer.Instance.CameraPosition += Renderer.Instance.Right * 0.1f;
						CameraMoved = true;
					}
					if (mKeys.Contains(Key.Space))
					{
						Renderer.Instance.CameraPosition += Renderer.Instance.Up * 0.1f;
						CameraMoved = true;
					}
					if (mKeys.Contains(Key.LeftShift )|| mKeys.Contains(Key.RightShift))
					{
						Renderer.Instance.CameraPosition -= Renderer.Instance.Up * 0.1f;
						CameraMoved = true;
					}
					if(CameraMoved) {
						UpdateGizmoScale();
					}
				} // END Not holding control
                else
                {
                    if (mKeys.Contains(Key.Q))
                    {
                        Gizmo.Instance.Mode = Gizmo.GizmoMode.Position;
                    }
                    if (mKeys.Contains(Key.W))
                    {
                        Gizmo.Instance.Mode = Gizmo.GizmoMode.Rotation;
                    }
                    if (mKeys.Contains(Key.E))
                    {
                        Gizmo.Instance.Mode = Gizmo.GizmoMode.Scale;
                    }
                }
				// If we press delete, destroy the current node and object.
				// Check the previous key state to ensure we don't delete more than one per press.
				if (!mKeys.Contains(Key.LeftControl) && !mKeys.Contains(Key.RightControl))
				{
					if (mKeys.Contains(Key.Delete) && !mPreviousKeys.Contains(Key.Delete))
					{
						if (Tree.SelectedNode != null)
						{
							RemoveTreeNode(Tree.SelectedNode);
							Tree_AfterSelect(null, null);
							Gizmo.Instance.Attach(null);
							btnFocus.Select();
						}
					}
				}
			}  // Ensure the Grapics Panel's focus textbox is selected.
			mPreviousKeys = mKeys;
		}

		private void UpdateGizmoScale() {
			Vector3 gPos = new Vector3(Gizmo.Instance.Position.M41, Gizmo.Instance.Position.M42, Gizmo.Instance.Position.M43);
			Matrix view = Renderer.Instance.View;
			view.Invert();
			Vector3 cPos = new Vector3(view.M41, view.M42, view.M43);
			Gizmo.Instance.ScaleFactor = (cPos - gPos).Length() * 0.1f;
		}

		private void RemoveTreeNode(TreeNode n)
		{
			BaseObject b = (BaseObject)n.Tag;
			b.Delete();
			Renderer.Instance.RemoveObject(b);
			for (int i = n.Nodes.Count - 1; i >= 0; --i)
			{
				RemoveTreeNode(n.Nodes[i]);
			}
			Tree.Nodes.Remove(n);
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
				case "TMesh":
					obj.AddComponent(new TexturedMeshComponent());
					break;
				case "CMesh":
					obj.AddComponent(new ColoredMeshComponent());
					break;
				case "Audio":
                    SoundComponent sc = new SoundComponent();
                    sc.Resize += ReorderComponents;
                    sc.Parent = spWorldView.Panel2;
                    obj.AddComponent(sc);
					break;

                // Code Components
                case "CodeComp":
                    obj.AddComponent(new CodeComponent());
                    break;

                default:
					break;
			}
		}

		private void openToolStripMenuItem_Click(object sender, EventArgs e)
		{
			OpenFileDialog o = new OpenFileDialog();
			o.Filter = "Epoch Level Files (*.elf)|*.elf|Legacy XML Level File (*.xml)|*.xml";
			o.FilterIndex = 1;
			o.Title = "Open a level...";
			if(o.ShowDialog() == DialogResult.OK)
			{
				newToolStripMenuItem_Click(null, null);
				if (o.FileName.EndsWith(".xml")) {
					FileIO.ReadXMLFile(o.FileName, Tree);
				} else {
					FileIO.openLevel(o.FileName, Tree);
				}
				// Attach Object Handlers
				for(int i = 0; i < Tree.Nodes.Count; ++i)
				{
					PostLoadSetup(Tree.Nodes[i]);
				}
			}
		}

		private void saveToolStripMenuItem_Click(object sender, EventArgs e)
		{
			if (currentFile != string.Empty)
			{
				FileIO.saveLevel(currentFile, Tree);
			}
			else
			{
				SaveFileDialog file = new SaveFileDialog();
				file.InitialDirectory = Application.StartupPath;
				file.Filter = "Epoch Level Files (*.elf)|*.elf";
				file.FilterIndex = 1;
				file.RestoreDirectory = true;
				if (file.ShowDialog() == DialogResult.OK)
				{
					FileIO.saveLevel(file.FileName, Tree);
				}
			}
		}

		private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
		{
			SaveFileDialog saveFile = new SaveFileDialog();
			saveFile.InitialDirectory = Application.StartupPath;
			saveFile.Filter = "Epoch files (*.elf)|*.elf";
			saveFile.FilterIndex = 1;
			saveFile.RestoreDirectory = true;
			if (saveFile.ShowDialog() == DialogResult.OK)
			{
				currentFile = saveFile.FileName;
				FileIO.saveLevel(saveFile.FileName, Tree);
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
			if (Tree.SelectedNode != null)
			{
				BaseObject obj = (BaseObject)Tree.SelectedNode.Tag;
				for (int i = 0; i < obj.GetComponents().Count; ++i)
				{
					spWorldView.Panel2.Controls.Add(obj.GetComponents()[i].GetGroupbox());
				}
				Gizmo.Instance.Attach((IGizmoAttachment)obj.GetComponents()[0]);
			}
			btnFocus.Select();
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
			if(_parent != null)
			{
				b.Parent = (BaseObject)_parent.Tag;
			}
			b.ComponentAdded += ObjectAddComponent;
			b.ComponentRemoved += ObjectRemoveComponent;
			Renderer.Instance.AddObject(b);
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

		private void PostLoadSetup(TreeNode n)
		{
			for(int i = 0; i < n.Nodes.Count; ++i)
			{
				PostLoadSetup(n.Nodes[i]);
			}
			((BaseObject)n.Tag).ComponentAdded += ObjectAddComponent;
			((BaseObject)n.Tag).ComponentRemoved += ObjectRemoveComponent;

			// Add the loaded objects into the renderer so we can see them.
			Renderer.Instance.AddObject((BaseObject)n.Tag);
		}

		private void levelSettingsToolStripMenuItem_Click(object sender, EventArgs e)
		{
			Forms.LevelSettingsForm settings = new Forms.LevelSettingsForm();
			settings.SetPosition(Settings.StartPos);
			settings.SetRotation(Settings.StartRot);
			if (settings.ShowDialog() == DialogResult.OK)
			{
				Settings.StartPos = settings.GetPosition();
				Settings.StartRot = settings.GetRotation();
			}
		}

		private void ObjectAddComponent(Component _c)
		{
			spWorldView.Panel2.Controls.Add(_c.GetGroupbox());
			ReorderComponents(spWorldView.Panel2, EventArgs.Empty);
		}

		private void Editor_Load(object sender, EventArgs e)
		{
			bool settingsLoaded = FileIO.LoadSettings();
			if (!settingsLoaded)
			{
				EditorSetupForm setup = new EditorSetupForm();
				if (setup.ShowDialog() == DialogResult.OK)
				{
					Settings.ProjectPath = setup.tbProjDir.Text;
				}
				else
				{
					this.Close();
				}
			}
			ResourceManager.Instance.BuildAssetList();
		}

		private void Editor_FormClosing(object sender, FormClosingEventArgs e)
		{
			if(!string.IsNullOrWhiteSpace(Settings.ProjectPath))
			{
				FileIO.SaveSettings();
			}
		}

		private void spWorldView_Panel2_Click(object sender, EventArgs e)
		{
			btnFocus.Select();
		}

		private void Editor_ClientSizeChanged(object sender, EventArgs e)
		{
			// Ensure the graphics panel can't get too small.
			spHierarchyPanel.Panel2MinSize = ClientRectangle.Width - 210;
			spWorldView.Panel1MinSize = ClientRectangle.Width - 210 - 260;
			spHierarchyPanel.PerformLayout();
			spWorldView.PerformLayout();
		}

		private void ObjectRemoveComponent(Component _c)
		{
			spWorldView.Panel2.Controls.Remove(_c.GetGroupbox());
			ReorderComponents(spWorldView.Panel2, EventArgs.Empty);
		}

		private void ReorderComponents(object sender, EventArgs e)
		{
			if(Tree.SelectedNode == null)
			{
				btnComponentAdd.Visible = false;
				return;
			}
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
			AnchorStyles LTR = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right;
			for (int i = 0; i < obj.GetComponents().Count; ++i)
			{
				GroupBox box = obj.GetComponents()[i].GetGroupbox();
				box.Location = position;
				position.Y += box.Size.Height + 3;

				dimensions.Height = box.Height;
				box.Size = dimensions;
				box.Anchor = LTR;
			}
			position.Y += 15;
			position.X = btnComponentAdd.Left;
			btnComponentAdd.Location = position;
			btnComponentAdd.Visible = true;
			btnComponentAdd.Margin = new Padding(30);
		}

        private void graphicsPanel1_Resize(object sender, EventArgs e)
        {
        }

		private void newToolStripMenuItem_Click(object sender, EventArgs e) {
			for(int i = Tree.Nodes.Count - 1; i >= 0; --i) {
				DeleteNode(Tree.Nodes[i]);
			}
		}

		private void DeleteNode(TreeNode n) {
			for (int i = n.Nodes.Count - 1; i >= 0; --i) {
				DeleteNode(n.Nodes[i]);
			}
			((BaseObject)n.Tag).Delete();
			n.Tag = null;
			if(n.Parent != null) {
				n.Parent.Nodes.Remove(n);
			} else {
				n.TreeView.Nodes.Remove(n);
			}
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
