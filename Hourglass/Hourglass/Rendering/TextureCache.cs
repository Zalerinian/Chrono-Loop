using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;

namespace Hourglass {
	class TextureCache {
		private static readonly TextureCache sInstance = new TextureCache();

		public static TextureCache Instance {
			get {
				return sInstance;
			}
		}

		private Dictionary<string, BaseTexture> mTextures = new Dictionary<string, BaseTexture>();

		public BaseTexture GetTexture(string _path) {
			if(mTextures.ContainsKey(_path)) {
				return mTextures[_path];
			} else {
				mTextures[_path] = Renderer.Instance.LoadTexture(_path);
				return mTextures[_path];
			}
		}

	}
}
