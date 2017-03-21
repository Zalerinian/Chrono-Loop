using System;
using System.Collections.Generic;
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;
using System.IO;

namespace Hourglass
{
	public class TexturedShape : RenderShape
	{
		public enum TextureType { Diffuse = 0, Normal, Specular, Emissive };
		public BaseTexture[] mTextures = new BaseTexture[4];
		public CustomVertex.PositionNormalTextured[] mVertices;


		public bool Valid {
			get {
				return VertexBuffer != null && IndexBuffer != null;
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
			List<Vector3> Verts = new List<Vector3>();
			List<Vector3> Norms = new List<Vector3>();
			List<Vector2> UVs = new List<Vector2>();
			List<CustomVertex.PositionNormalTextured> vertices = new List<CustomVertex.PositionNormalTextured>();
			List<int> Ind = new List<int>();
			StreamReader sr = new StreamReader(_File);
			string line = string.Empty;
			while ((line = sr.ReadLine()) != null)
			{
				string[] parts = line.Split(new char[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);

				if (parts.Length > 0)
				{
					Vector3 vec = new Vector3();
					switch (parts[0])
					{
						case "o":
							//mName = parts[1];
							break;
						case "v":
							vec.X = (float)Convert.ToDouble(parts[1]);
							vec.Y = (float)Convert.ToDouble(parts[2]);
							vec.Z = (float)Convert.ToDouble(parts[3]);
							Verts.Add(vec);
							break;
						case "vn":
							vec.X = (float)Convert.ToDouble(parts[1]);
							vec.Y = (float)Convert.ToDouble(parts[2]);
							vec.Z = (float)Convert.ToDouble(parts[3]);
							Norms.Add(vec);
							break;
						case "vt":
							Vector2 uv = new Vector2();
							uv.X = (float)Convert.ToDouble(parts[1]);
							uv.Y = (float)Convert.ToDouble(parts[2]);
							UVs.Add(uv);
							break;
						case "f":
							for (int i = 1; i < 4; i++)
							{
								string[] points = parts[i].Split(new char[] { '/' }, StringSplitOptions.None);
								Ind.Add(vertices.Count);
								vertices.Add(new CustomVertex.PositionNormalTextured(Verts[Convert.ToInt32(points[0]) - 1],
									Norms[Convert.ToInt32(points[2]) - 1],
									UVs[Convert.ToInt32(points[1]) - 1].X,
									UVs[Convert.ToInt32(points[1]) - 1].Y));
							}
							break;
						default:
							break;
					}
				}
			}
			mVertices = vertices.ToArray();
			Indices = Ind.ToArray();
		}

		public void SetMesh(string file)
		{
			mVertices = null;
			mIndices = null;
			Load(file);
			FillBuffers();
		}

		public void SetTexture(TextureType location, BaseTexture t)
		{
			mTextures[(int)location] = t;
		}

		public void FillBuffers()
		{
			if(mIndexBuffer != null)
			{
				mIndexBuffer.Dispose();
			}
			mIndexBuffer = new IndexBuffer(typeof(int),
				mIndices.Length * sizeof(int),
				Renderer.Instance.Device,
				Usage.WriteOnly,
				Pool.Default
			);

			if (mVertexBuffer != null)
			{
				mVertexBuffer.Dispose();
			}
			mVertexBuffer = new VertexBuffer(typeof(CustomVertex.PositionNormalTextured),
				mVertices.Length,
				Renderer.Instance.Device,
				Usage.Dynamic | Usage.WriteOnly,
				CustomVertex.PositionNormalTextured.Format,
				Pool.Default
			);

			mIndexBuffer.SetData(mIndices, 0, LockFlags.None);
			mVertexBuffer.SetData(mVertices, 0, LockFlags.None);
		}
	}
}
