using System;
using System.IO;
using System.Windows.Forms;
using System.Collections.Generic;

namespace Hourglass {
	public abstract class MeshComponent : Component, IRenderable {
		protected System.Drawing.Font mPlaceholderFont, mActiveFont;

		protected Label mLbMesh, mLbTransparency, mLbPixel, mLbVertex, mLbGeo;
		protected ComboBox mMesh, mPixel, mVertex, mGeo;
		protected RenderShape mShape = null;
		protected NumericUpDown mTransparency;

		protected string[] PixelShaders = {
			"ePS_BASIC",
			"ePS_TEXTURED",
			"ePS_POSTPROCESS",
			"ePS_PURETEXTURE",
			"ePS_TRANSPARENT",
			"ePS_TRANSPARENT_SCANLINE"
		};

		protected string[] VertexShaders = {
			"eVS_BASIC",
			"eVS_TEXTURED",
			"eVS_NDC"
		};

		protected string[] GeoShaders = {
			"eGS_PosNormTex",
			"eGS_PosNormTex_NDC",
			"eGS_PosNormTex_AnimQuad"
		};

		public RenderShape Shape {
			get {
				return mShape;
			}
		}

		virtual public int RenderStage {
			get {
				return 0;
			}
		}

		public MeshComponent(int _yOffset = 0) : base() {
			#region Component Creation

			mLbMesh = new Label();
			mLbTransparency = new Label();
			mLbPixel = new Label();
			mLbVertex = new Label();
			mLbGeo = new Label();

			mMesh = new ComboBox();
			mPixel = new ComboBox();
			mVertex = new ComboBox();
			mGeo = new ComboBox();


			mTransparency = new NumericUpDown();

			mGroupBox.Controls.Add(mLbMesh);
			mGroupBox.Controls.Add(mLbTransparency);
			mGroupBox.Controls.Add(mLbPixel);
			mGroupBox.Controls.Add(mLbVertex);
			mGroupBox.Controls.Add(mLbGeo);
			mGroupBox.Controls.Add(mTransparency);
			mGroupBox.Controls.Add(mMesh);
			mGroupBox.Controls.Add(mPixel);
			mGroupBox.Controls.Add(mVertex);
			mGroupBox.Controls.Add(mGeo);

			#endregion

			#region Component Setup
			int ContentWidth = (mGroupBox.Size - mGroupBox.Padding.Size - mGroupBox.Margin.Size).Width;

			// Fonts
			mPlaceholderFont = new System.Drawing.Font("Calibri", 8.25f, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			mActiveFont = new System.Drawing.Font("Calibri", 8.25f, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));

			mLbMesh.AutoSize = true;
			mLbMesh.Location = new System.Drawing.Point(6, 20 + _yOffset);
			mLbMesh.Name = "mLbMesh";
			mLbMesh.Text = "Mesh File";

			mLbPixel.AutoSize = true;
			mLbPixel.Location = new System.Drawing.Point(6, 53 + _yOffset);
			mLbPixel.Name = "mLbPixel";
			mLbPixel.Text = "Pixel Shader";

			mLbVertex.AutoSize = true;
			mLbVertex.Location = new System.Drawing.Point(6, 86 + _yOffset);
			mLbVertex.Name = "mLbVertex";
			mLbVertex.Text = "Vertex Shader";

			mLbGeo.AutoSize = true;
			mLbGeo.Location = new System.Drawing.Point(6, 119 + _yOffset);
			mLbGeo.Name = "mLbGeo";
			mLbGeo.Text = "Geo Shader";

			mLbTransparency.AutoSize = true; // Unneccessary?
			mLbTransparency.Text = "Transparency";
			mLbTransparency.Location = new System.Drawing.Point(6, 150 + _yOffset);
			mLbTransparency.Name = "mLbTransparency";


			mMesh.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right;
			mMesh.Location = new System.Drawing.Point(90, 18 + _yOffset);
			mMesh.Size = new System.Drawing.Size(ContentWidth - mMesh.Left, 24);
			mMesh.DropDownStyle = ComboBoxStyle.DropDownList;

			mPixel.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right;
			mPixel.Location = new System.Drawing.Point(90, 51 + _yOffset);
			mPixel.Size = new System.Drawing.Size(ContentWidth - mPixel.Left, 24);
			mPixel.DropDownStyle = ComboBoxStyle.DropDownList;

			mVertex.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right;
			mVertex.Location = new System.Drawing.Point(90, 84 + _yOffset);
			mVertex.Size = new System.Drawing.Size(ContentWidth - mVertex.Left, 24);
			mVertex.DropDownStyle = ComboBoxStyle.DropDownList;

			mGeo.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right;
			mGeo.Location = new System.Drawing.Point(90, 117 + _yOffset);
			mGeo.Size = new System.Drawing.Size(ContentWidth - mGeo.Left, 24);
			mGeo.DropDownStyle = ComboBoxStyle.DropDownList;


			mTransparency.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right;
			mTransparency.Location = new System.Drawing.Point(90, 148 + _yOffset);
			mTransparency.Size = new System.Drawing.Size(ContentWidth - mTransparency.Left, 24);
			mTransparency.Minimum = 0;
			mTransparency.Maximum = 1;
			mTransparency.Increment = (decimal)0.05f;
			mTransparency.DecimalPlaces = 2;

			{
				List<string>.Enumerator it = ResourceManager.Instance.Objects.GetEnumerator();
				while (it.MoveNext()) {
					mMesh.Items.Add(it.Current);
				}
			}

			for(int i = 0; i < PixelShaders.Length; ++i) {
				mPixel.Items.Add(PixelShaders[i]);
			}

			for (int i = 0; i < VertexShaders.Length; ++i) {
				mVertex.Items.Add(VertexShaders[i]);
			}

			for (int i = 0; i < GeoShaders.Length; ++i) {
				mGeo.Items.Add(GeoShaders[i]);
			}

			mPixel.SelectedIndex = CheckForValue(mPixel, "ePS_TEXTURED");
			mVertex.SelectedIndex = CheckForValue(mVertex, "eVS_TEXTURED");
			mGeo.SelectedIndex = CheckForValue(mGeo, "eGS_PosNormTex");

			#endregion

			mGroupBox.Text = "Mesh Component";
			mGroupBox.Size = mGroupBox.PreferredSize;
			//OnMenuClick_Reset(null, null);
		}

		public override void OnMenuClick_Reset(object sender, EventArgs e) {
			mMesh.SelectedIndex = -1;
			if (mShape != null) {
				mShape.Dispose();
			}
			mTransparency.Value = 1;
			mPixel.SelectedIndex = CheckForValue(mPixel, "ePS_TEXTURED");
			mVertex.SelectedIndex = CheckForValue(mVertex, "eVS_TEXTURED");
			mGeo.SelectedIndex = CheckForValue(mGeo, "eGS_PosNormTex");
			ReleaseControl();
		}

		public override void WriteData(BinaryWriter w) {
			base.WriteData(w);

			// Version 2
			w.Write((float)mTransparency.Value);

			// Version 3
			w.Write((byte)mPixel.SelectedIndex);
			w.Write((byte)mVertex.SelectedIndex);
			w.Write((byte)mGeo.SelectedIndex);

			// version 1
			string s = ".." + ResourceManager.Instance.ResourceDirectory + mMesh.Text;
			w.Write(s.Length + 1);
			w.Write(s.ToCharArray());
			byte term = 0;
			w.Write(term);
		}

		public override void ReadData(BinaryReader r, int _version) {
			if (_version >= 2) {
				mTransparency.Value = (decimal)(System.BitConverter.ToSingle(r.ReadBytes(4), 0));
			}
			if(_version >= 3) {
				int shaderIndex = r.ReadByte();
				shaderIndex = shaderIndex == 255 ? -1 : shaderIndex;
				mPixel.SelectedIndex = shaderIndex;

				shaderIndex = r.ReadByte();
				shaderIndex = shaderIndex == 255 ? -1 : shaderIndex;
				mVertex.SelectedIndex = shaderIndex;

				shaderIndex = r.ReadByte();
				shaderIndex = shaderIndex == 255 ? -1 : shaderIndex;
				mGeo.SelectedIndex = shaderIndex;
			}
			string filename = new string(r.ReadChars(r.ReadInt32() - 1));
			r.ReadByte(); // The null terminator breaks things in C#, but is necessary in C++, so we need to skip it in C#
			filename = filename.Substring(filename.LastIndexOf("\\") + 1);
			int index = mMesh.Items.IndexOf(filename);
			if (index >= 0) {
				mMesh.SelectedIndex = index;
			}
		}

		protected int CheckForValue(ComboBox _box, string _value) {
			for (int i = 0; i < _box.Items.Count; ++i) {
				if (((string)_box.Items[i]).ToLower() == _value.ToLower()) {
					return i;
				}
			}
			return -1;
		}

		public int CheckForMesh(string _object) {
			return CheckForValue(mMesh, _object);
		}

		public void SelectMesh(int _index) {
			if (_index > 0 && _index < mMesh.Items.Count) {
				mMesh.SelectedIndex = _index;
			}
		}

		public override void CopyData(ref Component _other) {
			if (!(_other is MeshComponent)) {
				throw new InvalidDataException("Given component does not match the calling type.");
			}
			MeshComponent comp = _other as MeshComponent;
			comp.mMesh.SelectedIndex = mMesh.SelectedIndex;
			comp.mTransparency.Value = mTransparency.Value;
			comp.mPixel.SelectedIndex = mPixel.SelectedIndex;
			comp.mVertex.SelectedIndex = mVertex.SelectedIndex;
			comp.mGeo.SelectedIndex = mGeo.SelectedIndex;
		}

	}
}