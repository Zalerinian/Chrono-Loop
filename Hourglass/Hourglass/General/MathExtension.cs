using Microsoft.DirectX;

namespace Hourglass
{
	public static class MathExtension
	{
		public static bool SameSign(this float v1, float v2)
		{
			return (v1 > 0 && v2 > 0) || (v1 < 0 && v2 < 0);
		}
	}
}
