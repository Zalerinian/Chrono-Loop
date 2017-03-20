using System;
using System.IO;
using System.Collections.Generic;
using Microsoft.DirectX;
using System.Windows.Forms;

namespace Hourglass
{

    public class BaseObject
    {
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
            TransformComponent transform = new TransformComponent(this);
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
            if(mParent != null && ImplementHierarchy)
            {
                return ((TransformComponent)mComponents[0]).CreateMatrix() * mParent.GetMatrix();
            }
            else
            {
                return ((TransformComponent)mComponents[0]).CreateMatrix();
            }
        }


    }
}
