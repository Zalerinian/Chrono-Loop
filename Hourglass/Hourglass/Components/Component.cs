using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace Hourglass
{
    public abstract class Component
    {
		public enum ComponentType { None = 0, BoxCollider, ButtonCollider, PlaneCollider, SphereCollider, ColoredMesh, TexturedMesh, Transform, Code, Audio, Light, MAX }



		public delegate void GenericEventHandler();
		public event GenericEventHandler OwnerChanged;
		public event GenericEventHandler RemoveControl;

        protected FocusGroupBox mGroupBox;
        protected ContextMenuStrip mMenuStrip;
        protected ToolStripMenuItem mMenuItemDelete, mMenuItemReset;
        protected BaseObject mOwner = null;
		protected ComponentType mType = ComponentType.None;

        private static readonly System.Drawing.Font 
            mPlaceholderFont = new System.Drawing.Font("Microsoft Sans Serif", 8.25f, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Point, ((byte)(0))),
            mActiveFont = new System.Drawing.Font("Microsoft Sans Serif", 8.25f, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));

        public int Width {
            get {
                return mGroupBox.Size.Width;
            }
        }

        public int Height {
            get {
                return mGroupBox.Size.Height;
            }
        }

		public ComponentType Type {
			get {
				return mType;
			}
		}

        public System.Drawing.Font PlaceholderFont {
            get { return mPlaceholderFont; }
        }

        public System.Drawing.Font ActiveFont {
            get { return mActiveFont; }
        }

		public BaseObject Owner {
			get {
				return mOwner;
			}
			set {
				mOwner = value;
				if(OwnerChanged != null)
				{
					OwnerChanged();
				}
			}
		}

        /// <summary>
        ///     A component is a property of an object that affects what it does in the
        ///     world.
        ///     When writing components to an XML File, the internal name of the controls
        ///     that hold values will be used as the XML keys for their values when written
        ///     in Text mode.
        /// </summary>
        /// <param name="_owner">
        ///     The base object that this component is a part of. This allows us to update 
        ///     any necessary data with the TreeNode.
        /// </param>
        /// <param name="_destructible">
        ///     Whether or not this component can be deleted from an object.
        ///     This is true for everything except an object's transform (Position,
        ///     Rotation, Scale), and its name.
        /// </param>
        public Component(bool _destructible = true)
        {

            mGroupBox = new FocusGroupBox();
            mGroupBox.AutoSize = false;
            mMenuStrip = new ContextMenuStrip();
            mMenuItemReset = new ToolStripMenuItem("Reset", null, OnMenuClick_Reset);
            mMenuItemDelete = new ToolStripMenuItem("Delete", null, OnMenuClick_Delete);
            mMenuStrip.Items.Add(mMenuItemReset);
            if (_destructible)
            {
                mMenuStrip.Items.Add(mMenuItemDelete);
            }

            mGroupBox.Size = new System.Drawing.Size(0, 0);
            mGroupBox.ContextMenuStrip = mMenuStrip;
            mGroupBox.Click += OnGroupBoxClick;
        }

        public GroupBox GetGroupbox()
        {
            return mGroupBox;
        }

		/// <summary>
		/// Removes focus from the component and gives it back to the main graphics panel.
		/// </summary>
		protected void ReleaseControl()
		{
			if(RemoveControl != null)
			{
				RemoveControl();
			}
		}

        public virtual void OnMenuClick_Delete(object sender, EventArgs e)
        {
            Owner.RemoveComponent(this);
			ReleaseControl();
		}

        public virtual void OnMenuClick_Reset(object sender, EventArgs e)
        {
            MessageBox.Show("Someone was lazy and didn't implement the Reset option for this component.",
                "Error: ID10-T",
                MessageBoxButtons.OK);
			ReleaseControl();
		}

        protected virtual void OnGroupBoxClick(object sender, EventArgs e)
        {
            mGroupBox.FindForm().ActiveControl = null;
        }


        protected void SetupTransformPanel(Panel _panel, int _x, int _y, int _contentWidth, Label _lx, Label _ly, Label _lz, NumericUpDown _nx, NumericUpDown _ny, NumericUpDown _nz)
        {
            _panel.AutoSize = false;
            _panel.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right;
            _panel.Location = new System.Drawing.Point(_x, _y);
            _panel.Size = new System.Drawing.Size(_contentWidth - _panel.Left, 20);

            _lx.Text = "X";
            _lx.Size = new System.Drawing.Size(14, 16);

            _ly.Text = "Y";
            _ly.Size = new System.Drawing.Size(14, 16);

            _lz.Text = "Z";
            _lz.Size = new System.Drawing.Size(14, 16);

            _nx.DecimalPlaces = 2;
            _nx.Increment = new decimal(0.1);
            _nx.Maximum = new decimal(50000.0);
            _nx.Minimum = new decimal(-50000.0);

            _ny.DecimalPlaces = 2;
            _ny.Increment = new decimal(0.1);
            _ny.Maximum = new decimal(50000.0);
            _ny.Minimum = new decimal(-50000.0);

            _nz.DecimalPlaces = 2;
            _nz.Increment = new decimal(0.1);
            _nz.Maximum = new decimal(50000.0);
            _nz.Minimum = new decimal(-50000.0);

            _panel.Controls.Add(_lx);
            _panel.Controls.Add(_ly);
            _panel.Controls.Add(_lz);
            _panel.Controls.Add(_nx);
            _panel.Controls.Add(_ny);
            _panel.Controls.Add(_nz);
            ResizeTransformPanel(_panel, null);
            _panel.Resize += ResizeTransformPanel;
        }

        protected void ResizeTransformPanel(object sender, EventArgs e)
        {
            // This assumes that the first 3 children of the panel are the Labels, and the 
            // last three are the X, Y, and Z Numeric Up-Downs in order.
            Panel panel = sender as Panel;
            Label lx = panel.Controls[0] as Label,
                ly = panel.Controls[1] as Label,
                lz = panel.Controls[2] as Label;
            NumericUpDown nx = panel.Controls[3] as NumericUpDown,
                ny = panel.Controls[4] as NumericUpDown,
                nz = panel.Controls[5] as NumericUpDown;
            int ContentWidth = (panel.Size - panel.Margin.Size - panel.Padding.Size).Width;
            int nudSize = (ContentWidth - (lx.Size.Width * 3)) / 3;
            System.Drawing.Size size = new System.Drawing.Size();

            size.Height = nx.Height;
            size.Width = nudSize;

            // X Label
            lx.Top = 3;
            lx.Left = 0;

            // X Numeric Up-Down
            nx.Size = size;
            nx.Top = 0;
            nx.Left = lx.Left + lx.Size.Width;

            // Y Label
            ly.Top = 3;
            ly.Left = 2 + nx.Right;

            // Y Numeric Up-Down
            ny.Size = size;
            ny.Top = 0;
            ny.Left = ly.Left + ly.Size.Width;

            // Z Label
            lz.Top = 3;
            lz.Left = 2 + ny.Right;

            // Z Numeric Up-Down
            nz.Size = size;
            nz.Top = 0;
            nz.Left = lz.Left + lz.Size.Width;
        }

		public virtual void WriteData(System.IO.BinaryWriter w)
		{
			w.Write((short)mType);
		}

		public abstract void ReadData(System.IO.BinaryReader r, int _version);

		public abstract void CopyData(ref Component _other);
    }
}