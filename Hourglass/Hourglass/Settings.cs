namespace Hourglass
{
	public static class Settings
	{
		private static int mVersion = 1;
		private static string mProjDir = string.Empty;

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

	}
}
