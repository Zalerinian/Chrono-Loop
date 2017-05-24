using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Hourglass
{
	public interface IRenderable
	{

		RenderShape Shape { get; }

		int RenderStage { get; }
	}
}
