using System;
using System.IO;
using System.Collections.Generic;
using Microsoft.DirectX;
using System.Windows.Forms;

namespace Hourglass
{

    public class BaseObject
    {
		public delegate void ComponentChangeHandler(Component _c);
		public event ComponentChangeHandler ComponentAdded;
		public event ComponentChangeHandler ComponentRemoved;

		private const bool ImplementHierarchy = false;

        protected static uint UID = 0;
        protected uint mObjectUID = UID++;
        protected List<Component> mComponents;
        protected BaseObject mParent = null;
        protected TreeNode mNode = null;
		protected Matrix mWorld = Matrix.Identity;

        public string Name {
            get { return ((TransformComponent)mComponents[0]).Name; }
            set { ((TransformComponent)mComponents[0]).Name = value; }
        }

        public TreeNode Node {
            get { return mNode; }
        }

		public BaseObject Parent {
			get {
				return mParent;
			}
			set {
				mParent = value;
			}
		}

        public BaseObject(TreeNode _node) : this(_node, "") {
        }

        public BaseObject(TreeNode _node, string _name)
        {
            mNode = _node;
            mComponents = new List<Component>();
            TransformComponent transform = new TransformComponent(_name);
			AddComponent(transform);
        }

        public List<Component> GetComponents()
        {
            return mComponents;
        }

		public Matrix GetMatrix()
		{
			if (mParent != null)
			{
				return mWorld * mParent.GetMatrix();
			}
			else
			{
				return mWorld;
			}
		}

		public void AddComponent(Component _c)
		{
			mComponents.Add(_c);
			_c.Owner = this;
			_c.RemoveControl += RelinquishControl;
			if(ComponentAdded != null)
			{ 
				ComponentAdded(_c);
			}
		}

		public void RemoveComponent(Component _c)
		{
			mComponents.Remove(_c);
			_c.Owner = null;

			// Is this actually needed, since we don't swap components with other objects?
			_c.RemoveControl -= RelinquishControl;
			if(ComponentRemoved != null)
			{
				ComponentRemoved(_c);
			}
			_c.GetGroupbox().Parent = null;
		}

		public void RelinquishControl()
		{
			if(mNode != null)
			{
				((Editor)mNode.TreeView.FindForm()).btnFocus.Select();
			}
		}

		public void InvalidateMatrix()
		{
			mWorld = ((TransformComponent)mComponents[0]).CreateMatrix();
		}

		public void Delete()
		{
			for(int i = mComponents.Count - 1; i >= 0 ; --i)
			{
				mComponents[i].OnMenuClick_Delete(null, null);
			}
		}

		public BaseObject Clone() {
			BaseObject b = new BaseObject(null);

			for(int i = 0; i < this.mComponents.Count; ++i) {
				Component c = null;
				switch (mComponents[i].Type) {
					case Component.ComponentType.Audio:
						c = new SoundComponent();
						break;
					case Component.ComponentType.BoxCollider:
						c = new BoxCollider();
						break;
					case Component.ComponentType.ButtonCollider:
						c = new ButtonCollider();
						break;
					case Component.ComponentType.Code:
						c = new CodeComponent();
						break;
					case Component.ComponentType.ColoredMesh:
						c = new ColoredMeshComponent();
						break;
					case Component.ComponentType.PlaneCollider:
						c = new PlaneCollider();
						break;
					case Component.ComponentType.SphereCollider:
						c = new SphereCollider();
						break;
					case Component.ComponentType.TexturedMesh:
						c = new TexturedMeshComponent();
						break;
					case Component.ComponentType.Transform:
						c = b.mComponents[0];
						break;
                    case Component.ComponentType.Light:
                        c = new LightComponent();
                        break;
					default:
						break;
				}
				if(c != null) {
					mComponents[i].CopyData(ref c);
					if(!(c is TransformComponent)) {
						b.AddComponent(c);
					}
				}
			}

			return b;
		}

		public void SetNode(TreeNode n) {
			mNode = n;
			((TransformComponent)mComponents[0]).OnUpdateName();
		}

    }
}
