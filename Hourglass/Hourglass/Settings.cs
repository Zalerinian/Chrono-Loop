namespace Hourglass
{
	public static class Settings
	{
		private static int mVersion = 1;
		private static string mProjDir = string.Empty;
		private static System.Drawing.Color mColor = System.Drawing.Color.Black;

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

		public static System.Drawing.Color BackgroundColor {
			get {
				return mColor;
			}
			set {
				mColor = value;
			}
		}

	}
}
