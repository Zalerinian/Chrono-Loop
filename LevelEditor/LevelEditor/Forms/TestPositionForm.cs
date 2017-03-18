using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Diagnostics;

namespace LevelEditor {
    public partial class TestPositionForm : Form {

        private ColliderComponent mColliderTest;
        private BoxCollider mBoxTest;
        private PlaneCollider mPlaneTest;
        private List<Component> mComponents;

        public TestPositionForm() {
            InitializeComponent();
            mComponents = new List<Component>(5);

            mColliderTest = new ColliderComponent(mComponents);
            mBoxTest = new BoxCollider(mComponents);
            mPlaneTest = new PlaneCollider(mComponents);
            Controls.Add(mColliderTest.GetGroupbox());
            Controls.Add(mBoxTest.GetGroupbox());
            Controls.Add(mPlaneTest.GetGroupbox());
            AutoSize = false;

            Padding p = new Padding(0);
            Padding = p;
            TestPositionForm_Resize(null, null);
        }

        private void TestPositionForm_Click(object sender, EventArgs e) {
            string breakpoint = "yup";
        }

        private void TestPositionForm_Resize(object sender, EventArgs e) {
            Point position = new Point(AutoScrollOffset.X, AutoScrollOffset.Y);
            Point NewScrollPos = new Point(0, AutoScrollPosition.Y);
            AutoScrollPosition = NewScrollPos;
            Size dimensions = new Size();
            for (int i = 0; i < mComponents.Count; ++i) {
                GroupBox box = mComponents[i].GetGroupbox();
                box.Location = position;
                position.Y += box.Size.Height + 3;

                dimensions.Width = ClientSize.Width;
                dimensions.Height = box.Height;
                box.Size = dimensions;
            }
        }

        private void TestPositionForm_Paint(object sender, PaintEventArgs e) {
            Point NewScrollPos = new Point(0, AutoScrollPosition.Y);
            AutoScrollPosition = NewScrollPos;
        }
    }
}
