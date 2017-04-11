using System.Windows.Forms;


namespace Hourglass
{
	public interface IGizmoAttachment
	{
		NumericUpDown PX { get; }
		NumericUpDown PY { get; }
		NumericUpDown PZ { get; }

		NumericUpDown RX { get; }
		NumericUpDown RY { get; }
		NumericUpDown RZ { get; }

		NumericUpDown SX { get; }
		NumericUpDown SY { get; }
		NumericUpDown SZ { get; }

		Microsoft.DirectX.Matrix GizmoWorld { get; }

		void OnGizmoAttached();

		void OnGizmoDetached();
	}
}

