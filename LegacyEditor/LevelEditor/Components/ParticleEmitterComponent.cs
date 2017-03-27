using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Diagnostics;
using System.Xml;

namespace LevelEditor
{
    class ParticleEmitterComponent : Component
    {
        protected Label mLbMaxParticles, mLbTexture, mLbPPS, mLbLifetime;
        protected Label mLbSSize, mLbESize, mLbSAlpha, mLbEAlpha;
        protected Label mLbRadial, mLbPosition, mLbVelocity, mLbSColor, mLbEColor;
        protected Label mLbRX, mLbRY, mLbRZ, mLbPX, mLbPY, mLbPZ, mLbVX, mLbVY, mLbVZ;

        protected TextBox mTexture;
        protected Button mTextureSelector, mSColor, mEColor;

        protected NumericUpDown mLifetime, mSSize, mESize, mSAlpha, mEAlpha, mMaxParticles, mPPS;
        protected NumericUpDown mRX, mRY, mRZ, mPX, mPY, mPZ, mVX, mVY, mVZ;

        protected Panel mRadPanel, mPosPanel, mVelPanel;

        protected GroupBox mParticleBox;

        public ParticleEmitterComponent(List<Component> _container) : base(_container)
        {
            #region Component Creation
            mLbMaxParticles = new Label();
            mLbTexture      = new Label();
            mLbPPS          = new Label();
            mLbLifetime     = new Label();
            mLbSSize        = new Label();
            mLbESize        = new Label();
            mLbSAlpha       = new Label();
            mLbEAlpha       = new Label();
            mLbRadial       = new Label();
            mLbPosition     = new Label();
            mLbVelocity     = new Label();
            mLbSColor       = new Label();
            mLbEColor       = new Label();
            mLbRX           = new Label();
            mLbRY           = new Label();
            mLbRZ           = new Label();
            mLbPX           = new Label();
            mLbPY           = new Label();
            mLbPZ           = new Label();
            mLbVX           = new Label();
            mLbVY           = new Label();
            mLbVZ           = new Label();

            mTexture = new TextBox();

            mTextureSelector = new Button();
            mSColor = new Button();
            mEColor = new Button();

            mLifetime = new NumericUpDown();
            mSSize = new NumericUpDown();
            mESize = new NumericUpDown();
            mSAlpha = new NumericUpDown();
            mEAlpha = new NumericUpDown();
            mMaxParticles = new NumericUpDown();
            mPPS = new NumericUpDown();
            mRX = new NumericUpDown();
            mRY = new NumericUpDown();
            mRZ = new NumericUpDown();
            mPX = new NumericUpDown();
            mPY = new NumericUpDown();
            mPZ = new NumericUpDown();
            mVX = new NumericUpDown();
            mVY = new NumericUpDown();
            mVZ = new NumericUpDown();

            mRadPanel = new Panel();
            mPosPanel = new Panel();
            mVelPanel = new Panel();

            mParticleBox = new GroupBox();


            mGroupBox.Controls.Add(mLbMaxParticles);
            mGroupBox.Controls.Add(mLbTexture);
            mGroupBox.Controls.Add(mLbPPS);
            mGroupBox.Controls.Add(mParticleBox);
            mGroupBox.Controls.Add(mMaxParticles);
            mGroupBox.Controls.Add(mTexture);
            mGroupBox.Controls.Add(mTextureSelector);
            mGroupBox.Controls.Add(mPPS);

            mParticleBox.Controls.Add(mLbLifetime);
            mParticleBox.Controls.Add(mLbSSize);
            mParticleBox.Controls.Add(mLbESize);
            mParticleBox.Controls.Add(mLbSAlpha);
            mParticleBox.Controls.Add(mLbEAlpha);
            mParticleBox.Controls.Add(mLbRadial);
            mParticleBox.Controls.Add(mLbPosition);
            mParticleBox.Controls.Add(mLbVelocity);
            mParticleBox.Controls.Add(mLbSColor);
            mParticleBox.Controls.Add(mLbEColor);
            mParticleBox.Controls.Add(mLifetime);
            mParticleBox.Controls.Add(mSSize);
            mParticleBox.Controls.Add(mESize);
            mParticleBox.Controls.Add(mSAlpha);
            mParticleBox.Controls.Add(mEAlpha);
            mParticleBox.Controls.Add(mRadPanel);
            mParticleBox.Controls.Add(mPosPanel);
            mParticleBox.Controls.Add(mVelPanel);
            mParticleBox.Controls.Add(mSColor);
            mParticleBox.Controls.Add(mEColor);
            #endregion

            #region Component Setup

            int ParticleWidth = (mParticleBox.Size - mParticleBox.Padding.Size - mParticleBox.Margin.Size).Width;
            int ContentWidth = (mGroupBox.Size - mGroupBox.Padding.Size - mGroupBox.Margin.Size).Width;

            SetupTransformPanel(mRadPanel, 75, 120, ParticleWidth, mLbRX, mLbRY, mLbRZ, mRX, mRY, mRZ);
            SetupTransformPanel(mPosPanel, 75, 150, ParticleWidth, mLbPX, mLbPY, mLbPZ, mPX, mPY, mPZ);
            SetupTransformPanel(mVelPanel, 75, 180, ParticleWidth, mLbVX, mLbVY, mLbVZ, mVX, mVY, mVZ);

            mLbMaxParticles.AutoSize = true;
            mLbMaxParticles.Location = new System.Drawing.Point(6, 26);
            mLbMaxParticles.Text = "Max Particles";

            mLbTexture.AutoSize = true;
            mLbTexture.Location = new System.Drawing.Point(6, 52);
            mLbTexture.Text = "Texture";

            mLbPPS.AutoSize = true;
            mLbPPS.Location = new System.Drawing.Point(6, 86);
            mLbPPS.Text = "P/s";

            mLbLifetime.AutoSize = true;
            mLbLifetime.Location = new System.Drawing.Point(6, 26);
            mLbLifetime.Text = "Lifetime";

            mLbSSize.AutoSize = true;
            mLbSSize.Location = new System.Drawing.Point(6, 52);
            mLbSSize.Text = "Start Size";

            mLbESize.AutoSize = true;
            mLbESize.Location = new System.Drawing.Point(167, 52);
            mLbESize.Text = "End Size";

            mLbSAlpha.AutoSize = true;
            mLbSAlpha.Location = new System.Drawing.Point(6, 86);
            mLbSAlpha.Text = "Start Alpha";

            mLbEAlpha.AutoSize = true;
            mLbEAlpha.Location = new System.Drawing.Point(167, 86);
            mLbEAlpha.Text = "End Alpha";

            mLbRadial.AutoSize = true;
            mLbRadial.Location = new System.Drawing.Point(6, 125);
            mLbRadial.Text = "Radial";

            mLbPosition.AutoSize = true;
            mLbPosition.Location = new System.Drawing.Point(6, 158);
            mLbPosition.Text = "Position";

            mLbVelocity.AutoSize = true;
            mLbVelocity.Location = new System.Drawing.Point(6, 192);
            mLbVelocity.Text = "Velocity";

            mLbSColor.AutoSize = true;
            mLbSColor.Location = new System.Drawing.Point(6, 220);
            mLbSColor.Text = "Start Color";

            mLbEColor.AutoSize = true;
            mLbEColor.Location = new System.Drawing.Point(6, 247);
            mLbEColor.Text = "End Color";

            mMaxParticles.Location = new System.Drawing.Point(90, 21);
            mMaxParticles.Size = new System.Drawing.Size(ContentWidth - mMaxParticles.Left, 20);
            mMaxParticles.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right;

            mTexture.Location = new System.Drawing.Point(90, 50);
            mTexture.Size = new System.Drawing.Size(200, 24);
            mTextureSelector.Text = "...";
            mTextureSelector.Click += DoFileSelect;

            mTextureSelector.Location = new System.Drawing.Point(294, 49);
            mTextureSelector.Size = new System.Drawing.Size(26, 26);

            mPPS.Location = new System.Drawing.Point(90, 85);
            mPPS.Size = new System.Drawing.Size(ContentWidth - mPPS.Left, 20);
            mPPS.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right;

            mLifetime.Location = new System.Drawing.Point(74, 24);
            mLifetime.Size = new System.Drawing.Size(217, 20);

            mSSize.Location = new System.Drawing.Point(75, 55);
            mSSize.Size = new System.Drawing.Size(85, 20);

            mESize.Location = new System.Drawing.Point(217, 55);
            mESize.Size = new System.Drawing.Size(75, 20);

            mSAlpha.Location = new System.Drawing.Point(75, 86);
            mSAlpha.Size = new System.Drawing.Size(85, 20);

            mEAlpha.Location = new System.Drawing.Point(230, 86);
            mEAlpha.Size = new System.Drawing.Size(85, 20);

            mSColor.Location = new System.Drawing.Point(75, 215);
            mSColor.Size = new System.Drawing.Size(ContentWidth - mSColor.Left, 23);
            mSColor.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right;
            mSColor.Text = "Start Color";
            mSColor.Click += DoColorSelect;

            mEColor.Location = new System.Drawing.Point(75, 245);
            mEColor.Size = new System.Drawing.Size(ContentWidth - mEColor.Left, 23);
            mEColor.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right;
            mEColor.Text = "End Color";
            mEColor.Click += DoColorSelect;

            #endregion

            mParticleBox.Size = mParticleBox.PreferredSize;
            mParticleBox.Location = new System.Drawing.Point(10, 110);
            mParticleBox.Text = "Particle";
            mGroupBox.Text = "Paticle Emitter";
            mGroupBox.Size = mGroupBox.PreferredSize;
        }

        protected void DoColorSelect(object sender, EventArgs e)
        {
            ColorDialog d = new ColorDialog();
            if(d.ShowDialog() == DialogResult.OK)
            {
                ((Button)sender).BackColor = d.Color;
            }
        }

        protected void DoFileSelect(object sender, EventArgs e)
        {
            OpenFileDialog o = new OpenFileDialog();
            o.Filter = "Texture File (*.png)|*.png";
            o.FilterIndex = 1;
            o.Title = "Select a texture file for the particle emitter...";
            if(o.ShowDialog() == DialogResult.OK)
            {
                mTexture.Text = o.FileName;
            }
        }

        public override void WriteComponentData(XmlWriter _w)
        {
            _w.WriteStartElement("ParticleEmitter");

            _w.WriteElementString("MaxParticles", mMaxParticles.Value.ToString());
            _w.WriteElementString("Texture", mTexture.Text.Substring(mTexture.Text.LastIndexOf("\\") + 1));
            _w.WriteElementString("PPS", mPPS.Value.ToString());

            _w.WriteStartElement("Particle");

            _w.WriteElementString("Lifetime", mLifetime.Value.ToString());
            _w.WriteElementString("StartSize", mSSize.Value.ToString());
            _w.WriteElementString("EndSize", mESize.Value.ToString());
            _w.WriteElementString("StartAlpha", mSAlpha.Value.ToString());
            _w.WriteElementString("EndAlpha", mEAlpha.Value.ToString());
            _w.WriteElementString("Radial", mRX.Value.ToString() + "," + mRY.Value + "," + mRZ.Value);
            _w.WriteElementString("Position", mPX.Value + "," + mPY.Value + "," + mPZ.Value);
            _w.WriteElementString("Velocity", mVX.Value + "," + mVY.Value + "," + mVZ.Value);
            _w.WriteElementString("StartColor", (mSColor.BackColor.R / 255.0f) + "," + (mSColor.BackColor.G / 255.0f) + "," + (mSColor.BackColor.B / 255.0f));
            _w.WriteElementString("EndColor", (mEColor.BackColor.R / 255.0f) + "," + (mEColor.BackColor.G / 255.0f) + "," + (mEColor.BackColor.B / 255.0f));

            _w.WriteEndElement();

            _w.WriteEndElement();
        }

        public override void ReadComponentData(XmlReader _r)
        {
            /*
             * parts = reader.Value.Split(',');
             * mStartRot.X = float.Parse(parts[0]) * RADIANS_TO_DEGREES;
             * mStartRot.Y = float.Parse(parts[1]) * RADIANS_TO_DEGREES;
             * mStartRot.Z = float.Parse(parts[2]) * RADIANS_TO_DEGREES;
             */
            string[] parts = { };
            string element = string.Empty;
            while (_r.Read())
            {
                switch(_r.NodeType)
                {
                    case XmlNodeType.Text:
                        Debug.Print("Text for " + element);
                        switch(element)
                        {
                            case "MaxParticles":
                                mMaxParticles.Value = decimal.Parse(_r.Value);
                                break;
                            case "Texture":
                                mTexture.Text = _r.Value;
                                break;
                            case "PPS":
                                mPPS.Value = decimal.Parse(_r.Value);
                                break;
                            case "Lifetime":
                                mLifetime.Value = decimal.Parse(_r.Value);
                                break;
                            case "StartSize":
                                mSSize.Value = decimal.Parse(_r.Value);
                                break;
                            case "EndSize":
                                mESize.Value = decimal.Parse(_r.Value);
                                break;
                            case "StartAlpha":
                                mSAlpha.Value = decimal.Parse(_r.Value);
                                break;
                            case "EndAlpha":
                                mEAlpha.Value = decimal.Parse(_r.Value);
                                break;
                            case "Radial":
                                parts = _r.Value.Split(',');
                                mRX.Value = decimal.Parse(parts[0]);
                                mRY.Value = decimal.Parse(parts[1]);
                                mRZ.Value = decimal.Parse(parts[2]);
                                break;
                            case "Position":
                                parts = _r.Value.Split(',');
                                mPX.Value = decimal.Parse(parts[0]);
                                mPY.Value = decimal.Parse(parts[1]);
                                mPZ.Value = decimal.Parse(parts[2]);
                                break;
                            case "Velocity":
                                parts = _r.Value.Split(',');
                                mVX.Value = decimal.Parse(parts[0]);
                                mVY.Value = decimal.Parse(parts[1]);
                                mVZ.Value = decimal.Parse(parts[2]);
                                break;
                            case "StartColor":
                                parts = _r.Value.Split(',');
                                mSColor.BackColor = System.Drawing.Color.FromArgb(255, 
                                    (int)(decimal.Parse(parts[0]) * 255), 
                                    (int)(decimal.Parse(parts[1]) * 255),
                                    (int)(decimal.Parse(parts[2]) * 255));
                                break;
                            case "EndColor":
                                parts = _r.Value.Split(',');
                                mEColor.BackColor = System.Drawing.Color.FromArgb(255,
                                    (int)(decimal.Parse(parts[0]) * 255),
                                    (int)(decimal.Parse(parts[1]) * 255),
                                    (int)(decimal.Parse(parts[2]) * 255));
                                break;
                        }
                        break;

                    case XmlNodeType.Element:
                        element = _r.Name;
                        break;

                    case XmlNodeType.EndElement:
                        if(_r.Name == "ParticleEmitter")
                        {
                            // TODO: Make less jank
                            return;
                        }
                        break;
                }
            }
        }


    }
}
