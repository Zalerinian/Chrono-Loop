using System;
using System.IO;
using System.Collections.Generic;
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;
using System.Drawing;

namespace Hourglass
{

    class BaseObject
    {
        private const bool ImplementHierarchy = false;

        protected static uint UID = 0;
        protected string mNodeKey;
        protected uint mObjectUID = UID++;
        protected List<Component> mComponents;
        protected BaseObject mParent = null;

        public string Name {
            get { return ((TransformComponent)mComponents[0]).Name; }
            set { ((TransformComponent)mComponents[0]).Name = value; }
        }

        public BaseObject() {
            mComponents = new List<Component>();
            TransformComponent transform = new TransformComponent(mComponents);
        }

        public BaseObject(string _name) : this()
        {
            ((TransformComponent)mComponents[0]).Name = _name;
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
