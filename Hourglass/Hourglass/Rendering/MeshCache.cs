using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;
using System.IO;

namespace Hourglass {
	
	struct MeshData {
		public IndexBuffer iBuffer;
		public VertexBuffer vBuffer;
		public int[] indices;
		public CustomVertex.PositionNormalTextured[] vertices;
	}

	class MeshCache {
		private static readonly MeshCache sInstance = new MeshCache();

		public static MeshCache Instance {
			get {
				return sInstance;
			}
		}

		private Dictionary<string, MeshData> mMeshes = new Dictionary<string, MeshData>();

		public MeshData GetMesh(string _path) {
			if(mMeshes.ContainsKey(_path)) {
				return mMeshes[_path];
			} else {
				MeshData data = Load(_path);
				mMeshes[_path] = data;
				return data;
			}
		}

		public MeshData ReloadMesh(string _path) {
			MeshData data = Load(_path);
			mMeshes[_path] = data;
			return data;
		}

		public MeshData Load(string _File) {
			List<Vector3> Verts = new List<Vector3>();
			List<Vector3> Norms = new List<Vector3>();
			List<Vector2> UVs = new List<Vector2>();
			List<CustomVertex.PositionNormalTextured> vertices = new List<CustomVertex.PositionNormalTextured>();
			List<int> Ind = new List<int>();
			StreamReader sr = new StreamReader(_File);
			string line = string.Empty;
			while ((line = sr.ReadLine()) != null) {
				string[] parts = line.Split(new char[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);

				if (parts.Length > 0) {
					Vector3 vec = new Vector3();
					switch (parts[0]) {
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
							uv.Y = 1 - (float)Convert.ToDouble(parts[2]);
							UVs.Add(uv);
							break;
						case "f":
							for (int i = 1; i < 4; i++) {
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
			MeshData objectData = new MeshData();
			objectData.vertices = vertices.ToArray();
			objectData.indices = Ind.ToArray();

			objectData.iBuffer = new IndexBuffer(typeof(int), sizeof(int) * Ind.Count, Renderer.Instance.Device, Usage.WriteOnly, Pool.Default);
			objectData.vBuffer = new VertexBuffer(typeof(CustomVertex.PositionNormalTextured),
												  vertices.Count,
												  Renderer.Instance.Device,
												  Usage.Dynamic | Usage.WriteOnly,
												  CustomVertex.PositionNormalTextured.Format,
												  Pool.Default);

			objectData.iBuffer.SetData(objectData.indices, 0, LockFlags.None);
			objectData.vBuffer.SetData(objectData.vertices, 0, LockFlags.None);
			return objectData;
		}

	}
}
