using System;
using System.Collections.Generic;
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;
using System.Drawing;
using System.IO;

namespace Hourglass
{
	public class ColoredShape : RenderShape
	{
		protected Color mColor = Color.White;
		protected CustomVertex.PositionNormalColored[] mVertices;

		public Color Color {
			get {
				return mColor;
			}
			set {
				mColor = value;
				for(int i = 0; i < mVertices.Length; ++i)
				{
					mVertices[i].Color = value.ToArgb();
				}
				FillBuffers();
			}
		}

		public CustomVertex.PositionNormalColored[] Vertices {
			get {
				return mVertices;
			}
			set {
				mVertices = value;
			}
		}


		public ColoredShape()
		{
			mType = ShapeType.Colored;
			World = Matrix.Identity;
		}

		public ColoredShape(string file)
		{
			mType = ShapeType.Colored;
			World = Matrix.Identity;
			SetMesh(file);
		}

		public ColoredShape(string file, Color color)
		{
			mType = ShapeType.Colored;
			World = Matrix.Identity;
			SetMesh(file, color);
		}

		public void Load(string _File)
		{
			List<Vector3> Verts = new List<Vector3>();
			List<Vector3> Norms = new List<Vector3>();
			List<Vector2> UVs = new List<Vector2>();
			List<CustomVertex.PositionNormalColored> vertices = new List<CustomVertex.PositionNormalColored>();
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
								vertices.Add(new CustomVertex.PositionNormalColored(Verts[Convert.ToInt32(points[0]) - 1],
									Norms[Convert.ToInt32(points[2]) - 1],
									Color.SeaGreen.ToArgb()));
							}
							break;
						default:
							break;
					}
				}
			}
			Vertices = vertices.ToArray();
			Indices = Ind.ToArray();
		}

		public void Load(string _File, Color _c)
		{
			List<Vector3> Verts = new List<Vector3>();
			List<Vector3> Norms = new List<Vector3>();
			List<Vector2> UVs = new List<Vector2>();
			List<CustomVertex.PositionNormalColored> vertices = new List<CustomVertex.PositionNormalColored>();
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
								vertices.Add(new CustomVertex.PositionNormalColored(Verts[Convert.ToInt32(points[0]) - 1],
									Norms[Convert.ToInt32(points[2]) - 1],
									_c.ToArgb()));
							}
							break;
						default:
							break;
					}
				}
			}
			Vertices = vertices.ToArray();
			Indices = Ind.ToArray();
		}

		public void SetMesh(string file)
		{
			Vertices = null;
			Indices = null;
			Load(file);
			FillBuffers();
		}

		public void SetMesh(string file, Color _c)
		{
			Vertices = null;
			Indices = null;
			Load(file, _c);
			FillBuffers();
		}

		public override void FillBuffers()
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
			mVertexBuffer = new VertexBuffer(typeof(CustomVertex.PositionNormalColored),
				mVertices.Length,
				Renderer.Instance.Device,
				Usage.Dynamic | Usage.WriteOnly,
				CustomVertex.PositionNormalColored.Format,
				Pool.Default
			);

			mIndexBuffer.SetData(mIndices, 0, LockFlags.None);
			mVertexBuffer.SetData(mVertices, 0, LockFlags.None);
		}

		public void MakeGrid()
		{
			List<CustomVertex.PositionNormalColored> verts = new List<CustomVertex.PositionNormalColored>();
			List<int> ind = new List<int>();
			for (int i = 0; i <= 100; i++)
			{
				ind.Add(verts.Count);
				verts.Add(new CustomVertex.PositionNormalColored(new Vector3((float)i - 50, 0, -50), new Vector3(), 0));
				ind.Add(verts.Count);
				verts.Add(new CustomVertex.PositionNormalColored(new Vector3((float)i - 50, 0, 50), new Vector3(), 0));
				ind.Add(verts.Count);
				verts.Add(new CustomVertex.PositionNormalColored(new Vector3((float)i - 50, 0, -50), new Vector3(), 0));
			}
			for (int i = 0; i <= 100; i++)
			{
				ind.Add(verts.Count);
				verts.Add(new CustomVertex.PositionNormalColored(new Vector3(-50, 0, (float)i - 50), new Vector3(), 0));
				ind.Add(verts.Count);
				verts.Add(new CustomVertex.PositionNormalColored(new Vector3(50, 0, (float)i - 50), new Vector3(), 0));
				ind.Add(verts.Count);
				verts.Add(new CustomVertex.PositionNormalColored(new Vector3(-50, 0, (float)i - 50), new Vector3(), 0));
			}
			mVertices = verts.ToArray();
			mIndices = ind.ToArray();
			FillBuffers();
		}

		public override void Dispose()
		{
			if (mIndexBuffer != null)
			{
				mIndexBuffer.Dispose();
				mIndexBuffer = null;
			}
			if (mVertexBuffer != null)
			{
				mVertexBuffer.Dispose();
				mVertexBuffer = null;
			}
		}
	}
}
