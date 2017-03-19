using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;
using System.Drawing;

namespace Hourglass
{
    class Changes
    {
        private ToolObject mSelected, mCreatedObject, mDestroyedObject;
        private ToolObjectColor mSelectedCollider, mCreatedCollider, mDestroyedCollider;
        private Vector3 mPosition, mRotation, mScale, mExtra;
        private bool mMove, mVisible, mTrigger;
        private Color mColor;
        private float mMass, mStaticF, mElasticity, mKeneticF, mDrag;

        #region Properties
        public ToolObject Selected
        {
            get { return mSelected; }
            set { mSelected = value; }
        }

        public ToolObjectColor SelectedCollider
        {
            get { return mSelectedCollider; }
            set { mSelectedCollider = value; }
        }

        public Vector3 Position
        {
            get { return mPosition; }
            set { mPosition = value; }
        }

        public Vector3 Rotation
        {
            get { return mRotation; }
            set { mRotation = value; }
        }

        public Vector3 Scale
        {
            get { return mScale; }
            set { mScale = value; }
        }

        public Vector3 Extra
        {
            get { return mExtra; }
            set { mExtra = value; }
        }

        public bool Move
        {
            get { return mMove; }
            set { mMove = value; }
        }

        public bool Visible
        {
            get { return mVisible; }
            set { mVisible = value; }
        }

        public bool Trigger
        {
            get { return mTrigger; }
            set { mTrigger = value; }
        }

        public Color Color
        {
            get { return mColor; }
            set { mColor = value; }
        }

        public float Mass
        {
            get { return mMass; }
            set { mMass = value; }
        }

        public float StaticF
        {
            get { return mStaticF; }
            set { mStaticF = value; }
        }

        public float Elasticity
        {
            get { return mElasticity; }
            set { mElasticity = value; }
        }

        public float KeneticF
        {
            get { return mKeneticF; }
            set { mKeneticF = value; }
        }

        public float Drag
        {
            get { return mDrag; }
            set { mDrag = value; }
        }

        public ToolObject CreatedObject
        {
            get { return mCreatedObject; }
            set { mCreatedObject = value; }
        }

        public ToolObject DestroyedObject
        {
            get { return mDestroyedObject; }
            set { mDestroyedObject = value; }
        }

        public ToolObjectColor CreatedCollider
        {
            get { return mCreatedCollider; }
            set { mCreatedCollider = value; }
        }

        public ToolObjectColor DestroyedCollider
        {
            get { return mDestroyedCollider; }
            set { mDestroyedCollider = value; }
        }
        #endregion

        public Changes()
        {
            mSelected = mCreatedObject = mDestroyedObject = null;
            mSelectedCollider = mCreatedCollider = mDestroyedCollider = null;
            mPosition = mRotation = mScale = mExtra = Vector3.Empty;
            mMove = mTrigger = false;
            mVisible = true;
            mColor = Color.Red;
            mMass = mStaticF = mElasticity = mKeneticF = mDrag = 0;
        }
    }
}
