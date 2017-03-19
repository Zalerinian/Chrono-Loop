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

        protected List<Component> mComponents;
        protected BaseObject mParent = null;

        public BaseObject(ref Device _device) {
            TransformComponent transform = new TransformComponent(mComponents);


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
