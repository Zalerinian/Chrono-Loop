using System.Windows.Forms;


namespace Hourglass
{
	public interface IGizmoAttachment
	{
		NumericUpDown PX { get; set; }
		NumericUpDown PY { get; set; }
		NumericUpDown PZ { get; set; }

		NumericUpDown RX { get; set; }
		NumericUpDown RY { get; set; }
		NumericUpDown RZ { get; set; }

		NumericUpDown SX { get; set; }
		NumericUpDown SY { get; set; }
		NumericUpDown SZ { get; set; }

		Microsoft.DirectX.Matrix GizmoWorld { get; }
	}
}

