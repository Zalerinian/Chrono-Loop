using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace Hourglass
{
    public class MeshComponent : Component
    {
        protected System.Drawing.Font mPlaceholderFont, mActiveFont;

        protected Label mLbMesh, mLbDiffuse;
        protected TextBox mMesh, mDiffuse;
        protected Button mMeshSelect, mDiffuseSelect;

        public struct TextBoxTag
        {
            public bool Placeholder;
            public string PlaceholderText;

            public TextBoxTag(bool _p, string _t)
            {
                Placeholder = _p;
                PlaceholderText = _t;
            }
        }

        public struct ButtonTag
        {
            public TextBox FillBox;
            public string SearchFilter;
            public string SearchTitle;

            public ButtonTag(TextBox _f, string _filter, string _title)
            {
                FillBox = _f;
                SearchFilter = _filter;
                SearchTitle = _title;
            }
        }

        public MeshComponent(BaseObject _owner) : base(_owner)
        {
            #region Component Creation

            mLbMesh = new Label();
            mLbDiffuse = new Label();

            mMesh = new TextBox();
            mDiffuse = new TextBox();

            mMeshSelect = new Button();
            mDiffuseSelect = new Button();

            mGroupBox.Controls.Add(mLbMesh);
            mGroupBox.Controls.Add(mLbDiffuse);
            mGroupBox.Controls.Add(mMesh);
            mGroupBox.Controls.Add(mDiffuse);
            mGroupBox.Controls.Add(mMeshSelect);
            mGroupBox.Controls.Add(mDiffuseSelect);

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

            mMeshSelect.Anchor = AnchorStyles.Top | AnchorStyles.Right;
            mMeshSelect.Size = new System.Drawing.Size(26, 23);
            mMeshSelect.Location = new System.Drawing.Point(mGroupBox.Right - mMeshSelect.Width, 20);
            mMeshSelect.Text = "...";
            mMeshSelect.Tag = new ButtonTag(mMesh, "Mesh Files (*.obj)|*.obj", "Select a mesh for this object...");
            mMeshSelect.Click += OnFileSelectorClick;

            mDiffuseSelect.Anchor = AnchorStyles.Top | AnchorStyles.Right;
            mDiffuseSelect.Size = new System.Drawing.Size(26, 23);
            mDiffuseSelect.Location = new System.Drawing.Point(mGroupBox.Right - mDiffuseSelect.Width, 50);
            mDiffuseSelect.Text = "...";
            mDiffuseSelect.Tag = new ButtonTag(mDiffuse, "Texture Files (*.png)|*.png", "Select a texture for this object...");
            mDiffuseSelect.Click += OnFileSelectorClick;

            mMesh.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right;
            mMesh.Location = new System.Drawing.Point(90, 21);
            mMesh.Size = new System.Drawing.Size(ContentWidth - mMesh.Left - mMeshSelect.Width, 24);
            mMesh.Tag = new TextBoxTag(true, "Mesh File...");
            mMesh.GotFocus += OnTextBoxGetFocus;
            mMesh.LostFocus += OnTextBoxLostFocus;

            mDiffuse.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right;
            mDiffuse.Location = new System.Drawing.Point(90, 51);
            mDiffuse.Size = new System.Drawing.Size(ContentWidth - mDiffuse.Left - mDiffuseSelect.Width, 24);
            mDiffuse.Tag = new TextBoxTag(true, "Texture File...");
            mDiffuse.GotFocus += OnTextBoxGetFocus;
            mDiffuse.LostFocus += OnTextBoxLostFocus;

            #endregion

            mGroupBox.Text = "Mesh Component";
            mGroupBox.Size = mGroupBox.PreferredSize;
            OnMenuClick_Reset(null, null);
        }

        protected override void OnMenuClick_Reset(object sender, EventArgs e)
        {
            mMesh.Text = "Mesh File...";
            mMesh.Font = mPlaceholderFont;
            mMesh.ForeColor = System.Drawing.SystemColors.ControlDark;
            mMesh.Tag = new TextBoxTag(true, "Mesh File...");

            mDiffuse.Text = "Texture File...";
            mDiffuse.Font = mPlaceholderFont;
            mDiffuse.ForeColor = System.Drawing.SystemColors.ControlDark;
            mDiffuse.Tag = new TextBoxTag(true, "Texture File...");
        }

        protected void OnTextBoxGetFocus(object sender, EventArgs e)
        {
            if(!(sender is TextBox))
            {
                return;
            }
            TextBox tb = sender as TextBox;
            // The tag on the text boxes is whether or not the text box is in placeholder mode.
            if (((TextBoxTag)tb.Tag).Placeholder)
            {
                TextBoxTag tag = ((TextBoxTag)tb.Tag);
                tag.Placeholder = false;
                tb.Font = mActiveFont;
                tb.ForeColor = System.Drawing.SystemColors.ControlText;
                tb.Text = "";
            }
        }

        protected void OnTextBoxLostFocus(object sender, EventArgs e)
        {
            if (!(sender is TextBox))
            {
                return;
            }
            TextBox tb = sender as TextBox;
            if (String.IsNullOrWhiteSpace(tb.Text))
            {
                TextBoxTag tag = ((TextBoxTag)tb.Tag);
                tag.Placeholder = true;
                tb.Font = mPlaceholderFont;
                tb.ForeColor = System.Drawing.SystemColors.ControlDark;
                tb.Text = ((TextBoxTag)tb.Tag).PlaceholderText;
            }
        }

        protected void OnFileSelectorClick(object sender, EventArgs e)
        {
            if(!(sender is Button))
            {
                return;
            }
            Button btn = sender as Button;
            ButtonTag tag = (ButtonTag)btn.Tag;
            TextBoxTag ttag = (TextBoxTag)tag.FillBox.Tag;
            OpenFileDialog file = new OpenFileDialog();
            file.Filter = tag.SearchFilter;
            file.Title = tag.SearchTitle;
            file.FilterIndex = 1;
            file.Multiselect = false;
            if(file.ShowDialog() == DialogResult.OK)
            {
                ttag.Placeholder = false;
                tag.FillBox.Tag = ttag;
                tag.FillBox.Font = mActiveFont;
                tag.FillBox.ForeColor = System.Drawing.SystemColors.ControlText;
                tag.FillBox.Text = file.FileName;
            }
        }

    }

}