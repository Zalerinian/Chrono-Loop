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
        protected string mNodeKey;
        protected uint mObjectUID = UID++;
        protected List<Component> mComponents;
        protected BaseObject mParent = null;
        protected TreeNode mNode = null;

        public string Name {
            get { return ((TransformComponent)mComponents[0]).Name; }
            set { ((TransformComponent)mComponents[0]).Name = value; }
        }

        public TreeNode Node {
            get { return mNode; }
        }

        public BaseObject(TreeNode _node) {
            mNode = _node;
            mComponents = new List<Component>();
            TransformComponent transform = new TransformComponent();
			AddComponent(transform);
        }

        public BaseObject(TreeNode _node, string _name) : this(_node)
        {
            ((TransformComponent)mComponents[0]).Name = _name;
            _node.Text = _name;
        }

        public List<Component> GetComponents()
        {
            return mComponents;
        }

		public Matrix GetMatrix()
		{
			if (mParent != null && ImplementHierarchy)
			{
				return ((TransformComponent)mComponents[0]).CreateMatrix() * mParent.GetMatrix();
			}
			else
			{
				return ((TransformComponent)mComponents[0]).CreateMatrix();
			}
		}

		public void AddComponent(Component _c)
		{
			mComponents.Add(_c);
			_c.Owner = this;
			if(ComponentAdded != null)
			{ 
				ComponentAdded(_c);
			}
		}

		public void RemoveComponent(Component _c)
		{
			mComponents.Remove(_c);
			_c.Owner = null;
			if(ComponentRemoved != null)
			{
				ComponentRemoved(_c);
			}
			_c.GetGroupbox().Parent = null;
		}

    }
}
