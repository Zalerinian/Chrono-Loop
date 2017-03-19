using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace Hourglass {

    public class ColliderComponent : Component {

        protected NumericUpDown mMass, mStaticFriction, mElasticity, mKineticFriction, mDrag;
        protected Label mLbMass, mLbStaticFriction, mLbElasticity, mLbKineticFriction, mLbDrag;

        public ColliderComponent(List<Component> _container, int _yOffset = 0) : base(_container) {
            #region Control Creation
            
            // Numeric Up-Down controls:
            mMass            = new NumericUpDown();
            mStaticFriction  = new NumericUpDown();
            mElasticity      = new NumericUpDown();
            mKineticFriction = new NumericUpDown();
            mDrag            = new NumericUpDown();

            // Labels
            mLbMass            = new Label();
            mLbStaticFriction  = new Label();
            mLbElasticity      = new Label();
            mLbKineticFriction = new Label();
            mLbDrag            = new Label();

            mGroupBox.Controls.Add(mMass);
            mGroupBox.Controls.Add(mStaticFriction);
            mGroupBox.Controls.Add(mElasticity);
            mGroupBox.Controls.Add(mKineticFriction);
            mGroupBox.Controls.Add(mDrag);

            mGroupBox.Controls.Add(mLbMass);
            mGroupBox.Controls.Add(mLbStaticFriction);
            mGroupBox.Controls.Add(mLbElasticity);
            mGroupBox.Controls.Add(mLbKineticFriction);
            mGroupBox.Controls.Add(mLbDrag);

            #endregion

            #region Control Setup

            int ContentWidth = (mGroupBox.Size - mGroupBox.Padding.Size - mGroupBox.Margin.Size).Width;

            // Labels
            mLbMass.AutoSize = true;
            mLbMass.Location = new System.Drawing.Point(6, 22 + _yOffset);
            mLbMass.Name = "mLbMass";
            mLbMass.Size = new System.Drawing.Size(38, 13);
            mLbMass.TabIndex = 0;
            mLbMass.Text = "Mass";

            mLbElasticity.AutoSize = true;
            mLbElasticity.Location = new System.Drawing.Point(6, 52 + _yOffset);
            mLbElasticity.Name = "mLbElasticity";
            mLbElasticity.Size = new System.Drawing.Size(54, 13);
            mLbElasticity.TabIndex = 1;
            mLbElasticity.Text = "Elasticity";

            mLbDrag.AutoSize = true;
            mLbDrag.Location = new System.Drawing.Point(6, 79 + _yOffset);
            mLbDrag.Name = "mLbDrag";
            mLbDrag.Size = new System.Drawing.Size(36, 13);
            mLbDrag.TabIndex = 2;
            mLbDrag.Text = "Drag";

            mLbStaticFriction.AutoSize = true;
            mLbStaticFriction.Location = new System.Drawing.Point(6, 105 + _yOffset);
            mLbStaticFriction.Name = "mLbStaticFriction";
            mLbStaticFriction.Size = new System.Drawing.Size(77, 13);
            mLbStaticFriction.TabIndex = 3;
            mLbStaticFriction.Text = "Static Friction";

            mLbKineticFriction.AutoSize = true;
            mLbKineticFriction.Location = new System.Drawing.Point(6, 132 + _yOffset);
            mLbKineticFriction.Name = "mLbKineticFriction";
            mLbKineticFriction.Size = new System.Drawing.Size(82, 13);
            mLbKineticFriction.TabIndex = 4;
            mLbKineticFriction.Text = "Kinetic Friction";


            // Numeric Up-Downs
            mElasticity.Anchor = (AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right);
            mElasticity.Location = new System.Drawing.Point(90, 49 + _yOffset);
            mElasticity.Name = "mElasticity";
            mElasticity.Size = new System.Drawing.Size(ContentWidth - mElasticity.Left, 20);
            mElasticity.TabIndex = 6;

            mDrag.Anchor = (AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right);
            mDrag.Location = new System.Drawing.Point(90, 77 + _yOffset);
            mDrag.Name = "mDrag";
            mDrag.Size = new System.Drawing.Size(ContentWidth - mDrag.Left, 20);
            mDrag.TabIndex = 7;

            mMass.Anchor = (AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right);
            mMass.Location = new System.Drawing.Point(90, 20 + _yOffset);
            mMass.Name = "mMass";
            mMass.Size = new System.Drawing.Size(ContentWidth - mMass.Left, 20);
            mMass.TabIndex = 5;

            mStaticFriction.Anchor = (AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right);
            mStaticFriction.Location = new System.Drawing.Point(90, 103 + _yOffset);
            mStaticFriction.Name = "mStaticFriction";
            mStaticFriction.Size = new System.Drawing.Size(ContentWidth - mStaticFriction.Left, 20);
            mStaticFriction.TabIndex = 8;

            mKineticFriction.Anchor = (AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right);
            mKineticFriction.Location = new System.Drawing.Point(90, 130 + _yOffset);
            mKineticFriction.Name = "mKineticFriction";
            mKineticFriction.Size = new System.Drawing.Size(ContentWidth - mKineticFriction.Left, 20);
            mKineticFriction.TabIndex = 9;

            #endregion

            mGroupBox.Text = "Collider Component";
            mGroupBox.Size = mGroupBox.PreferredSize;
        }

        protected override void OnMenuClick_Reset(object sender, EventArgs e) {
            mMass.Value = 0;
            mElasticity.Value = 0;
            mDrag.Value = 0;
            mStaticFriction.Value = 0;
            mKineticFriction.Value = 0;
        }

    }

}

