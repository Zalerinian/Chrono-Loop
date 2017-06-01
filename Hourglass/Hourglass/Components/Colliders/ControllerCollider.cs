using System.IO;

namespace Hourglass {
	class ControllerCollider : BoxCollider {

		public ControllerCollider() : base(0) {
			for(int i = mGroupBox.Controls.Count - 1; i >= 0; --i) {
				mGroupBox.Controls.RemoveAt(0);
			}
			mGroupBox.Controls.Add(mLbScale);
			mGroupBox.Controls.Add(mScalePanel);
			mLbScale.Location = new System.Drawing.Point(6, 15);
			mScalePanel.Location = new System.Drawing.Point(90, 12);
			mGroupBox.Size = mGroupBox.PreferredSize;
		}

		public override void WriteData(BinaryWriter w) {
			w.Write((short)mType);
			w.Write((float)mScaleX.Value);
			w.Write((float)mScaleY.Value);
			w.Write((float)mScaleZ.Value);
		}

		public override void ReadData(BinaryReader r, int _version) {
			mScaleX.Value = (decimal)(System.BitConverter.ToSingle(r.ReadBytes(4), 0));
			mScaleY.Value = (decimal)(System.BitConverter.ToSingle(r.ReadBytes(4), 0));
			mScaleZ.Value = (decimal)(System.BitConverter.ToSingle(r.ReadBytes(4), 0));
		}

	}
}
