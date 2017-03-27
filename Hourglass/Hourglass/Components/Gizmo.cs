using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;

namespace Hourglass
{
	public class Gizmo
	{
		private static readonly Gizmo mGizmo = new Gizmo();
		public static Gizmo Instance {
			get {
				return mGizmo;
			}
		}

		private Gizmo()
		{
			mTranslateX = new ColoredShape("Assets\\Gizmo\\GizmoTranslate.obj", System.Drawing.Color.Red);
			mTranslateY = new ColoredShape("Assets\\Gizmo\\GizmoTranslate.obj", System.Drawing.Color.Green);
			mTranslateZ = new ColoredShape("Assets\\Gizmo\\GizmoTranslate.obj", System.Drawing.Color.Blue);

			mRotateX = new ColoredShape("Assets\\Gizmo\\GizmoRotate.obj", System.Drawing.Color.Red);
			mRotateY = new ColoredShape("Assets\\Gizmo\\GizmoRotate.obj", System.Drawing.Color.Green);
			mRotateZ = new ColoredShape("Assets\\Gizmo\\GizmoRotate.obj", System.Drawing.Color.Blue);

			mScaleX = new ColoredShape("Assets\\Gizmo\\GizmoScale.obj", System.Drawing.Color.Red);
			mScaleY = new ColoredShape("Assets\\Gizmo\\GizmoScale.obj", System.Drawing.Color.Green);
			mScaleZ = new ColoredShape("Assets\\Gizmo\\GizmoScale.obj", System.Drawing.Color.Blue);


			Reposition();
		}

		public enum GizmoMode { Position = 1, Rotation = 2, Scale = 4 };

		private ColoredShape mTranslateX, mTranslateY, mTranslateZ;
		private ColoredShape mRotateX, mRotateY, mRotateZ;
		private ColoredShape mScaleX, mScaleY, mScaleZ;
		private IGizmoAttachment mAttached = null;
		private GizmoMode mMode = GizmoMode.Position;
		private bool mGrabbed = false;

		public bool Grabbed {
			get {
				return mGrabbed;
			}
			set {
				mGrabbed = value;
			}
		}

		public ColoredShape[] GetVisibleComponents()
		{
			int count = 0, index = 0;
			count += ((mMode & GizmoMode.Position) != 0) ? 3 : 0;
			count += ((mMode & GizmoMode.Rotation) != 0) ? 3 : 0;
			count += ((mMode & GizmoMode.Scale) != 0) ? 3 : 0;
			ColoredShape[] ary = new ColoredShape[count];
			if((mMode & GizmoMode.Position) > 0)
			{
				ary[index++] = mTranslateX;
				ary[index++] = mTranslateY;
				ary[index++] = mTranslateZ;
			}
			if((mMode & GizmoMode.Rotation) > 0)
			{
				ary[index++] = mRotateX;
				ary[index++] = mRotateY;
				ary[index++] = mRotateZ;
			}
			if ((mMode & GizmoMode.Scale) > 0)
			{
				ary[index++] = mScaleX;
				ary[index++] = mScaleY;
				ary[index++] = mScaleZ;
			}
			return ary;
		}

		public bool Valid {
			get {
				return mAttached != null;
			}
		}

		public GizmoMode Mode {
			get {
				return mMode;
			}
			set {
				mMode = value;
			}
		}

		public bool CanPosition()
		{
			return mAttached != null &&
				mAttached.PX != null &&
				mAttached.PY != null &&
				mAttached.PZ != null;
		}

		public bool CanRotate()
		{
			return mAttached != null &&
				mAttached.RX != null &&
				mAttached.RY != null &&
				mAttached.RZ != null;
		}

		public bool CanScale()
		{
			return mAttached != null &&
				mAttached.SX != null &&
				mAttached.SY != null &&
				mAttached.SZ != null;
		}

		public void Attach(IGizmoAttachment _attachable)
		{
			mAttached = _attachable;
		}

		public void Reposition()
		{
			Matrix position = (mAttached == null) ? Matrix.Identity : mAttached.GizmoWorld;
			Matrix posX = Matrix.RotationY((float)(Math.PI / 2)) * position;
			Matrix posY = Matrix.RotationX(-(float)(Math.PI / 2)) * position;
			Matrix posZ = position;

			
			mTranslateX.World = posX;
			mRotateX.World = posX;
			mScaleX.World = posX;
			
			mTranslateY.World = posY;
			mRotateY.World = posY;
			mScaleY.World = posY;
			
			mTranslateZ.World = posZ;
			mRotateZ.World = posZ;
			mScaleZ.World = posZ;
		}

		public void Render()
		{
			if(mAttached == null)
			{
				// The gizmo won't render if it's not attached to anything.
				return;
			}
			Device dev = Renderer.Instance.Device;
			dev.VertexFormat = CustomVertex.PositionNormalColored.Format;
			dev.SetTexture(0, null);
			dev.RenderState.AlphaBlendEnable = false;
			dev.RenderState.CullMode = Cull.None;

			ColoredShape[] components = GetVisibleComponents();
			for(int i = 0; i < components.Length; ++i)
			{
				dev.Indices = components[i].IndexBuffer;
				dev.SetStreamSource(0, components[i].VertexBuffer, 0);
				dev.RenderState.FillMode = components[i].FillMode;
				dev.Transform.World = components[i].World;
				dev.DrawIndexedPrimitives(PrimitiveType.TriangleList, 0, 0, components[i].Indices.Length, 0, components[i].Indices.Length / 3);
			}
		}
	}
}
