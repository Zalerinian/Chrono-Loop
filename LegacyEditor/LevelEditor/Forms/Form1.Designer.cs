
namespace LevelEditor
{
    partial class Editor
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Editor));
            System.Windows.Forms.TreeNode treeNode3 = new System.Windows.Forms.TreeNode("Objects");
            System.Windows.Forms.TreeNode treeNode4 = new System.Windows.Forms.TreeNode("Hierarchy");
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator = new System.Windows.Forms.ToolStripSeparator();
            this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveAsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.printToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.printPreviewToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.editToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.undoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.redoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.cutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.copyToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.pasteToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
            this.selectAllToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.levelSettingsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.componentEditorToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.customizeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.optionsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.contentsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.indexToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.searchToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator5 = new System.Windows.Forms.ToolStripSeparator();
            this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.spHierarchyPanel = new System.Windows.Forms.SplitContainer();
            this.Tree = new System.Windows.Forms.TreeView();
            this.splitContainer2 = new System.Windows.Forms.SplitContainer();
            this.graphicsPanel1 = new LevelEditor.GraphicsPanel();
            this.LeftToggle = new LevelEditor.NoFocusButton();
            this.RightToggle = new LevelEditor.NoFocusButton();
            this.visibleCheck = new System.Windows.Forms.CheckBox();
            this.MoveCheck = new System.Windows.Forms.CheckBox();
            this.Trigger = new System.Windows.Forms.CheckBox();
            this.colorSelect = new System.Windows.Forms.Button();
            this.Physics = new System.Windows.Forms.GroupBox();
            this.Drag = new System.Windows.Forms.NumericUpDown();
            this.MassL = new System.Windows.Forms.Label();
            this.DragL = new System.Windows.Forms.Label();
            this.Mass = new System.Windows.Forms.NumericUpDown();
            this.KeneticF = new System.Windows.Forms.NumericUpDown();
            this.ElasticityL = new System.Windows.Forms.Label();
            this.KeneticL = new System.Windows.Forms.Label();
            this.Elasticity = new System.Windows.Forms.NumericUpDown();
            this.StaticF = new System.Windows.Forms.NumericUpDown();
            this.StaticL = new System.Windows.Forms.Label();
            this.ExtraVector = new System.Windows.Forms.GroupBox();
            this.ExtraX = new System.Windows.Forms.NumericUpDown();
            this.label10 = new System.Windows.Forms.Label();
            this.ExtraY = new System.Windows.Forms.NumericUpDown();
            this.label11 = new System.Windows.Forms.Label();
            this.ExtraZ = new System.Windows.Forms.NumericUpDown();
            this.label12 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.rotZ = new System.Windows.Forms.NumericUpDown();
            this.rotY = new System.Windows.Forms.NumericUpDown();
            this.rotX = new System.Windows.Forms.NumericUpDown();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.scaleZ = new System.Windows.Forms.NumericUpDown();
            this.scaleY = new System.Windows.Forms.NumericUpDown();
            this.scaleX = new System.Windows.Forms.NumericUpDown();
            this.groupBox6 = new System.Windows.Forms.GroupBox();
            this.Radius = new System.Windows.Forms.NumericUpDown();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.posZ = new System.Windows.Forms.NumericUpDown();
            this.posY = new System.Windows.Forms.NumericUpDown();
            this.posX = new System.Windows.Forms.NumericUpDown();
            this.TextureBox = new System.Windows.Forms.GroupBox();
            this.changeTexture = new System.Windows.Forms.Button();
            this.textureFileBox = new System.Windows.Forms.TextBox();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.componetsCheck = new System.Windows.Forms.CheckedListBox();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.nameBox = new System.Windows.Forms.TextBox();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.menuStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.spHierarchyPanel)).BeginInit();
            this.spHierarchyPanel.Panel1.SuspendLayout();
            this.spHierarchyPanel.Panel2.SuspendLayout();
            this.spHierarchyPanel.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).BeginInit();
            this.splitContainer2.Panel1.SuspendLayout();
            this.splitContainer2.Panel2.SuspendLayout();
            this.splitContainer2.SuspendLayout();
            this.graphicsPanel1.SuspendLayout();
            this.Physics.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.Drag)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.Mass)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.KeneticF)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.Elasticity)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.StaticF)).BeginInit();
            this.ExtraVector.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.ExtraX)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.ExtraY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.ExtraZ)).BeginInit();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.rotZ)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.rotY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.rotX)).BeginInit();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.scaleZ)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.scaleY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.scaleX)).BeginInit();
            this.groupBox6.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.Radius)).BeginInit();
            this.groupBox3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.posZ)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.posY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.posX)).BeginInit();
            this.TextureBox.SuspendLayout();
            this.groupBox5.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.editToolStripMenuItem,
            this.toolsToolStripMenuItem,
            this.helpToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(1011, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newToolStripMenuItem,
            this.openToolStripMenuItem,
            this.toolStripSeparator,
            this.saveToolStripMenuItem,
            this.saveAsToolStripMenuItem,
            this.toolStripSeparator1,
            this.printToolStripMenuItem,
            this.printPreviewToolStripMenuItem,
            this.toolStripSeparator2,
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "&File";
            // 
            // newToolStripMenuItem
            // 
            this.newToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("newToolStripMenuItem.Image")));
            this.newToolStripMenuItem.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.newToolStripMenuItem.Name = "newToolStripMenuItem";
            this.newToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.N)));
            this.newToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.newToolStripMenuItem.Text = "&New";
            // 
            // openToolStripMenuItem
            // 
            this.openToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("openToolStripMenuItem.Image")));
            this.openToolStripMenuItem.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.openToolStripMenuItem.Name = "openToolStripMenuItem";
            this.openToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.O)));
            this.openToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.openToolStripMenuItem.Text = "&Open";
            this.openToolStripMenuItem.Click += new System.EventHandler(this.openToolStripMenuItem_Click);
            // 
            // toolStripSeparator
            // 
            this.toolStripSeparator.Name = "toolStripSeparator";
            this.toolStripSeparator.Size = new System.Drawing.Size(149, 6);
            // 
            // saveToolStripMenuItem
            // 
            this.saveToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("saveToolStripMenuItem.Image")));
            this.saveToolStripMenuItem.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
            this.saveToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.S)));
            this.saveToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.saveToolStripMenuItem.Text = "&Save";
            this.saveToolStripMenuItem.Click += new System.EventHandler(this.saveToolStripMenuItem_Click);
            // 
            // saveAsToolStripMenuItem
            // 
            this.saveAsToolStripMenuItem.Name = "saveAsToolStripMenuItem";
            this.saveAsToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.saveAsToolStripMenuItem.Text = "Save &As";
            this.saveAsToolStripMenuItem.Click += new System.EventHandler(this.saveAsToolStripMenuItem_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(149, 6);
            // 
            // printToolStripMenuItem
            // 
            this.printToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("printToolStripMenuItem.Image")));
            this.printToolStripMenuItem.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.printToolStripMenuItem.Name = "printToolStripMenuItem";
            this.printToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.P)));
            this.printToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.printToolStripMenuItem.Text = "&Print";
            // 
            // printPreviewToolStripMenuItem
            // 
            this.printPreviewToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("printPreviewToolStripMenuItem.Image")));
            this.printPreviewToolStripMenuItem.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.printPreviewToolStripMenuItem.Name = "printPreviewToolStripMenuItem";
            this.printPreviewToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.printPreviewToolStripMenuItem.Text = "Print Pre&view";
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(149, 6);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.exitToolStripMenuItem.Text = "E&xit";
            // 
            // editToolStripMenuItem
            // 
            this.editToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.undoToolStripMenuItem,
            this.redoToolStripMenuItem,
            this.toolStripSeparator3,
            this.cutToolStripMenuItem,
            this.copyToolStripMenuItem,
            this.pasteToolStripMenuItem,
            this.toolStripSeparator4,
            this.selectAllToolStripMenuItem});
            this.editToolStripMenuItem.Name = "editToolStripMenuItem";
            this.editToolStripMenuItem.Size = new System.Drawing.Size(39, 20);
            this.editToolStripMenuItem.Text = "&Edit";
            // 
            // undoToolStripMenuItem
            // 
            this.undoToolStripMenuItem.Name = "undoToolStripMenuItem";
            this.undoToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Z)));
            this.undoToolStripMenuItem.Size = new System.Drawing.Size(144, 22);
            this.undoToolStripMenuItem.Text = "&Undo";
            // 
            // redoToolStripMenuItem
            // 
            this.redoToolStripMenuItem.Name = "redoToolStripMenuItem";
            this.redoToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Y)));
            this.redoToolStripMenuItem.Size = new System.Drawing.Size(144, 22);
            this.redoToolStripMenuItem.Text = "&Redo";
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(141, 6);
            // 
            // cutToolStripMenuItem
            // 
            this.cutToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("cutToolStripMenuItem.Image")));
            this.cutToolStripMenuItem.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.cutToolStripMenuItem.Name = "cutToolStripMenuItem";
            this.cutToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.X)));
            this.cutToolStripMenuItem.Size = new System.Drawing.Size(144, 22);
            this.cutToolStripMenuItem.Text = "Cu&t";
            // 
            // copyToolStripMenuItem
            // 
            this.copyToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("copyToolStripMenuItem.Image")));
            this.copyToolStripMenuItem.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.copyToolStripMenuItem.Name = "copyToolStripMenuItem";
            this.copyToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.C)));
            this.copyToolStripMenuItem.Size = new System.Drawing.Size(144, 22);
            this.copyToolStripMenuItem.Text = "&Copy";
            // 
            // pasteToolStripMenuItem
            // 
            this.pasteToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("pasteToolStripMenuItem.Image")));
            this.pasteToolStripMenuItem.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.pasteToolStripMenuItem.Name = "pasteToolStripMenuItem";
            this.pasteToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.V)));
            this.pasteToolStripMenuItem.Size = new System.Drawing.Size(144, 22);
            this.pasteToolStripMenuItem.Text = "&Paste";
            // 
            // toolStripSeparator4
            // 
            this.toolStripSeparator4.Name = "toolStripSeparator4";
            this.toolStripSeparator4.Size = new System.Drawing.Size(141, 6);
            // 
            // selectAllToolStripMenuItem
            // 
            this.selectAllToolStripMenuItem.Name = "selectAllToolStripMenuItem";
            this.selectAllToolStripMenuItem.Size = new System.Drawing.Size(144, 22);
            this.selectAllToolStripMenuItem.Text = "Select &All";
            // 
            // toolsToolStripMenuItem
            // 
            this.toolsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.levelSettingsToolStripMenuItem,
            this.componentEditorToolStripMenuItem,
            this.optionsToolStripMenuItem,
            this.customizeToolStripMenuItem});
            this.toolsToolStripMenuItem.Name = "toolsToolStripMenuItem";
            this.toolsToolStripMenuItem.Size = new System.Drawing.Size(47, 20);
            this.toolsToolStripMenuItem.Text = "&Tools";
            // 
            // levelSettingsToolStripMenuItem
            // 
            this.levelSettingsToolStripMenuItem.Name = "levelSettingsToolStripMenuItem";
            this.levelSettingsToolStripMenuItem.Size = new System.Drawing.Size(172, 22);
            this.levelSettingsToolStripMenuItem.Text = "Level Settings";
            this.levelSettingsToolStripMenuItem.Click += new System.EventHandler(this.levelSettingsToolStripMenuItem_Click);
            // 
            // componentEditorToolStripMenuItem
            // 
            this.componentEditorToolStripMenuItem.Name = "componentEditorToolStripMenuItem";
            this.componentEditorToolStripMenuItem.Size = new System.Drawing.Size(172, 22);
            this.componentEditorToolStripMenuItem.Text = "Component Editor";
            this.componentEditorToolStripMenuItem.Click += new System.EventHandler(this.componentEditorToolStripMenuItem_Click);
            // 
            // customizeToolStripMenuItem
            // 
            this.customizeToolStripMenuItem.Name = "customizeToolStripMenuItem";
            this.customizeToolStripMenuItem.Size = new System.Drawing.Size(172, 22);
            this.customizeToolStripMenuItem.Text = "&Customize";
            // 
            // optionsToolStripMenuItem
            // 
            this.optionsToolStripMenuItem.Name = "optionsToolStripMenuItem";
            this.optionsToolStripMenuItem.Size = new System.Drawing.Size(172, 22);
            this.optionsToolStripMenuItem.Text = "&Options";
            this.optionsToolStripMenuItem.Click += new System.EventHandler(this.optionsToolStripMenuItem_Click);
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.contentsToolStripMenuItem,
            this.indexToolStripMenuItem,
            this.searchToolStripMenuItem,
            this.toolStripSeparator5,
            this.aboutToolStripMenuItem});
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            this.helpToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
            this.helpToolStripMenuItem.Text = "&Help";
            // 
            // contentsToolStripMenuItem
            // 
            this.contentsToolStripMenuItem.Name = "contentsToolStripMenuItem";
            this.contentsToolStripMenuItem.Size = new System.Drawing.Size(122, 22);
            this.contentsToolStripMenuItem.Text = "&Contents";
            // 
            // indexToolStripMenuItem
            // 
            this.indexToolStripMenuItem.Name = "indexToolStripMenuItem";
            this.indexToolStripMenuItem.Size = new System.Drawing.Size(122, 22);
            this.indexToolStripMenuItem.Text = "&Index";
            // 
            // searchToolStripMenuItem
            // 
            this.searchToolStripMenuItem.Name = "searchToolStripMenuItem";
            this.searchToolStripMenuItem.Size = new System.Drawing.Size(122, 22);
            this.searchToolStripMenuItem.Text = "&Search";
            // 
            // toolStripSeparator5
            // 
            this.toolStripSeparator5.Name = "toolStripSeparator5";
            this.toolStripSeparator5.Size = new System.Drawing.Size(119, 6);
            // 
            // aboutToolStripMenuItem
            // 
            this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
            this.aboutToolStripMenuItem.Size = new System.Drawing.Size(122, 22);
            this.aboutToolStripMenuItem.Text = "&About...";
            // 
            // spHierarchyPanel
            // 
            this.spHierarchyPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.spHierarchyPanel.FixedPanel = System.Windows.Forms.FixedPanel.Panel1;
            this.spHierarchyPanel.IsSplitterFixed = true;
            this.spHierarchyPanel.Location = new System.Drawing.Point(0, 24);
            this.spHierarchyPanel.Margin = new System.Windows.Forms.Padding(0);
            this.spHierarchyPanel.Name = "spHierarchyPanel";
            // 
            // spHierarchyPanel.Panel1
            // 
            this.spHierarchyPanel.Panel1.Controls.Add(this.Tree);
            this.spHierarchyPanel.Panel1.ForeColor = System.Drawing.SystemColors.Control;
            // 
            // spHierarchyPanel.Panel2
            // 
            this.spHierarchyPanel.Panel2.Controls.Add(this.splitContainer2);
            this.spHierarchyPanel.Size = new System.Drawing.Size(1011, 628);
            this.spHierarchyPanel.SplitterDistance = 150;
            this.spHierarchyPanel.SplitterWidth = 1;
            this.spHierarchyPanel.TabIndex = 3;
            this.spHierarchyPanel.TabStop = false;
            // 
            // Tree
            // 
            this.Tree.BackColor = System.Drawing.SystemColors.Control;
            this.Tree.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.Tree.CausesValidation = false;
            this.Tree.Dock = System.Windows.Forms.DockStyle.Fill;
            this.Tree.FullRowSelect = true;
            this.Tree.HideSelection = false;
            this.Tree.HotTracking = true;
            this.Tree.Indent = 10;
            this.Tree.Location = new System.Drawing.Point(0, 0);
            this.Tree.Name = "Tree";
            treeNode3.Name = "Objects";
            treeNode3.Text = "Objects";
            treeNode4.Name = "Hierarchy";
            treeNode4.Text = "Hierarchy";
            this.Tree.Nodes.AddRange(new System.Windows.Forms.TreeNode[] {
            treeNode3,
            treeNode4});
            this.Tree.PathSeparator = "/";
            this.Tree.ShowLines = false;
            this.Tree.Size = new System.Drawing.Size(150, 628);
            this.Tree.TabIndex = 0;
            this.Tree.TabStop = false;
            this.Tree.Click += new System.EventHandler(this.Tree_Click);
            this.Tree.DoubleClick += new System.EventHandler(this.Tree_DoubleClick);
            this.Tree.Enter += new System.EventHandler(this.Tree_Click);
            // 
            // splitContainer2
            // 
            this.splitContainer2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer2.FixedPanel = System.Windows.Forms.FixedPanel.Panel2;
            this.splitContainer2.Location = new System.Drawing.Point(0, 0);
            this.splitContainer2.Margin = new System.Windows.Forms.Padding(0);
            this.splitContainer2.Name = "splitContainer2";
            // 
            // splitContainer2.Panel1
            // 
            this.splitContainer2.Panel1.Controls.Add(this.graphicsPanel1);
            // 
            // splitContainer2.Panel2
            // 
            this.splitContainer2.Panel2.AutoScroll = true;
            this.splitContainer2.Panel2.Controls.Add(this.visibleCheck);
            this.splitContainer2.Panel2.Controls.Add(this.MoveCheck);
            this.splitContainer2.Panel2.Controls.Add(this.Trigger);
            this.splitContainer2.Panel2.Controls.Add(this.colorSelect);
            this.splitContainer2.Panel2.Controls.Add(this.Physics);
            this.splitContainer2.Panel2.Controls.Add(this.ExtraVector);
            this.splitContainer2.Panel2.Controls.Add(this.groupBox2);
            this.splitContainer2.Panel2.Controls.Add(this.groupBox1);
            this.splitContainer2.Panel2.Controls.Add(this.groupBox6);
            this.splitContainer2.Panel2.Controls.Add(this.groupBox3);
            this.splitContainer2.Panel2.Controls.Add(this.TextureBox);
            this.splitContainer2.Panel2.Controls.Add(this.groupBox5);
            this.splitContainer2.Panel2.Controls.Add(this.groupBox4);
            this.splitContainer2.Panel2MinSize = 300;
            this.splitContainer2.Size = new System.Drawing.Size(860, 628);
            this.splitContainer2.SplitterDistance = 526;
            this.splitContainer2.SplitterWidth = 10;
            this.splitContainer2.TabIndex = 3;
            // 
            // graphicsPanel1
            // 
            this.graphicsPanel1.Controls.Add(this.LeftToggle);
            this.graphicsPanel1.Controls.Add(this.RightToggle);
            this.graphicsPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.graphicsPanel1.Location = new System.Drawing.Point(0, 0);
            this.graphicsPanel1.Name = "graphicsPanel1";
            this.graphicsPanel1.Size = new System.Drawing.Size(526, 628);
            this.graphicsPanel1.TabIndex = 2;
            this.graphicsPanel1.Paint += new System.Windows.Forms.PaintEventHandler(this.Paint);
            this.graphicsPanel1.MouseClick += new System.Windows.Forms.MouseEventHandler(this.graphicsPanel1_MouseClick);
            this.graphicsPanel1.MouseLeave += new System.EventHandler(this.graphicsPanel1_MouseLeave);
            this.graphicsPanel1.MouseMove += new System.Windows.Forms.MouseEventHandler(this.graphicsPanel1_MouseMove);
            this.graphicsPanel1.MouseUp += new System.Windows.Forms.MouseEventHandler(this.graphicsPanel1_MouseUp);
            this.graphicsPanel1.Resize += new System.EventHandler(this.Resize);
            // 
            // LeftToggle
            // 
            this.LeftToggle.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.LeftToggle.Location = new System.Drawing.Point(3, 3);
            this.LeftToggle.MinimumSize = new System.Drawing.Size(1, 1);
            this.LeftToggle.Name = "LeftToggle";
            this.LeftToggle.Size = new System.Drawing.Size(23, 23);
            this.LeftToggle.TabIndex = 1;
            this.LeftToggle.Text = "<";
            this.LeftToggle.UseVisualStyleBackColor = true;
            this.LeftToggle.Click += new System.EventHandler(this.LeftToggle_Click);
            // 
            // RightToggle
            // 
            this.RightToggle.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.RightToggle.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.RightToggle.Location = new System.Drawing.Point(500, 3);
            this.RightToggle.MinimumSize = new System.Drawing.Size(1, 1);
            this.RightToggle.Name = "RightToggle";
            this.RightToggle.Size = new System.Drawing.Size(23, 23);
            this.RightToggle.TabIndex = 0;
            this.RightToggle.Text = ">";
            this.RightToggle.UseVisualStyleBackColor = true;
            this.RightToggle.Click += new System.EventHandler(this.RightToggle_Click);
            // 
            // visibleCheck
            // 
            this.visibleCheck.AutoSize = true;
            this.visibleCheck.Dock = System.Windows.Forms.DockStyle.Top;
            this.visibleCheck.Location = new System.Drawing.Point(0, 777);
            this.visibleCheck.Name = "visibleCheck";
            this.visibleCheck.Size = new System.Drawing.Size(307, 17);
            this.visibleCheck.TabIndex = 21;
            this.visibleCheck.Text = "Visible";
            this.visibleCheck.UseVisualStyleBackColor = true;
            this.visibleCheck.Visible = false;
            this.visibleCheck.CheckedChanged += new System.EventHandler(this.Trigger_CheckedChanged);
            // 
            // MoveCheck
            // 
            this.MoveCheck.AutoSize = true;
            this.MoveCheck.Dock = System.Windows.Forms.DockStyle.Top;
            this.MoveCheck.Location = new System.Drawing.Point(0, 760);
            this.MoveCheck.Name = "MoveCheck";
            this.MoveCheck.Size = new System.Drawing.Size(307, 17);
            this.MoveCheck.TabIndex = 13;
            this.MoveCheck.Text = "Can Move";
            this.MoveCheck.UseVisualStyleBackColor = true;
            this.MoveCheck.Visible = false;
            this.MoveCheck.CheckedChanged += new System.EventHandler(this.Trigger_CheckedChanged);
            // 
            // Trigger
            // 
            this.Trigger.AutoSize = true;
            this.Trigger.Dock = System.Windows.Forms.DockStyle.Top;
            this.Trigger.Location = new System.Drawing.Point(0, 743);
            this.Trigger.Name = "Trigger";
            this.Trigger.Size = new System.Drawing.Size(307, 17);
            this.Trigger.TabIndex = 12;
            this.Trigger.Text = "Trigger";
            this.Trigger.UseVisualStyleBackColor = true;
            this.Trigger.Visible = false;
            this.Trigger.CheckedChanged += new System.EventHandler(this.Trigger_CheckedChanged);
            // 
            // colorSelect
            // 
            this.colorSelect.AutoSize = true;
            this.colorSelect.BackColor = System.Drawing.Color.Red;
            this.colorSelect.Dock = System.Windows.Forms.DockStyle.Top;
            this.colorSelect.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.colorSelect.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.colorSelect.Location = new System.Drawing.Point(0, 718);
            this.colorSelect.Name = "colorSelect";
            this.colorSelect.Size = new System.Drawing.Size(307, 25);
            this.colorSelect.TabIndex = 8;
            this.colorSelect.Text = "Color";
            this.colorSelect.UseVisualStyleBackColor = false;
            this.colorSelect.Visible = false;
            this.colorSelect.Click += new System.EventHandler(this.colorSelect_Click);
            // 
            // Physics
            // 
            this.Physics.AutoSize = true;
            this.Physics.Controls.Add(this.Drag);
            this.Physics.Controls.Add(this.MassL);
            this.Physics.Controls.Add(this.DragL);
            this.Physics.Controls.Add(this.Mass);
            this.Physics.Controls.Add(this.KeneticF);
            this.Physics.Controls.Add(this.ElasticityL);
            this.Physics.Controls.Add(this.KeneticL);
            this.Physics.Controls.Add(this.Elasticity);
            this.Physics.Controls.Add(this.StaticF);
            this.Physics.Controls.Add(this.StaticL);
            this.Physics.Dock = System.Windows.Forms.DockStyle.Top;
            this.Physics.Location = new System.Drawing.Point(0, 559);
            this.Physics.Name = "Physics";
            this.Physics.Size = new System.Drawing.Size(307, 159);
            this.Physics.TabIndex = 1;
            this.Physics.TabStop = false;
            this.Physics.Text = "Pysics";
            this.Physics.Visible = false;
            // 
            // Drag
            // 
            this.Drag.DecimalPlaces = 2;
            this.Drag.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.Drag.Location = new System.Drawing.Point(41, 120);
            this.Drag.Margin = new System.Windows.Forms.Padding(0, 3, 0, 3);
            this.Drag.Maximum = new decimal(new int[] {
            999,
            0,
            0,
            0});
            this.Drag.Minimum = new decimal(new int[] {
            999,
            0,
            0,
            -2147483648});
            this.Drag.Name = "Drag";
            this.Drag.Size = new System.Drawing.Size(104, 20);
            this.Drag.TabIndex = 28;
            this.Drag.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.Drag.ValueChanged += new System.EventHandler(this.transform_ValueChanged);
            // 
            // MassL
            // 
            this.MassL.AutoSize = true;
            this.MassL.Location = new System.Drawing.Point(6, 18);
            this.MassL.Name = "MassL";
            this.MassL.Size = new System.Drawing.Size(32, 13);
            this.MassL.TabIndex = 21;
            this.MassL.Text = "Mass";
            // 
            // DragL
            // 
            this.DragL.AutoSize = true;
            this.DragL.Location = new System.Drawing.Point(6, 122);
            this.DragL.Name = "DragL";
            this.DragL.Size = new System.Drawing.Size(30, 13);
            this.DragL.TabIndex = 29;
            this.DragL.Text = "Drag";
            // 
            // Mass
            // 
            this.Mass.DecimalPlaces = 2;
            this.Mass.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.Mass.Location = new System.Drawing.Point(41, 16);
            this.Mass.Margin = new System.Windows.Forms.Padding(0, 3, 0, 3);
            this.Mass.Maximum = new decimal(new int[] {
            999,
            0,
            0,
            0});
            this.Mass.Minimum = new decimal(new int[] {
            999,
            0,
            0,
            -2147483648});
            this.Mass.Name = "Mass";
            this.Mass.Size = new System.Drawing.Size(104, 20);
            this.Mass.TabIndex = 8;
            this.Mass.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.Mass.ValueChanged += new System.EventHandler(this.transform_ValueChanged);
            // 
            // KeneticF
            // 
            this.KeneticF.DecimalPlaces = 2;
            this.KeneticF.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.KeneticF.Location = new System.Drawing.Point(89, 94);
            this.KeneticF.Margin = new System.Windows.Forms.Padding(0, 3, 0, 3);
            this.KeneticF.Maximum = new decimal(new int[] {
            999,
            0,
            0,
            0});
            this.KeneticF.Minimum = new decimal(new int[] {
            999,
            0,
            0,
            -2147483648});
            this.KeneticF.Name = "KeneticF";
            this.KeneticF.Size = new System.Drawing.Size(56, 20);
            this.KeneticF.TabIndex = 26;
            this.KeneticF.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.KeneticF.ValueChanged += new System.EventHandler(this.transform_ValueChanged);
            // 
            // ElasticityL
            // 
            this.ElasticityL.AutoSize = true;
            this.ElasticityL.Location = new System.Drawing.Point(6, 70);
            this.ElasticityL.Name = "ElasticityL";
            this.ElasticityL.Size = new System.Drawing.Size(48, 13);
            this.ElasticityL.TabIndex = 23;
            this.ElasticityL.Text = "Elasticity";
            // 
            // KeneticL
            // 
            this.KeneticL.AutoSize = true;
            this.KeneticL.Location = new System.Drawing.Point(6, 96);
            this.KeneticL.Name = "KeneticL";
            this.KeneticL.Size = new System.Drawing.Size(80, 13);
            this.KeneticL.TabIndex = 27;
            this.KeneticL.Text = "Kenetic Friction";
            // 
            // Elasticity
            // 
            this.Elasticity.DecimalPlaces = 2;
            this.Elasticity.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.Elasticity.Location = new System.Drawing.Point(57, 68);
            this.Elasticity.Margin = new System.Windows.Forms.Padding(0, 3, 0, 3);
            this.Elasticity.Maximum = new decimal(new int[] {
            999,
            0,
            0,
            0});
            this.Elasticity.Minimum = new decimal(new int[] {
            999,
            0,
            0,
            -2147483648});
            this.Elasticity.Name = "Elasticity";
            this.Elasticity.Size = new System.Drawing.Size(88, 20);
            this.Elasticity.TabIndex = 22;
            this.Elasticity.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.Elasticity.ValueChanged += new System.EventHandler(this.transform_ValueChanged);
            // 
            // StaticF
            // 
            this.StaticF.DecimalPlaces = 2;
            this.StaticF.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.StaticF.Location = new System.Drawing.Point(80, 42);
            this.StaticF.Margin = new System.Windows.Forms.Padding(0, 3, 0, 3);
            this.StaticF.Maximum = new decimal(new int[] {
            999,
            0,
            0,
            0});
            this.StaticF.Minimum = new decimal(new int[] {
            999,
            0,
            0,
            -2147483648});
            this.StaticF.Name = "StaticF";
            this.StaticF.Size = new System.Drawing.Size(65, 20);
            this.StaticF.TabIndex = 24;
            this.StaticF.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.StaticF.ValueChanged += new System.EventHandler(this.transform_ValueChanged);
            // 
            // StaticL
            // 
            this.StaticL.AutoSize = true;
            this.StaticL.Location = new System.Drawing.Point(6, 44);
            this.StaticL.Name = "StaticL";
            this.StaticL.Size = new System.Drawing.Size(71, 13);
            this.StaticL.TabIndex = 25;
            this.StaticL.Text = "Static Friction";
            // 
            // ExtraVector
            // 
            this.ExtraVector.AutoSize = true;
            this.ExtraVector.Controls.Add(this.ExtraX);
            this.ExtraVector.Controls.Add(this.label10);
            this.ExtraVector.Controls.Add(this.ExtraY);
            this.ExtraVector.Controls.Add(this.label11);
            this.ExtraVector.Controls.Add(this.ExtraZ);
            this.ExtraVector.Controls.Add(this.label12);
            this.ExtraVector.Dock = System.Windows.Forms.DockStyle.Top;
            this.ExtraVector.Location = new System.Drawing.Point(0, 485);
            this.ExtraVector.Name = "ExtraVector";
            this.ExtraVector.Size = new System.Drawing.Size(307, 74);
            this.ExtraVector.TabIndex = 20;
            this.ExtraVector.TabStop = false;
            this.ExtraVector.Text = "ExtraVector";
            this.ExtraVector.Visible = false;
            // 
            // ExtraX
            // 
            this.ExtraX.DecimalPlaces = 2;
            this.ExtraX.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.ExtraX.Location = new System.Drawing.Point(2, 35);
            this.ExtraX.Margin = new System.Windows.Forms.Padding(0, 3, 0, 3);
            this.ExtraX.Maximum = new decimal(new int[] {
            999,
            0,
            0,
            0});
            this.ExtraX.Minimum = new decimal(new int[] {
            999,
            0,
            0,
            -2147483648});
            this.ExtraX.Name = "ExtraX";
            this.ExtraX.Size = new System.Drawing.Size(48, 20);
            this.ExtraX.TabIndex = 14;
            this.ExtraX.ValueChanged += new System.EventHandler(this.transform_ValueChanged);
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(112, 16);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(14, 13);
            this.label10.TabIndex = 19;
            this.label10.Text = "Z";
            // 
            // ExtraY
            // 
            this.ExtraY.DecimalPlaces = 2;
            this.ExtraY.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.ExtraY.Location = new System.Drawing.Point(50, 35);
            this.ExtraY.Margin = new System.Windows.Forms.Padding(0, 3, 0, 3);
            this.ExtraY.Maximum = new decimal(new int[] {
            999,
            0,
            0,
            0});
            this.ExtraY.Minimum = new decimal(new int[] {
            999,
            0,
            0,
            -2147483648});
            this.ExtraY.Name = "ExtraY";
            this.ExtraY.Size = new System.Drawing.Size(48, 20);
            this.ExtraY.TabIndex = 15;
            this.ExtraY.ValueChanged += new System.EventHandler(this.transform_ValueChanged);
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(62, 16);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(14, 13);
            this.label11.TabIndex = 18;
            this.label11.Text = "Y";
            // 
            // ExtraZ
            // 
            this.ExtraZ.DecimalPlaces = 2;
            this.ExtraZ.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.ExtraZ.Location = new System.Drawing.Point(98, 35);
            this.ExtraZ.Margin = new System.Windows.Forms.Padding(0, 3, 0, 3);
            this.ExtraZ.Maximum = new decimal(new int[] {
            999,
            0,
            0,
            0});
            this.ExtraZ.Minimum = new decimal(new int[] {
            999,
            0,
            0,
            -2147483648});
            this.ExtraZ.Name = "ExtraZ";
            this.ExtraZ.Size = new System.Drawing.Size(48, 20);
            this.ExtraZ.TabIndex = 16;
            this.ExtraZ.ValueChanged += new System.EventHandler(this.transform_ValueChanged);
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(15, 16);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(14, 13);
            this.label12.TabIndex = 17;
            this.label12.Text = "X";
            // 
            // groupBox2
            // 
            this.groupBox2.AutoSize = true;
            this.groupBox2.Controls.Add(this.label7);
            this.groupBox2.Controls.Add(this.label8);
            this.groupBox2.Controls.Add(this.label9);
            this.groupBox2.Controls.Add(this.rotZ);
            this.groupBox2.Controls.Add(this.rotY);
            this.groupBox2.Controls.Add(this.rotX);
            this.groupBox2.Dock = System.Windows.Forms.DockStyle.Top;
            this.groupBox2.Location = new System.Drawing.Point(0, 413);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Padding = new System.Windows.Forms.Padding(0);
            this.groupBox2.Size = new System.Drawing.Size(307, 72);
            this.groupBox2.TabIndex = 9;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Rotation";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(112, 17);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(14, 13);
            this.label7.TabIndex = 7;
            this.label7.Text = "Z";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(62, 17);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(14, 13);
            this.label8.TabIndex = 6;
            this.label8.Text = "Y";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(15, 17);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(14, 13);
            this.label9.TabIndex = 5;
            this.label9.Text = "X";
            // 
            // rotZ
            // 
            this.rotZ.DecimalPlaces = 2;
            this.rotZ.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.rotZ.Location = new System.Drawing.Point(98, 36);
            this.rotZ.Margin = new System.Windows.Forms.Padding(0, 3, 0, 3);
            this.rotZ.Maximum = new decimal(new int[] {
            999,
            0,
            0,
            0});
            this.rotZ.Minimum = new decimal(new int[] {
            999,
            0,
            0,
            -2147483648});
            this.rotZ.Name = "rotZ";
            this.rotZ.Size = new System.Drawing.Size(48, 20);
            this.rotZ.TabIndex = 4;
            this.rotZ.ValueChanged += new System.EventHandler(this.transform_ValueChanged);
            this.rotZ.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.supress_KeyDown);
            // 
            // rotY
            // 
            this.rotY.DecimalPlaces = 2;
            this.rotY.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.rotY.Location = new System.Drawing.Point(50, 36);
            this.rotY.Margin = new System.Windows.Forms.Padding(0, 3, 0, 3);
            this.rotY.Maximum = new decimal(new int[] {
            999,
            0,
            0,
            0});
            this.rotY.Minimum = new decimal(new int[] {
            999,
            0,
            0,
            -2147483648});
            this.rotY.Name = "rotY";
            this.rotY.Size = new System.Drawing.Size(48, 20);
            this.rotY.TabIndex = 3;
            this.rotY.ValueChanged += new System.EventHandler(this.transform_ValueChanged);
            this.rotY.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.supress_KeyDown);
            // 
            // rotX
            // 
            this.rotX.DecimalPlaces = 2;
            this.rotX.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.rotX.Location = new System.Drawing.Point(2, 36);
            this.rotX.Margin = new System.Windows.Forms.Padding(0, 3, 0, 3);
            this.rotX.Maximum = new decimal(new int[] {
            999,
            0,
            0,
            0});
            this.rotX.Minimum = new decimal(new int[] {
            999,
            0,
            0,
            -2147483648});
            this.rotX.Name = "rotX";
            this.rotX.Size = new System.Drawing.Size(48, 20);
            this.rotX.TabIndex = 2;
            this.rotX.ValueChanged += new System.EventHandler(this.transform_ValueChanged);
            this.rotX.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.supress_KeyDown);
            // 
            // groupBox1
            // 
            this.groupBox1.AutoSize = true;
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.label5);
            this.groupBox1.Controls.Add(this.label6);
            this.groupBox1.Controls.Add(this.scaleZ);
            this.groupBox1.Controls.Add(this.scaleY);
            this.groupBox1.Controls.Add(this.scaleX);
            this.groupBox1.Dock = System.Windows.Forms.DockStyle.Top;
            this.groupBox1.Location = new System.Drawing.Point(0, 341);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Padding = new System.Windows.Forms.Padding(0);
            this.groupBox1.Size = new System.Drawing.Size(307, 72);
            this.groupBox1.TabIndex = 8;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Scale";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(112, 17);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(14, 13);
            this.label4.TabIndex = 7;
            this.label4.Text = "Z";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(62, 17);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(14, 13);
            this.label5.TabIndex = 6;
            this.label5.Text = "Y";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(15, 17);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(14, 13);
            this.label6.TabIndex = 5;
            this.label6.Text = "X";
            // 
            // scaleZ
            // 
            this.scaleZ.DecimalPlaces = 2;
            this.scaleZ.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.scaleZ.Location = new System.Drawing.Point(98, 36);
            this.scaleZ.Margin = new System.Windows.Forms.Padding(0, 3, 0, 3);
            this.scaleZ.Maximum = new decimal(new int[] {
            999,
            0,
            0,
            0});
            this.scaleZ.Minimum = new decimal(new int[] {
            999,
            0,
            0,
            -2147483648});
            this.scaleZ.Name = "scaleZ";
            this.scaleZ.Size = new System.Drawing.Size(48, 20);
            this.scaleZ.TabIndex = 4;
            this.scaleZ.ValueChanged += new System.EventHandler(this.transform_ValueChanged);
            this.scaleZ.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.supress_KeyDown);
            // 
            // scaleY
            // 
            this.scaleY.DecimalPlaces = 2;
            this.scaleY.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.scaleY.Location = new System.Drawing.Point(50, 36);
            this.scaleY.Margin = new System.Windows.Forms.Padding(0, 3, 0, 3);
            this.scaleY.Maximum = new decimal(new int[] {
            999,
            0,
            0,
            0});
            this.scaleY.Minimum = new decimal(new int[] {
            999,
            0,
            0,
            -2147483648});
            this.scaleY.Name = "scaleY";
            this.scaleY.Size = new System.Drawing.Size(48, 20);
            this.scaleY.TabIndex = 3;
            this.scaleY.ValueChanged += new System.EventHandler(this.transform_ValueChanged);
            this.scaleY.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.supress_KeyDown);
            // 
            // scaleX
            // 
            this.scaleX.DecimalPlaces = 2;
            this.scaleX.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.scaleX.Location = new System.Drawing.Point(2, 36);
            this.scaleX.Margin = new System.Windows.Forms.Padding(0, 3, 0, 3);
            this.scaleX.Maximum = new decimal(new int[] {
            999,
            0,
            0,
            0});
            this.scaleX.Minimum = new decimal(new int[] {
            999,
            0,
            0,
            -2147483648});
            this.scaleX.Name = "scaleX";
            this.scaleX.Size = new System.Drawing.Size(48, 20);
            this.scaleX.TabIndex = 2;
            this.scaleX.ValueChanged += new System.EventHandler(this.transform_ValueChanged);
            this.scaleX.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.supress_KeyDown);
            // 
            // groupBox6
            // 
            this.groupBox6.Controls.Add(this.Radius);
            this.groupBox6.Dock = System.Windows.Forms.DockStyle.Top;
            this.groupBox6.Location = new System.Drawing.Point(0, 301);
            this.groupBox6.Name = "groupBox6";
            this.groupBox6.Size = new System.Drawing.Size(307, 40);
            this.groupBox6.TabIndex = 11;
            this.groupBox6.TabStop = false;
            this.groupBox6.Text = "Radius";
            this.groupBox6.Visible = false;
            // 
            // Radius
            // 
            this.Radius.DecimalPlaces = 2;
            this.Radius.Dock = System.Windows.Forms.DockStyle.Top;
            this.Radius.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.Radius.Location = new System.Drawing.Point(3, 16);
            this.Radius.Margin = new System.Windows.Forms.Padding(0, 3, 0, 3);
            this.Radius.Maximum = new decimal(new int[] {
            999,
            0,
            0,
            0});
            this.Radius.Minimum = new decimal(new int[] {
            999,
            0,
            0,
            -2147483648});
            this.Radius.Name = "Radius";
            this.Radius.Size = new System.Drawing.Size(301, 20);
            this.Radius.TabIndex = 8;
            this.Radius.ValueChanged += new System.EventHandler(this.transform_ValueChanged);
            // 
            // groupBox3
            // 
            this.groupBox3.AutoSize = true;
            this.groupBox3.Controls.Add(this.label3);
            this.groupBox3.Controls.Add(this.label2);
            this.groupBox3.Controls.Add(this.label1);
            this.groupBox3.Controls.Add(this.posZ);
            this.groupBox3.Controls.Add(this.posY);
            this.groupBox3.Controls.Add(this.posX);
            this.groupBox3.Dock = System.Windows.Forms.DockStyle.Top;
            this.groupBox3.Location = new System.Drawing.Point(0, 229);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Padding = new System.Windows.Forms.Padding(0);
            this.groupBox3.Size = new System.Drawing.Size(307, 72);
            this.groupBox3.TabIndex = 5;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Position";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(112, 17);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(14, 13);
            this.label3.TabIndex = 7;
            this.label3.Text = "Z";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(62, 17);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(14, 13);
            this.label2.TabIndex = 6;
            this.label2.Text = "Y";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(15, 17);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(14, 13);
            this.label1.TabIndex = 5;
            this.label1.Text = "X";
            // 
            // posZ
            // 
            this.posZ.DecimalPlaces = 2;
            this.posZ.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.posZ.Location = new System.Drawing.Point(98, 36);
            this.posZ.Margin = new System.Windows.Forms.Padding(0, 3, 0, 3);
            this.posZ.Maximum = new decimal(new int[] {
            999,
            0,
            0,
            0});
            this.posZ.Minimum = new decimal(new int[] {
            999,
            0,
            0,
            -2147483648});
            this.posZ.Name = "posZ";
            this.posZ.Size = new System.Drawing.Size(48, 20);
            this.posZ.TabIndex = 4;
            this.posZ.ValueChanged += new System.EventHandler(this.transform_ValueChanged);
            this.posZ.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.supress_KeyDown);
            // 
            // posY
            // 
            this.posY.DecimalPlaces = 2;
            this.posY.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.posY.Location = new System.Drawing.Point(50, 36);
            this.posY.Margin = new System.Windows.Forms.Padding(0, 3, 0, 3);
            this.posY.Maximum = new decimal(new int[] {
            999,
            0,
            0,
            0});
            this.posY.Minimum = new decimal(new int[] {
            999,
            0,
            0,
            -2147483648});
            this.posY.Name = "posY";
            this.posY.Size = new System.Drawing.Size(48, 20);
            this.posY.TabIndex = 3;
            this.posY.ValueChanged += new System.EventHandler(this.transform_ValueChanged);
            this.posY.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.supress_KeyDown);
            // 
            // posX
            // 
            this.posX.DecimalPlaces = 2;
            this.posX.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.posX.Location = new System.Drawing.Point(2, 36);
            this.posX.Margin = new System.Windows.Forms.Padding(0, 3, 0, 3);
            this.posX.Maximum = new decimal(new int[] {
            999,
            0,
            0,
            0});
            this.posX.Minimum = new decimal(new int[] {
            999,
            0,
            0,
            -2147483648});
            this.posX.Name = "posX";
            this.posX.Size = new System.Drawing.Size(48, 20);
            this.posX.TabIndex = 2;
            this.posX.ValueChanged += new System.EventHandler(this.transform_ValueChanged);
            this.posX.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.supress_KeyDown);
            // 
            // TextureBox
            // 
            this.TextureBox.AutoSize = true;
            this.TextureBox.Controls.Add(this.changeTexture);
            this.TextureBox.Controls.Add(this.textureFileBox);
            this.TextureBox.Dock = System.Windows.Forms.DockStyle.Top;
            this.TextureBox.Location = new System.Drawing.Point(0, 177);
            this.TextureBox.Name = "TextureBox";
            this.TextureBox.Padding = new System.Windows.Forms.Padding(0);
            this.TextureBox.Size = new System.Drawing.Size(307, 52);
            this.TextureBox.TabIndex = 4;
            this.TextureBox.TabStop = false;
            this.TextureBox.Text = "Texture";
            // 
            // changeTexture
            // 
            this.changeTexture.FlatAppearance.BorderSize = 0;
            this.changeTexture.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.changeTexture.Location = new System.Drawing.Point(103, 19);
            this.changeTexture.Margin = new System.Windows.Forms.Padding(0);
            this.changeTexture.Name = "changeTexture";
            this.changeTexture.Size = new System.Drawing.Size(24, 20);
            this.changeTexture.TabIndex = 1;
            this.changeTexture.Text = "...";
            this.changeTexture.UseVisualStyleBackColor = true;
            this.changeTexture.Click += new System.EventHandler(this.changeTexture_Click);
            // 
            // textureFileBox
            // 
            this.textureFileBox.Location = new System.Drawing.Point(3, 19);
            this.textureFileBox.Margin = new System.Windows.Forms.Padding(0);
            this.textureFileBox.Name = "textureFileBox";
            this.textureFileBox.ReadOnly = true;
            this.textureFileBox.Size = new System.Drawing.Size(100, 20);
            this.textureFileBox.TabIndex = 0;
            this.textureFileBox.Text = "None";
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.componetsCheck);
            this.groupBox5.Dock = System.Windows.Forms.DockStyle.Top;
            this.groupBox5.Location = new System.Drawing.Point(0, 39);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(307, 138);
            this.groupBox5.TabIndex = 10;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "Components";
            // 
            // componetsCheck
            // 
            this.componetsCheck.BackColor = System.Drawing.SystemColors.Control;
            this.componetsCheck.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.componetsCheck.Dock = System.Windows.Forms.DockStyle.Fill;
            this.componetsCheck.FormattingEnabled = true;
            this.componetsCheck.Items.AddRange(new object[] {
            "Box Collider",
            "Sphere Collider",
            "Plane Collider",
            "Button Collider",
            "Particle Emitter",
            "Sound Emitter (NOT IMPLEMENTED)",
            "Box Snap",
            "Button Press",
            "AABB to AABB",
            "AABB to Sphere",
            "Elastic Plane",
            "Sphere to Sphere",
            "Enter Level",
            "Gesture",
            "Headset Follow",
            "Main Menu",
            "Teleport"});
            this.componetsCheck.Location = new System.Drawing.Point(3, 16);
            this.componetsCheck.Margin = new System.Windows.Forms.Padding(0);
            this.componetsCheck.Name = "componetsCheck";
            this.componetsCheck.Size = new System.Drawing.Size(301, 119);
            this.componetsCheck.TabIndex = 1;
            this.componetsCheck.ItemCheck += new System.Windows.Forms.ItemCheckEventHandler(this.componetsCheck_ItemCheck);
            // 
            // groupBox4
            // 
            this.groupBox4.AutoSize = true;
            this.groupBox4.Controls.Add(this.nameBox);
            this.groupBox4.Dock = System.Windows.Forms.DockStyle.Top;
            this.groupBox4.Location = new System.Drawing.Point(0, 0);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(307, 39);
            this.groupBox4.TabIndex = 0;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Name";
            // 
            // nameBox
            // 
            this.nameBox.Dock = System.Windows.Forms.DockStyle.Top;
            this.nameBox.Location = new System.Drawing.Point(3, 16);
            this.nameBox.Margin = new System.Windows.Forms.Padding(0);
            this.nameBox.Name = "nameBox";
            this.nameBox.Size = new System.Drawing.Size(301, 20);
            this.nameBox.TabIndex = 0;
            this.nameBox.TextChanged += new System.EventHandler(this.transform_ValueChanged);
            this.nameBox.KeyDown += new System.Windows.Forms.KeyEventHandler(this.nameBox_KeyDown);
            // 
            // timer1
            // 
            this.timer1.Enabled = true;
            this.timer1.Interval = 1;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // Editor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1011, 652);
            this.Controls.Add(this.spHierarchyPanel);
            this.Controls.Add(this.menuStrip1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "Editor";
            this.Text = "Level Editor";
            this.KeyUp += new System.Windows.Forms.KeyEventHandler(this.Editor_KeyDown);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.spHierarchyPanel.Panel1.ResumeLayout(false);
            this.spHierarchyPanel.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.spHierarchyPanel)).EndInit();
            this.spHierarchyPanel.ResumeLayout(false);
            this.splitContainer2.Panel1.ResumeLayout(false);
            this.splitContainer2.Panel2.ResumeLayout(false);
            this.splitContainer2.Panel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).EndInit();
            this.splitContainer2.ResumeLayout(false);
            this.graphicsPanel1.ResumeLayout(false);
            this.Physics.ResumeLayout(false);
            this.Physics.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.Drag)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.Mass)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.KeneticF)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.Elasticity)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.StaticF)).EndInit();
            this.ExtraVector.ResumeLayout(false);
            this.ExtraVector.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.ExtraX)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.ExtraY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.ExtraZ)).EndInit();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.rotZ)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.rotY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.rotX)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.scaleZ)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.scaleY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.scaleX)).EndInit();
            this.groupBox6.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.Radius)).EndInit();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.posZ)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.posY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.posX)).EndInit();
            this.TextureBox.ResumeLayout(false);
            this.TextureBox.PerformLayout();
            this.groupBox5.ResumeLayout(false);
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator;
        private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveAsToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem printToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem printPreviewToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem editToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem undoToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem redoToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripMenuItem cutToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem copyToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem pasteToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator4;
        private System.Windows.Forms.ToolStripMenuItem selectAllToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem toolsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem customizeToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem optionsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem contentsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem indexToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem searchToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator5;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
        private GraphicsPanel graphicsPanel1;
        private System.Windows.Forms.SplitContainer splitContainer2;
        private System.Windows.Forms.SplitContainer spHierarchyPanel;
        private NoFocusButton RightToggle;
        private NoFocusButton LeftToggle;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.TreeView Tree;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.NumericUpDown posX;
        private System.Windows.Forms.TextBox nameBox;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.NumericUpDown rotZ;
        private System.Windows.Forms.NumericUpDown rotY;
        private System.Windows.Forms.NumericUpDown rotX;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.NumericUpDown scaleZ;
        private System.Windows.Forms.NumericUpDown scaleY;
        private System.Windows.Forms.NumericUpDown scaleX;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.NumericUpDown posZ;
        private System.Windows.Forms.NumericUpDown posY;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.CheckedListBox componetsCheck;
        private System.Windows.Forms.GroupBox groupBox6;
        private System.Windows.Forms.NumericUpDown Radius;
        private System.Windows.Forms.CheckBox Trigger;
        private System.Windows.Forms.CheckBox MoveCheck;
        private System.Windows.Forms.GroupBox Physics;
        private System.Windows.Forms.NumericUpDown Drag;
        private System.Windows.Forms.Label MassL;
        private System.Windows.Forms.Label DragL;
        private System.Windows.Forms.NumericUpDown Mass;
        private System.Windows.Forms.NumericUpDown KeneticF;
        private System.Windows.Forms.Label ElasticityL;
        private System.Windows.Forms.Label KeneticL;
        private System.Windows.Forms.NumericUpDown Elasticity;
        private System.Windows.Forms.NumericUpDown StaticF;
        private System.Windows.Forms.Label StaticL;
        private System.Windows.Forms.GroupBox ExtraVector;
        private System.Windows.Forms.NumericUpDown ExtraX;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.NumericUpDown ExtraY;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.NumericUpDown ExtraZ;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.GroupBox TextureBox;
        private System.Windows.Forms.Button changeTexture;
        private System.Windows.Forms.TextBox textureFileBox;
        private System.Windows.Forms.CheckBox visibleCheck;
        private System.Windows.Forms.Button colorSelect;
        private System.Windows.Forms.ToolStripMenuItem levelSettingsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem componentEditorToolStripMenuItem;
    }
}

