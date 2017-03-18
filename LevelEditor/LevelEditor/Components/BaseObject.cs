using System;
using System.IO;
using System.Collections.Generic;
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;
using System.Drawing;

namespace LevelEditor
{

    class BaseObject
    {

        protected List<Component> mComponents;
        protected BaseObject mParent = null;

        public BaseObject(ref Device _device) {
            TransformComponent transform = new TransformComponent(mComponents);


        }

        public List<Component> GetComponents()
        {
            return mComponents;
        }

        

    }
}
