using System;
using System.Collections.Generic;
using System.IO;
using System.Diagnostics;
using System.Threading.Tasks;

namespace Hourglass
{
	public class ResourceManager
	{
		private static readonly ResourceManager sInstance = new ResourceManager();
		public static ResourceManager Instance {
			get {
				return sInstance;
			}
		}


		// Instance Data
		public enum ResourceType { Mesh, Texture };

		public delegate void ResourceChangedHandler(FileSystemEventArgs e);
		public delegate void ResourceRenamedHandler(RenamedEventArgs e);
		public event ResourceChangedHandler ResourceAdded;
		public event ResourceChangedHandler ResourceUpdated;
		public event ResourceChangedHandler ResourceDeleted;
		public event ResourceRenamedHandler ResourceRenamed;
		private List<string> mObjects, mTextures, mCode;
		private List<FileSystemWatcher> mWatchers;

		public string ResourceDirectory {
			get {
				return "\\Resources\\";
			}
		}

        public string ActionDirectory {
            get {
                return "\\ChronoLoop\\Actions\\";
            }
        }

		public string[] ObjectFileFilters {
			get {
				string[] ary = { "*.obj" };
				return ary;
			}
		}


        public string[] CodeFileFilters {
            get {
                string[] ary = { "*.h", "*.hpp" };
                return ary;
            }
        }

        public string[] TextureFileFilters {
			get {
				string[] ary = { "*.png", "*.jpg", "*.jpeg", "*.bmp", "*.gif", "*.dds" };
				return ary;
			}
		}

		public List<string> Objects {
			get {
				return mObjects;
			}
		}

		public List<string> Textures {
			get {
				return mTextures;
			}
        }

        public List<string> Codes {
            get {
                return mCode;
            }
        }

        public string ResourcePrefix {
			get {
				return ".." + ResourceDirectory;
			}
		}

		private ResourceManager()
		{
			mObjects = new List<string>();
			mTextures = new List<string>();
            mCode = new List<string>();
			mWatchers = new List<FileSystemWatcher>();
		}

		private void InitializeWatchers()
		{
			// Prepare object listeners
			for (int i = 0; i < ObjectFileFilters.Length; ++i)
			{
				FileSystemWatcher w = new FileSystemWatcher(Settings.ProjectPath + ResourceDirectory, ObjectFileFilters[i]);
				w.IncludeSubdirectories = true;
				w.Changed += FileChanged;
				w.Renamed += FileRenamed;
				w.Deleted += FileDeleted;
				w.Created += FileCreated;
			}

			// Prepare texture listeners.
			for (int i = 0; i < ObjectFileFilters.Length; ++i)
			{
				FileSystemWatcher w = new FileSystemWatcher(Settings.ProjectPath + ResourceDirectory, ObjectFileFilters[i]);
				w.IncludeSubdirectories = true;
				w.Changed += FileChanged;
				w.Renamed += FileRenamed;
				w.Deleted += FileDeleted;
				w.Created += FileCreated;
			}
		}

		public void BuildAssetList()
		{
			if (string.IsNullOrWhiteSpace(Settings.ProjectPath))
			{
				Debug.Print("Well the project path is null, so there must not be any resources!");
				return;
			}

            for (int i = 0; i < CodeFileFilters.Length; i++)
            {
                IEnumerator<string> it = Directory.EnumerateFiles(Settings.ProjectPath + "\\ChronoLoop\\Actions\\", CodeFileFilters[i], SearchOption.AllDirectories).GetEnumerator();
                while (it.MoveNext())
                {
                    mCode.Add(it.Current.Substring(Settings.ProjectPath.Length + "\\ChronoLoop\\Actions\\".Length));
                }
            }

			for (int i = 0; i < ObjectFileFilters.Length; ++i)
			{

				IEnumerator<string> it = Directory.EnumerateFiles(Settings.ProjectPath + ResourceDirectory, ObjectFileFilters[i], SearchOption.AllDirectories).GetEnumerator();
				while (it.MoveNext())
				{
					mObjects.Add(it.Current.Substring(Settings.ProjectPath.Length + ResourceDirectory.Length));
				}
			}

			for (int i = 0; i < TextureFileFilters.Length; ++i)
			{
				IEnumerator<string> it = Directory.EnumerateFiles(Settings.ProjectPath + ResourceDirectory, TextureFileFilters[i], SearchOption.AllDirectories).GetEnumerator();
				while (it.MoveNext())
				{
					mTextures.Add(it.Current.Substring(Settings.ProjectPath.Length + ResourceDirectory.Length));
				}
			}

			Debug.Print("We have identified " + mObjects.Count + " object files, and " + mTextures.Count + " texture files!");
		}

		private void FileChanged(object sender, FileSystemEventArgs e)
		{
			if(ResourceUpdated != null)
			{
				ResourceUpdated(e);
			}
		}

		private void FileCreated(object sender, FileSystemEventArgs e)
		{
			if (ResourceAdded != null)
			{
				ResourceAdded(e);
			}
		}

		private void FileDeleted(object sender, FileSystemEventArgs e)
		{
			if (ResourceDeleted != null)
			{
				ResourceDeleted(e);
			}
		}

		private void FileRenamed(object sender, RenamedEventArgs e)
		{
			if (ResourceRenamed != null)
			{
				ResourceRenamed(e);
			}
		}
	}
}
