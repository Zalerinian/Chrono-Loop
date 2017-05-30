using System;
using System.Windows.Forms;

namespace Hourglass{

	public partial class LevelOrderForm : Form {

		public LevelOrderForm() {
			InitializeComponent();
			FileIO.ReadLevelOrder(lbxLevels.Items);
		}

		private void btnAddLevel_Click(object sender, EventArgs e) {
			OpenFileDialog o = new OpenFileDialog() {
				Filter = "Binary Level Files (*.elf)|*.elf",
				Title = "Find a level to add",
				InitialDirectory = Settings.ProjectPath + ResourceManager.Instance.ResourceDirectory
			};
			if(o.ShowDialog() == DialogResult.OK) {
				if(!o.FileName.Contains(Settings.ProjectPath + ResourceManager.Instance.ResourceDirectory)) {
					MessageBox.Show("The selected level is not contained within the project's resource folder (" + Settings.ProjectPath + ResourceManager.Instance.ResourceDirectory + ")",
						"Caution", MessageBoxButtons.OK, MessageBoxIcon.Warning);
					//return;
				}

				for(int i = 0; i < lbxLevels.Items.Count; ++i) {
					LevelRecord r = lbxLevels.Items[i] as LevelRecord;
					if(r.FilePath == o.FileName.Substring((Settings.ProjectPath + ResourceManager.Instance.ResourceDirectory).Length)) {
						if (MessageBox.Show("This level is already included, insert it anyway?", "Duplicate level detected", MessageBoxButtons.YesNo, MessageBoxIcon.Question) != DialogResult.Yes) {
							// Duplicate detected, and Yes was not selected. Don't add the level.
							return;
						} else {
							// Duplicate detected, and yes *was* selected, so stop bothering them.
							break;
						}
					}
				}

				// Add the level to the bottom of the list.
				lbxLevels.Items.Add(
					new LevelRecord(o.FileName.Substring(o.FileName.LastIndexOf("\\") + 1),
					o.FileName.Substring((Settings.ProjectPath + ResourceManager.Instance.ResourceDirectory).Length
				)));
			}
		}

		private void btnAdd_Click(object sender, EventArgs e) {
			if(lbxLevels.SelectedIndex > 0) {
				int idx = lbxLevels.SelectedIndex;
				object lr = lbxLevels.Items[idx];
				lbxLevels.Items.RemoveAt(idx);
				lbxLevels.Items.Insert(idx - 1, lr);
				lbxLevels.SelectedIndex = lbxLevels.Items.Count - 1;
			}
		}

		private void btnRemoveLevel_Click(object sender, EventArgs e) {
			if(lbxLevels.SelectedIndex >= 0) {
				int idx = lbxLevels.SelectedIndex;
				lbxLevels.Items.RemoveAt(idx);
				lbxLevels.SelectedIndex = idx - 1;
				if(idx - 1 < 0 && lbxLevels.Items.Count > 0) {
					lbxLevels.SelectedIndex = 0;
				}
			}
		}

		private void btnUp_Click(object sender, EventArgs e) {
			if (lbxLevels.SelectedIndex > 0 && lbxLevels.SelectedIndex < lbxLevels.Items.Count) {
				int idx = lbxLevels.SelectedIndex;
				object lr = lbxLevels.Items[idx];
				lbxLevels.Items.RemoveAt(idx);
				lbxLevels.Items.Insert(idx - 1, lr);
				lbxLevels.SelectedIndex = idx - 1;
			}
		}

		private void btnDown_Click(object sender, EventArgs e) {
			if (lbxLevels.SelectedIndex < lbxLevels.Items.Count - 1 && lbxLevels.SelectedIndex >= 0) {
				int idx = lbxLevels.SelectedIndex;
				object lr = lbxLevels.Items[idx];
				lbxLevels.Items.RemoveAt(idx);
				lbxLevels.Items.Insert(idx + 1, lr);
				lbxLevels.SelectedIndex = idx + 1;
			}
		}

		private void btnCancel_Click(object sender, EventArgs e) {
			// Cancel changes by setting the DR to Cancel, and just closing the form.
			DialogResult = DialogResult.Cancel;
			Close();
		}

		private void btnOk_Click(object sender, EventArgs e) {
			DialogResult = DialogResult.OK;
			Close();
		}
	}

	class LevelRecord {
		public string FilePath;
		public string Text;

		public override string ToString() {
			return Text;
		}

		public LevelRecord(string _text, string _filePath) {
			Text = _text;
			FilePath = _filePath;
		}
	}
}
