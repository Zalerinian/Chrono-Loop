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
		private object mTag;
        protected Vector3 mPosition, mRotation, mScale;

        public Vector3 Position {
            get {
                return mPosition;
            }
            set {
                mPosition = value;
                RebuildMatrix();
            }
        }

        public Vector3 Rotation {
            get {
                return mRotation;
            }
            set {
                mRotation = value;
                RebuildMatrix();
            }
        }

        public Vector3 Scale {
            get {
                return mScale;
            }
            set {
                mScale = value;
                RebuildMatrix();
            }
        }

		public object Tag {
			get {
				return mTag;
			}
			set {
				mTag = value;
			}
		}

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
		public abstract bool CheckRaycast(Vector3 _start, Vector3 _dir, out float _time);

        private void RebuildMatrix() {
            mWorld = Matrix.Identity;
            mWorld.RotateX(mRotation.X);
            mWorld.RotateZ(mRotation.Z);
            mWorld.RotateY(mRotation.Y);
            mWorld.Translate(mPosition);
            mWorld.Scale(mScale);
        }

		public bool RayToTriangle(Vector3 _v1, Vector3 _v2, Vector3 _v3, Vector3 _norm, Vector3 _start, Vector3 _dir, out float _time)
		{
			Vector3 Centroid = (_v1 + _v2 + _v3);
			Centroid.Multiply(1.0f / 3.0f);
			Centroid -= _start;

			// These early-outs are to ensure the triangle is facing toward the ray's start, but we want
			// to raycast to objects no matter where they're facing for the editor (for things such as planes, with only one side).
			//if (Vector3.Dot(_norm, _dir) > 0 || Vector3.Dot(Centroid, _norm) > 0)
			//{
			//	_time = 0;
			//	return false;
			//}

			Vector3 sa = _v1 - _start,
				sb = _v2 - _start,
				sc = _v3 - _start,
				N1 = Vector3.Cross(sc, sb),
				N2 = Vector3.Cross(sa, sc),
				N3 = Vector3.Cross(sb, sa);
			float dn1 = Vector3.Dot(_dir, N1),
				dn2 = Vector3.Dot(_dir, N2),
				dn3 = Vector3.Dot(_dir, N3);

			if (dn1 == 0 && dn2 == 0 && dn3 == 0)
			{
				_time = 0;
				return true;
			}
			if (dn1.SameSign(dn2) && dn2.SameSign(dn3))
			{
				float offset = Vector3.Dot(_norm, _v1);
				_time = (offset - (Vector3.Dot(_start, _norm)) / Vector3.Dot(_norm, _dir));
				return true;
			}

			_time = 0;
			return false;
		}
	}
}
