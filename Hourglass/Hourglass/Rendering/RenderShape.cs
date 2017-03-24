using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;

namespace Hourglass
{
	public abstract class RenderShape
	{
		protected FillMode mFillMode = FillMode.Solid;
		protected Matrix mWorld;
		protected IndexBuffer mIndexBuffer;
		protected VertexBuffer mVertexBuffer;
		protected int[] mIndices;
		public enum ShapeType { Textured = 0, Colored };
		protected ShapeType mType;

		public bool Valid {
			get {
				return mVertexBuffer != null && mIndexBuffer != null;
			}
		}

		public ShapeType Type {
			get {
				return mType;
			}
		}


		public FillMode FillMode {
			get {
				return mFillMode;
			}
			set {
				mFillMode = value;
			}
		}

		public Matrix World {
			get {
				return mWorld;
			}
			set {
				mWorld = value;
			}
		}

		public IndexBuffer IndexBuffer {
			get {
				return mIndexBuffer;
			}
			set {
				mIndexBuffer = value;
			}
		}

		public VertexBuffer VertexBuffer {
			get {
				return mVertexBuffer;
			}
			set {
				mVertexBuffer = value;
			}
		}

		public int[] Indices {
			get {
				return mIndices;
			}
			set {
				mIndices = value;
			}
		}

		public abstract void FillBuffers();
		public abstract void Dispose();
	}
}
