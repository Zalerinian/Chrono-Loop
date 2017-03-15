using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;
using System.Drawing;

namespace LevelEditor
{
    class Changes
    {
        ToolObject mSelected = null;
        ToolObjectColor mSelectedCollider = null;
        Vector3 mPosition, mRotation, mExtra;
        bool mMove, mVisible, mColor;
        float mMass, mStaticF, mElasticity, mKeneticF, mDrag;
        #region Properties

        #endregion
    }
}
