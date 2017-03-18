using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Microsoft.DirectX;

namespace LevelEditor.Forms
{
    public partial class LevelSettingsForm : Form
    {
        private Vector3 mPos, mRot;

        public LevelSettingsForm()
        {
            InitializeComponent();
        }

        public Vector3 GetPosition()
        {
            mPos.X = (float)nPX.Value;
            mPos.Y = (float)nPY.Value;
            mPos.Z = (float)nPZ.Value;
            return mPos;
        }

        public Vector3 GetRotation()
        {
            mRot.X = (float)nRX.Value;
            mRot.Y = (float)nRY.Value;
            mRot.Z = (float)nRZ.Value;
            return mRot;
        }

        public void SetPosition(Vector3 _pos)
        {
            nPX.Value = (decimal)_pos.X;
            nPY.Value = (decimal)_pos.Y;
            nPZ.Value = (decimal)_pos.Z;
        }

        public void SetRotation(Vector3 _rot)
        {
            nRX.Value = (decimal)_rot.X;
            nRY.Value = (decimal)_rot.Y;
            nRZ.Value = (decimal)_rot.Z;
        }
    }
}
