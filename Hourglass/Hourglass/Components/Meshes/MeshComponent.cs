using System;
using System.Windows.Forms;

namespace Hourglass
{
    public class MeshComponent : Component
    {
        protected System.Drawing.Font mPlaceholderFont, mActiveFont;

        protected Label mLbMesh, mLbDiffuse;
		protected ComboBox mMesh;

        public MeshComponent(int _yOffset = 0) : base()
        {
            #region Component Creation

            mLbMesh = new Label();
            mLbDiffuse = new Label();

            mMesh = new ComboBox();

            mGroupBox.Controls.Add(mLbMesh);
            mGroupBox.Controls.Add(mLbDiffuse);
            mGroupBox.Controls.Add(mMesh);

            #endregion

            #region Component Setup
            int ContentWidth = (mGroupBox.Size - mGroupBox.Padding.Size - mGroupBox.Margin.Size).Width;

            // Fonts
            mPlaceholderFont = new System.Drawing.Font("Calibri", 8.25f, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            mActiveFont = new System.Drawing.Font("Calibri", 8.25f, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));

            mLbMesh.AutoSize = true;
            mLbMesh.Location = new System.Drawing.Point(6, 26);
            mLbMesh.Name = "mLbMesh";
            mLbMesh.Text = "Mesh File";

            mLbDiffuse.AutoSize = true;
            mLbDiffuse.Location = new System.Drawing.Point(6, 55);
            mLbDiffuse.Name = "mLbDiffuse";
            mLbDiffuse.Text = "Diffuse Texture";

			mMesh.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right;
			mMesh.Location = new System.Drawing.Point(90, 21 + _yOffset);
			mMesh.Size = new System.Drawing.Size(ContentWidth - mMesh.Left, 24);
			mMesh.DropDownStyle = ComboBoxStyle.DropDownList;

			#endregion

			mGroupBox.Text = "Mesh Component";
            mGroupBox.Size = mGroupBox.PreferredSize;
            OnMenuClick_Reset(null, null);
        }

        protected override void OnMenuClick_Reset(object sender, EventArgs e)
        {
            mMesh.SelectedIndex = -1;
			ReleaseControl();
		}
    }
}