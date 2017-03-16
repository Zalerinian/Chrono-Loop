using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace LevelEditor {
    public abstract class Component {
        protected GroupBox mGroupBox;
        protected ContextMenuStrip mMenuStrip;
        protected ToolStripMenuItem mMenuItemDelete, mMenuItemReset;
        protected List<Component> mContainerReference;


        int Width {
            get {
                return mGroupBox.Size.Width;
            }
        }

        int Height {
            get {
                return mGroupBox.Size.Height;
            }
        }

        public Component(List<Component> _container) {
            mContainerReference = _container;
            mContainerReference.Add(this);

            mGroupBox = new GroupBox();
            mMenuStrip = new ContextMenuStrip();
            mMenuItemReset = new ToolStripMenuItem("Reset", null, OnMenuClick_Reset);
            mMenuItemDelete = new ToolStripMenuItem("Delete", null, OnMenuClick_Delete);
            mMenuStrip.Items.Add(mMenuItemReset);
            mMenuStrip.Items.Add(mMenuItemDelete);

            mGroupBox.Size = new System.Drawing.Size(0, 0);
            mGroupBox.ContextMenuStrip = mMenuStrip;
        }

        public GroupBox GetGroupbox() {
            return mGroupBox;
        }

        protected virtual void OnMenuClick_Delete(object sender, EventArgs e) {
            mContainerReference.Remove(this);
            mGroupBox.Parent = null;
        }

        protected virtual void OnMenuClick_Reset(object sender, EventArgs e) {
            MessageBox.Show("Someone was lazy and didn't implement the Reset option for this component.",
                "Error: ID10-T",
                MessageBoxButtons.OK);
        }
    }
}