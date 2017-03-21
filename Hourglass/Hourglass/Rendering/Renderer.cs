using System;
using System.Collections.Generic;
using System.Windows.Forms;
using Microsoft.DirectX.Direct3D;
using Microsoft.DirectX;
using System.Diagnostics;

namespace Hourglass
{
	public sealed class Renderer
	{
		private Renderer() { }
		private static readonly Renderer sInstance = new Renderer();

		private List<RenderShape> mRenderSet;
		private bool mInitialized = false;
		private Device mDevice;
		private Control mPanel;

		private Vector3 mCameraPos = new Vector3(0, 0, 0);
		private float mAngleX = 0, mAngleY = 0;

		public static Renderer Instance {
			get { return sInstance; }
		}

		public float AngleY {
			get {
				return mAngleY;
			}
			set {
				mAngleY = value;
			}
		}

		public float AngleX {
			get {
				return mAngleX;
			}
			set {
				mAngleX = value;
			}
		}

		public Matrix View {
			get {
				return mDevice.Transform.View;
			}
			set {
				mDevice.Transform.View = value;
			}
		}

		public Device Device {
			get { return mDevice; }
		}


		public bool RenderGrid { get; set; }

		public void AttachToControl(Control _panel)
		{
			mPanel = _panel;
			PresentParameters p = new PresentParameters();
			p.Windowed = true;
			p.SwapEffect = SwapEffect.Copy;
			p.EnableAutoDepthStencil = true;
			p.AutoDepthStencilFormat = DepthFormat.D16;
			mDevice = new Device(0, DeviceType.Hardware, mPanel, CreateFlags.HardwareVertexProcessing, p);
			mDevice.RenderState.CullMode = Cull.None;
			mDevice.RenderState.FillMode = FillMode.Solid;
			mDevice.RenderState.ZBufferWriteEnable = true;
			mDevice.RenderState.ZBufferEnable = true;

			RebuildProjectionMatrix();
			RebuildViewMatrix();

			mRenderSet = new List<RenderShape>();

			mInitialized = true;
		}

		public void RebuildProjectionMatrix()
		{
			mDevice.Transform.Projection = Matrix.PerspectiveFovRH((float)Math.PI / 4.0f, (float)mPanel.Width / (float)mPanel.Height, 0.1f, 1000);
		}

		public void RebuildViewMatrix()
		{
			Matrix rotate = Matrix.RotationYawPitchRoll(mAngleY, mAngleX, 0);
			Vector4 transform = Vector3.Transform(new Vector3(0, 0, 3), rotate);
			Vector3 look = mCameraPos + new Vector3(transform.X, transform.Y, transform.Z);
			mDevice.Transform.View = Matrix.LookAtRH(mCameraPos, look, new Vector3(0, 1, 0));
			mDevice.RenderState.Lighting = false;
			mDevice.RenderState.ZBufferEnable = true;
			mDevice.RenderState.CullMode = Cull.Clockwise;
		}

		public void Render()
		{
			if(!mInitialized)
			{
				Debug.Print("Attempting to render the scene before setting up the renderer!");
				return;
			}


			mDevice.BeginScene();
			if(RenderGrid)
			{

			}
			List<RenderShape>.Enumerator it = mRenderSet.GetEnumerator();
			mDevice.Clear(ClearFlags.ZBuffer, 0, 1.0f, 0);
			mDevice.Clear(ClearFlags.Target, System.Drawing.Color.Black, 1.0f, 0);
			while (it.MoveNext())
			{

				// Render the shape.

				if(it.Current.Type == RenderShape.ShapeType.Textured)
				{
					mDevice.VertexFormat = CustomVertex.PositionNormalTextured.Format;
					TexturedShape ts = it.Current as TexturedShape;
					if(ts.Textures[(int)TexturedShape.TextureType.Diffuse] != null)
					{
						mDevice.SetTexture(0, ts.Textures[(int)TexturedShape.TextureType.Diffuse]);
					}
					else
					{
						// TODO: Default texture.
						mDevice.SetTexture(0, null);
					}
				}
				else
				{
					mDevice.VertexFormat = CustomVertex.PositionNormalColored.Format;
					mDevice.SetTexture(0, null);
					mDevice.RenderState.AlphaBlendEnable = false;
					mDevice.RenderState.CullMode = Cull.None;
				}
				mDevice.Indices = it.Current.IndexBuffer;
				mDevice.SetStreamSource(0, it.Current.VertexBuffer, 0);
				mDevice.RenderState.FillMode = it.Current.FillMode;
				mDevice.Transform.World = it.Current.World;
				mDevice.DrawIndexedPrimitives(PrimitiveType.TriangleList, 0, 0, it.Current.Indices.Length, 0, it.Current.Indices.Length / 3);
			}
			mDevice.EndScene();
			mDevice.Present();
		}
		
		public Texture LoadTexture(string file)
		{
			return TextureLoader.FromFile(mDevice, file);
		}

		public void AddShape(RenderShape _m)
		{
			mRenderSet.Add(_m);
		}

		public void RemoveShape(TexturedShape _m)
		{
			mRenderSet.Remove(_m);
		}



		private void DoRenderGrid() {
			uint width = 100, height = 100;
		}
	}
		
		

	
}
