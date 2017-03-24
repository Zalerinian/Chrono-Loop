using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.DirectX.Direct3D;

namespace Hourglass
{
	public class VertexBufferManager
	{
		private static readonly VertexBufferManager sInstance = new VertexBufferManager();
		public static VertexBufferManager Instance {
			get {
				return sInstance;
			}
		}


		private Dictionary<string, KeyValuePair<int[], CustomVertex.PositionNormalTextured[]>> mTexturedMeshes;
		private Dictionary<string, KeyValuePair<int[], CustomVertex.PositionNormalColored[]>> mColoredMeshes;

		//public GetTexturedMesh(string file)
		//{
		//
		//}
	}
}
