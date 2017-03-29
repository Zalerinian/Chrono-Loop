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

			GizmoTag X = new GizmoTag(new Vector3(1, 0, 0), System.Drawing.Color.Red);
			GizmoTag Y = new GizmoTag(new Vector3(0, 1, 0), System.Drawing.Color.Green);
			GizmoTag Z = new GizmoTag(new Vector3(0, 0, 1), System.Drawing.Color.Blue);
			mTranslateX.Tag = X;
			mRotateX.Tag = X;
			mScaleX.Tag = X;

			mTranslateY.Tag = Y;
			mRotateY.Tag = Y;
			mScaleY.Tag = Y;

			mTranslateZ.Tag = Z;
			mRotateZ.Tag = Z;
			mScaleZ.Tag = Z;


			Reposition();
		}

		public enum GizmoMode { Position = 1, Rotation = 2, Scale = 4 };

		private ColoredShape mTranslateX, mTranslateY, mTranslateZ;
		private ColoredShape mRotateX, mRotateY, mRotateZ;
		private ColoredShape mScaleX, mScaleY, mScaleZ;
		private IGizmoAttachment mAttached = null;
		private GizmoMode mMode = GizmoMode.Position;
		private ColoredShape mGrabbed = null;


		private struct GizmoTag
		{
			public Vector3 Axis;
			public System.Drawing.Color Color;
			public GizmoTag(Vector3 _axis, System.Drawing.Color _color)
			{
				Axis = _axis;
				Color = _color;
			}
		}

		public ColoredShape Grabbed {
			get {
				if(!Valid)
				{
					return null;
				}
				else
				{
					return mGrabbed;
				}
			}
			set {
				mGrabbed = value;
			}
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

		public Matrix Position {
			get {
				if(Valid)
				{
					return mAttached.GizmoWorld;
				}
				else
				{
					return Matrix.Identity;
				}
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
			Reposition();
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

		public bool Apply(Vector3 _movement)
		{
			if(Grabbed == null)
			{
				return false;
			}
			switch(mMode)
			{
				case GizmoMode.Position:
					ApplyPosition(_movement);
					break;
				case GizmoMode.Rotation:
					ApplyRotation(_movement);
					break;
				case GizmoMode.Scale:
					ApplyScale(_movement);
					break;
			}

			return true;
		}

		private void ApplyPosition(Vector3 _movement)
		{
			Vector3 axis = ((GizmoTag)Grabbed.Tag).Axis;
			Vector3 raxis = Renderer.Instance.RotateInto(axis, Renderer.Instance.View);
			if(axis.X > 0)
			{
				mAttached.PX.Value -= (decimal)Vector3.Dot(raxis, _movement);
			}
			else if(axis.Y > 0)
			{
				mAttached.PY.Value -= (decimal)Vector3.Dot(raxis, _movement);

			}
			else if (axis.Z > 0)
			{
				mAttached.PZ.Value += (decimal)Vector3.Dot(raxis, _movement);

			}
		}

		private void ApplyRotation(Vector3 _movement)
		{

		}

		private void ApplyScale(Vector3 _movement)
		{

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
			dev.Clear(ClearFlags.ZBuffer, 0, 1.0f, 0);


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
