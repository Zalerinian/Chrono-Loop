using System.Drawing;
using Microsoft.DirectX;

namespace Hourglass
{
	public static class Settings
	{
		private static int mVersion = 1;
        private static uint mMaxCloneCount = 0;
		private static string mProjDir = string.Empty;
		private static Color mColor = Color.Black;
		private static Vector3 mLevelStartPosition = new Vector3();
		private static Vector3 mLevelStartRotation = new Vector3();

		public static int SettingsVersion {
			get {
				return mVersion;
			}
		}

		public static string ProjectPath {
			get {
				return mProjDir;
			}
			set {
				mProjDir = value;
			}
		}

		public static Color BackgroundColor {
			get {
				return mColor;
			}
			set {
				mColor = value;
			}
		}

		public static Vector3 StartPos {
			get {
				return mLevelStartPosition;
			}
			set {
				mLevelStartPosition = value;
			}
		}

		public static Vector3 StartRot {
			get {
				return mLevelStartRotation;
			}
			set {
				mLevelStartRotation = value;
			}
		}

        public static uint MaxClones
        {
            get { return mMaxCloneCount; }
            set { mMaxCloneCount = value; }
        }

	}
}
