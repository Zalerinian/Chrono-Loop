using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;
using System.IO;

namespace LevelEditor
{
    class Mesh
    {
        private List<CustomVertex.PositionColoredTextured> mVertices;
        private Matrix mTransform;
        private string mTexture;

        public string Texture
        {
            get
            {
                return mTexture;
            }

            set
            {
                mTexture = value;
            }
        }

        Mesh()
        {
            mTransform = Matrix.Identity;
            mTexture = string.Empty;
        }

        void Load(string _file)
        {

        }

        void AddVert(CustomVertex.PositionColoredTextured _vert)
        {
            mVertices.Add(_vert);
        }

        CustomVertex.PositionColoredTextured[] GetVerts()
        {
            return mVertices.ToArray();
        }

        void Translate(Vector3 _offset)
        {
            mTransform.Translate(_offset);
        }

        void Rotate(Vector3 _rotation)
        {
            mTransform.RotateX(_rotation.X);
            mTransform.RotateY(_rotation.Y);
            mTransform.RotateZ(_rotation.Z);
        }

        void Scale(Vector3 _scale)
        {
            mTransform.Scale(_scale);
        }
    }
}
