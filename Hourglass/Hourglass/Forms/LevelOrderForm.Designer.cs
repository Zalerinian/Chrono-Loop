namespace Hourglass {
	partial class LevelOrderForm {
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing) {
			if (disposing && (components != null)) {
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent() {
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(LevelOrderForm));
			this.btnOk = new System.Windows.Forms.Button();
			this.btnCancel = new System.Windows.Forms.Button();
			this.lbxLevels = new System.Windows.Forms.ListBox();
			this.btnAdd = new System.Windows.Forms.Button();
			this.btnDown = new System.Windows.Forms.Button();
			this.btnAddLevel = new System.Windows.Forms.Button();
			this.btnRemoveLevel = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// btnOk
			// 
			this.btnOk.Location = new System.Drawing.Point(98, 254);
			this.btnOk.Name = "btnOk";
			this.btnOk.Size = new System.Drawing.Size(75, 23);
			this.btnOk.TabIndex = 0;
			this.btnOk.Text = "OK";
			this.btnOk.UseVisualStyleBackColor = true;
			this.btnOk.Click += new System.EventHandler(this.btnOk_Click);
			// 
			// btnCancel
			// 
			this.btnCancel.Location = new System.Drawing.Point(179, 254);
			this.btnCancel.Name = "btnCancel";
			this.btnCancel.Size = new System.Drawing.Size(75, 23);
			this.btnCancel.TabIndex = 1;
			this.btnCancel.Text = "Cancel";
			this.btnCancel.UseVisualStyleBackColor = true;
			this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
			// 
			// lbxLevels
			// 
			this.lbxLevels.FormattingEnabled = true;
			this.lbxLevels.Location = new System.Drawing.Point(12, 12);
			this.lbxLevels.Name = "lbxLevels";
			this.lbxLevels.Size = new System.Drawing.Size(199, 225);
			this.lbxLevels.TabIndex = 2;
			// 
			// btnAdd
			// 
			this.btnAdd.Image = ((System.Drawing.Image)(resources.GetObject("btnAdd.Image")));
			this.btnAdd.Location = new System.Drawing.Point(217, 12);
			this.btnAdd.Name = "btnAdd";
			this.btnAdd.Size = new System.Drawing.Size(37, 38);
			this.btnAdd.TabIndex = 3;
			this.btnAdd.UseVisualStyleBackColor = true;
			this.btnAdd.Click += new System.EventHandler(this.btnUp_Click);
			// 
			// btnDown
			// 
			this.btnDown.Image = ((System.Drawing.Image)(resources.GetObject("btnDown.Image")));
			this.btnDown.Location = new System.Drawing.Point(217, 56);
			this.btnDown.Name = "btnDown";
			this.btnDown.Size = new System.Drawing.Size(37, 38);
			this.btnDown.TabIndex = 4;
			this.btnDown.UseVisualStyleBackColor = true;
			this.btnDown.Click += new System.EventHandler(this.btnDown_Click);
			// 
			// btnAddLevel
			// 
			this.btnAddLevel.Image = ((System.Drawing.Image)(resources.GetObject("btnAddLevel.Image")));
			this.btnAddLevel.Location = new System.Drawing.Point(217, 100);
			this.btnAddLevel.Name = "btnAddLevel";
			this.btnAddLevel.Size = new System.Drawing.Size(37, 38);
			this.btnAddLevel.TabIndex = 5;
			this.btnAddLevel.UseVisualStyleBackColor = true;
			this.btnAddLevel.Click += new System.EventHandler(this.btnAddLevel_Click);
			// 
			// btnRemoveLevel
			// 
			this.btnRemoveLevel.Image = ((System.Drawing.Image)(resources.GetObject("btnRemoveLevel.Image")));
			this.btnRemoveLevel.Location = new System.Drawing.Point(217, 144);
			this.btnRemoveLevel.Name = "btnRemoveLevel";
			this.btnRemoveLevel.Size = new System.Drawing.Size(37, 38);
			this.btnRemoveLevel.TabIndex = 6;
			this.btnRemoveLevel.UseVisualStyleBackColor = true;
			this.btnRemoveLevel.Click += new System.EventHandler(this.btnRemoveLevel_Click);
			// 
			// LevelOrderForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(266, 289);
			this.Controls.Add(this.btnRemoveLevel);
			this.Controls.Add(this.btnAddLevel);
			this.Controls.Add(this.btnDown);
			this.Controls.Add(this.btnAdd);
			this.Controls.Add(this.lbxLevels);
			this.Controls.Add(this.btnCancel);
			this.Controls.Add(this.btnOk);
			this.MaximizeBox = false;
			this.MaximumSize = new System.Drawing.Size(282, 328);
			this.MinimizeBox = false;
			this.MinimumSize = new System.Drawing.Size(282, 328);
			this.Name = "LevelOrderForm";
			this.Text = "LevelOrderForm";
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.Button btnOk;
		private System.Windows.Forms.Button btnCancel;
		private System.Windows.Forms.Button btnAdd;
		private System.Windows.Forms.Button btnDown;
		private System.Windows.Forms.Button btnAddLevel;
		private System.Windows.Forms.Button btnRemoveLevel;
		public System.Windows.Forms.ListBox lbxLevels;
	}
}