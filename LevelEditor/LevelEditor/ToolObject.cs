using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;
using System.Drawing;

namespace LevelEditor
{
    class ToolObject
    {
        public const float RADIANS_TO_DEGREES = ((180.0f / 3.14f));
        public const float DEGREES_TO_RADIANS = (1 / 180.0f * 3.14f);
        private CustomVertex.PositionNormalTextured[] mVertices;
        private int[] mIndices;
        private IndexBuffer mIndexBuffer;
        private VertexBuffer mVertexBuffer;
        private Matrix mTransform;
        private Device mDevice;
        private Texture mTexture;
        private Material mMaterial;
        private bool mIsWireFrame;
        private Vector3 mPosition;
        private Vector3 mRotation;
        private Vector3 mScale;
        private string mName, mTextureFile, mMeshFile;

        #region Properties
        public CustomVertex.PositionNormalTextured[] Vertices
        {
            get { return mVertices; }
        }

        public int[] Indices
        {
            get { return mIndices; }
        }

        public IndexBuffer IndexBuffer
        {
            get { return mIndexBuffer; }
        }

        public VertexBuffer VertexBuffer
        {
            get { return mVertexBuffer; }
        }

        public Matrix Transform
        {
            get { return mTransform; }
        }

        public Texture Texture
        {
            get { return mTexture; }
        }

        public bool IsWireFrame
        {
            get { return mIsWireFrame; }
            set { mIsWireFrame = value; }
        }

        public Vector3 Position
        {
            get { return mPosition; }
        }

        public Vector3 Rotation
        {
            get { return mRotation; }
        }

        public Vector3 Scale
        {
            get { return mScale; }
        }

        public string Name
        {
            get { return mName; }
            set { mName = value; }
        }

        public string TextureFile
        {
            get { return mTextureFile; }
            set { mTextureFile = value; }
        }

        public string MeshFile
        {
            get { return mMeshFile; }
            set { mMeshFile = value; }
        }
        #endregion

        public ToolObject(ToolObject _Tool)
        {
            mName = _Tool.Name;
            mTransform = _Tool.mTransform;
            mPosition = _Tool.mPosition;
            mRotation = _Tool.mRotation;
            mScale = _Tool.mScale;
            mMaterial = _Tool.mMaterial;
            mDevice = _Tool.mDevice;
            mIsWireFrame = _Tool.mIsWireFrame;
            mTexture = _Tool.mTexture;
            mVertices = _Tool.mVertices;
            mIndices = _Tool.mIndices;
            mTextureFile = _Tool.mTextureFile;
            mMeshFile = _Tool.mMeshFile;
            VertexDeclaration();
            IndicesDeclaration();
        }
        public ToolObject(ref Device _Device)
        {
            mTransform = Matrix.Identity;
            mPosition = mRotation = new Vector3();
            mScale = new Vector3(1, 1, 1);
            mMaterial = new Material();
            mMaterial.Diffuse = Color.White;
            mMaterial.Specular = Color.LightGray;
            mMaterial.SpecularSharpness = 15.0F;
            mDevice = _Device;
            MakeGrid();
            mIsWireFrame = true;
            mTexture = null;
            VertexDeclaration();
            IndicesDeclaration();
        }
        public ToolObject(string _File, ref Device _Device)
        {
            mTransform = Matrix.Identity;
            mPosition = mRotation = new Vector3();
            mScale = new Vector3(1, 1, 1);
            mMaterial = new Material();
            mMaterial.Diffuse = Color.White;
            mMaterial.Specular = Color.LightGray;
            mMaterial.SpecularSharpness = 15.0F;
            mDevice = _Device;
            Load(_File);
            mIsWireFrame = false;
            mTexture = null;
            VertexDeclaration();
            IndicesDeclaration();
        }
        public ToolObject(string _File, string _Texture, ref Device _Device)
        {
            mTransform = Matrix.Identity;
            mPosition = mRotation = new Vector3();
            mScale = new Vector3(1, 1, 1);
            mMaterial = new Material();
            mMaterial.Diffuse = Color.White;
            mMaterial.Specular = Color.LightGray;
            mMaterial.SpecularSharpness = 0.0F;
            mDevice = _Device;
            Load(_File);
            mIsWireFrame = false;
            mTextureFile = _Texture;
            mTexture = TextureLoader.FromFile(_Device, _Texture);
            VertexDeclaration();
            IndicesDeclaration();
        }
        public void Load(string _File)
        {
            mMeshFile = _File;
            List<Vector3> Verts = new List<Vector3>();
            List<Vector3> Norms = new List<Vector3>();
            List<Vector2> UVs = new List<Vector2>();
            List<CustomVertex.PositionNormalTextured> vertices = new List<CustomVertex.PositionNormalTextured>();
            List<int> Ind = new List<int>();
            StreamReader sr = new StreamReader(_File);
            string line = string.Empty;
            while ((line = sr.ReadLine()) != null)
            {
                string[] parts = line.Split(new char[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);

                if (parts.Length > 0)
                {
                    Vector3 vec = new Vector3();
                    switch (parts[0])
                    {
                        case "o":
                            mName = parts[1];
                            break;
                        case "v":
                            vec.X = (float)Convert.ToDouble(parts[1]);
                            vec.Y = (float)Convert.ToDouble(parts[2]);
                            vec.Z = (float)Convert.ToDouble(parts[3]);
                            Verts.Add(vec);
                            break;
                        case "vn":
                            vec.X = (float)Convert.ToDouble(parts[1]);
                            vec.Y = (float)Convert.ToDouble(parts[2]);
                            vec.Z = (float)Convert.ToDouble(parts[3]);
                            Norms.Add(vec);
                            break;
                        case "vt":
                            Vector2 uv = new Vector2();
                            uv.X = (float)Convert.ToDouble(parts[1]);
                            uv.Y = (float)Convert.ToDouble(parts[2]);
                            UVs.Add(uv);
                            break;
                        case "f":
                            for (int i = 1; i < 4; i++)
                            {
                                string[] points = parts[i].Split(new char[] { '/' }, StringSplitOptions.None);
                                Ind.Add(vertices.Count);
                                vertices.Add(new CustomVertex.PositionNormalTextured(Verts[Convert.ToInt32(points[0]) - 1],
                                    Norms[Convert.ToInt32(points[2]) - 1],
                                    UVs[Convert.ToInt32(points[1]) - 1].X,
                                    UVs[Convert.ToInt32(points[1]) - 1].Y));
                            }
                            break;
                        default:
                            break;
                    }
                }
            }
            mVertices = vertices.ToArray();
            mIndices = Ind.ToArray();
        }
        public Vector3 TriNormal(int _triangleIndice)
        {
            Vector3 n1, n2, n3, nOut;
            n1 = mVertices[mIndices[_triangleIndice + 0]].Normal;
            n2 = mVertices[mIndices[_triangleIndice + 1]].Normal;
            n3 = mVertices[mIndices[_triangleIndice + 2]].Normal;
            nOut = (n1 + n2 + n3);
            nOut.Multiply(1.0f / 3.0f);
            return  nOut;
        }
        public bool RayHit(out Vector3 _out, Vector3 _start, Vector3 _end)
        {
            _out = new Vector3();
            Vector3 end = _end;
            bool hit = false;
            for (int i = 0; i < mIndices.Length; i += 3)
            {
                Vector3 norm = TriNormal(i);
                if (Vector3.Dot(_start, norm) - Vector3.Dot(mVertices[mIndices[i]].Position, norm) < 0)
                    continue;
                if (Vector3.Dot(end, norm) - Vector3.Dot(mVertices[mIndices[i]].Position, norm) > 0)
                    continue;
                //Plane intersection
                float D0 = Vector3.Dot(norm, _start);
                float D1 = Vector3.Dot(norm, mVertices[mIndices[i]].Position);
                float D2 = D0 - D1;
                Vector3 L = end - _start;
                float D3 = Vector3.Dot(norm, L);
                Vector3 CP = _start + (L * (-1.0f * (D2 / D3)));
                //Point in triangle
                bool outside = false;
                for (int j = 0; j < 3; j++)
                {
                    if (outside)
                        continue;
                    Vector3 Edge0 = mVertices[mIndices[i + ((j + 1) % 3)]].Position - mVertices[mIndices[i + j]].Position;
                    Vector3 Normal0 = Vector3.Cross(Edge0, norm);

                    if (Vector3.Dot(CP - mVertices[mIndices[i + j]].Position, Normal0) > 0)
                        outside = true;
                }
                if (!outside)
                {
                    end = CP;
                    hit = true;
                }
            }
            if (hit)
                _out = end;
            return hit;
        }
        public void Invert()
        {
            for (int i = 0; i < mIndices.Length / 3; i++)
            {
                int temp = mIndices[(i * 3)];
                mIndices[(i * 3)] = mIndices[(i * 3) + 2];
                mIndices[(i * 3) + 2] = temp;
            }
        }
        public void MakeGrid()
        {
            List<CustomVertex.PositionNormalTextured> verts = new List<CustomVertex.PositionNormalTextured>();
            List<int> ind = new List<int>();
            for (int i = 0; i <= 100; i++)
            {
                ind.Add(verts.Count);
                verts.Add(new CustomVertex.PositionNormalTextured(new Vector3((float)i - 50, 0, -50), new Vector3(), 0, 0));
                ind.Add(verts.Count);
                verts.Add(new CustomVertex.PositionNormalTextured(new Vector3((float)i - 50, 0, 50), new Vector3(), 0, 0));
                ind.Add(verts.Count);
                verts.Add(new CustomVertex.PositionNormalTextured(new Vector3((float)i  - 50, 0, -50), new Vector3(), 0, 0));
            }
            for (int i = 0; i <= 100; i++)
            {
                ind.Add(verts.Count);
                verts.Add(new CustomVertex.PositionNormalTextured(new Vector3(-50, 0, (float)i - 50), new Vector3(), 0, 0));
                ind.Add(verts.Count);
                verts.Add(new CustomVertex.PositionNormalTextured(new Vector3(50, 0, (float)i - 50), new Vector3(), 0, 0));
                ind.Add(verts.Count);
                verts.Add(new CustomVertex.PositionNormalTextured(new Vector3(-50, 0, (float)i - 50), new Vector3(), 0, 0));
            }
            mVertices = verts.ToArray();
            mIndices = ind.ToArray();
        }
        public void VertexDeclaration()
        {
            mVertexBuffer = new VertexBuffer(typeof(CustomVertex.PositionNormalTextured), mVertices.Length, mDevice, Usage.Dynamic | Usage.WriteOnly, CustomVertex.PositionNormalTextured.Format, Pool.Default);
            mVertexBuffer.SetData(mVertices, 0, LockFlags.None);
        }
        public void IndicesDeclaration()
        {
            mIndexBuffer = new IndexBuffer(typeof(int), mIndices.Length * sizeof(int), mDevice, Usage.WriteOnly, Pool.Default);
            mIndexBuffer.SetData(mIndices, 0, LockFlags.None);
        }
        public void Translate(Vector3 _Offset)
        {
            mTransform = mTransform * Matrix.Translation(_Offset);
            mPosition += _Offset;
        }
        public void SetPosition(Vector3 _Pos)
        {
            mPosition = _Pos;
            SetRotate(mRotation);
        }
        public void Rotate(Vector3 _Rotation)
        {
            mRotation += _Rotation;
            float circle = (360.0f * DEGREES_TO_RADIANS);
            mRotation.X = (float)(mRotation.X - circle * Math.Round(mRotation.X / circle));
            mRotation.Y = (float)(mRotation.Y - circle * Math.Round(mRotation.Y / circle));
            mRotation.Z = (float)(mRotation.Z - circle * Math.Round(mRotation.Z / circle));
            SetRotate(mRotation);
        }
        public void SetRotate(Vector3 _Rotation)
        {
            mTransform = Matrix.RotationYawPitchRoll(_Rotation.Y, _Rotation.X, _Rotation.Z);
            mTransform = Matrix.Scaling(mScale) * mTransform * Matrix.Translation(mPosition);
            
            mRotation = _Rotation;
        }
        public void AddScale(Vector3 _Scale)
        {
            if (Math.Abs(mScale.X * _Scale.X) < 998 && Math.Abs(mScale.Y * _Scale.Y) < 998 && Math.Abs(mScale.Z * _Scale.Z) < 998)
            {
                mTransform = mTransform * Matrix.Scaling(_Scale);
                mScale = new Vector3(mScale.X * _Scale.X, mScale.Y * _Scale.Y, mScale.Z * _Scale.Z);
            }
        }
        public void SetScale(Vector3 _Scale)
        {
            mScale = new Vector3(_Scale.X, _Scale.Y, _Scale.Z);
            SetPosition(mPosition);
        }
    }
}
