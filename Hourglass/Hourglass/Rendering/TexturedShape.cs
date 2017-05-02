using System;
using System.Collections.Generic;
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;
using System.IO;

namespace Hourglass
{
	public class TexturedShape : RenderShape
	{
		public enum TextureType { Diffuse = 0, Normal, Specular, Emissive, Max };
		public BaseTexture[] mTextures = new BaseTexture[4];
		public CustomVertex.PositionNormalTextured[] mVertices;
		public string mResourceName = "";
		public string[] mTexturePaths = new string[4];

		public override IndexBuffer IndexBuffer {
			get {
				return MeshCache.Instance.GetMesh(mResourceName).iBuffer;
			}
		}

		public override VertexBuffer VertexBuffer {
			get {
				return MeshCache.Instance.GetMesh(mResourceName).vBuffer;
			}
		}

		public override bool Valid {
			get {
				if (!string.IsNullOrWhiteSpace(mResourceName)) {
					MeshData m = MeshCache.Instance.GetMesh(mResourceName);
					return m.iBuffer != null && m.vBuffer != null;
				} else {
					return false;
				}
			}
		}

		public BaseTexture[] Textures {
			get {
				return mTextures;
			}
			set {
				mTextures = value;
			}
		}

		public TexturedShape()
		{
			World = Matrix.Identity;
			mType = ShapeType.Textured;
		}

		public TexturedShape(string file)
		{
			World = Matrix.Identity;
			SetMesh(file);
			mType = ShapeType.Textured;
		}

		public void Load(string _File)
		{
			mResourceName = _File;
			MeshData m = MeshCache.Instance.GetMesh(_File);
			Indices = m.indices;
			mVertices = m.vertices;
		}

		public void SetMesh(string file)
		{
			mResourceName = file;
			MeshData m = MeshCache.Instance.GetMesh(file); // Ensure the file is loaded.
			Indices = m.indices;
			mVertices = m.vertices;
		}

		public void SetTexture(TextureType location, BaseTexture t)
		{
			if(mTextures[(int)location] != null)
			{
				mTextures[(int)location].Dispose();
			}
			mTextures[(int)location] = t;
		}

		public void SetTexture(TextureType location, string file)
		{
			if (mTextures[(int)location] != null)
			{
				mTextures[(int)location].Dispose();
			}
			SetTexture(location, TextureCache.Instance.GetTexture(file));
		}

		public override void FillBuffers()
		{
			if(!string.IsNullOrWhiteSpace(mResourceName)) {
				MeshData m = MeshCache.Instance.ReloadMesh(mResourceName);
				Indices = m.indices;
				mVertices = m.vertices;
			}
		}

		public override void Dispose()
		{
			if(mIndexBuffer != null)
			{
				mIndexBuffer.Dispose();
				mIndexBuffer = null;
			}
			if (mVertexBuffer != null)
			{
				mVertexBuffer.Dispose();
				mVertexBuffer = null;
			}
			// Textures are stored in the texture cache now.
			//for(TextureType t = TextureType.Diffuse; t < TextureType.Max; ++t)
			//{
			//	if(mTextures[(int)t] != null)
			//	{
			//		mTextures[(int)t].Dispose();
			//	}
			//}
		}

		public override bool CheckRaycast(Vector3 _start, Vector3 _dir, out float _time)
		{
			for (int TriIndex = 0; TriIndex < Indices.Length; TriIndex += 3)
			{
				Vector3 norm = mVertices[Indices[TriIndex + 0]].Normal + mVertices[Indices[TriIndex + 1]].Normal + mVertices[Indices[TriIndex + 2]].Normal;
				norm.Multiply(1.0f / 3.0f);
				float t = 0;
				if (RayToTriangle(mVertices[Indices[TriIndex + 0]].Position, mVertices[Indices[TriIndex + 1]].Position, mVertices[Indices[TriIndex + 2]].Position, norm, _start, _dir, out t))
				{
					_time = t;
					return true;
				}
			}
			_time = 0;
			return false;
		}




	}
}
