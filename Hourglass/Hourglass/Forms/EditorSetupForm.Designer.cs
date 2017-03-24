namespace Hourglass
{
	partial class EditorSetupForm
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
			this.richTextBox1 = new System.Windows.Forms.RichTextBox();
			this.lbProjDir = new System.Windows.Forms.Label();
			this.btnSelectDir = new System.Windows.Forms.Button();
			this.tbProjDir = new System.Windows.Forms.TextBox();
			this.btnClose = new System.Windows.Forms.Button();
			this.btnDone = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// richTextBox1
			// 
			this.richTextBox1.BackColor = System.Drawing.SystemColors.Control;
			this.richTextBox1.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.richTextBox1.Cursor = System.Windows.Forms.Cursors.Arrow;
			this.richTextBox1.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.richTextBox1.Location = new System.Drawing.Point(12, 12);
			this.richTextBox1.Name = "richTextBox1";
			this.richTextBox1.ReadOnly = true;
			this.richTextBox1.Size = new System.Drawing.Size(500, 44);
			this.richTextBox1.TabIndex = 1;
			this.richTextBox1.Text = "Welcome to Hourglass! Before we get started, let\'s sort out a few details...";
			// 
			// lbProjDir
			// 
			this.lbProjDir.AutoSize = true;
			this.lbProjDir.Location = new System.Drawing.Point(12, 76);
			this.lbProjDir.Name = "lbProjDir";
			this.lbProjDir.Size = new System.Drawing.Size(72, 13);
			this.lbProjDir.TabIndex = 2;
			this.lbProjDir.Text = "Project Folder";
			// 
			// btnSelectDir
			// 
			this.btnSelectDir.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.btnSelectDir.Location = new System.Drawing.Point(480, 71);
			this.btnSelectDir.Name = "btnSelectDir";
			this.btnSelectDir.Size = new System.Drawing.Size(32, 23);
			this.btnSelectDir.TabIndex = 3;
			this.btnSelectDir.Text = "...";
			this.btnSelectDir.UseVisualStyleBackColor = true;
			this.btnSelectDir.Click += new System.EventHandler(this.btnSelectDir_Click);
			// 
			// tbProjDir
			// 
			this.tbProjDir.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.tbProjDir.Location = new System.Drawing.Point(90, 73);
			this.tbProjDir.Name = "tbProjDir";
			this.tbProjDir.Size = new System.Drawing.Size(384, 20);
			this.tbProjDir.TabIndex = 4;
			// 
			// btnClose
			// 
			this.btnClose.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.btnClose.Location = new System.Drawing.Point(404, 299);
			this.btnClose.Name = "btnClose";
			this.btnClose.Size = new System.Drawing.Size(108, 23);
			this.btnClose.TabIndex = 5;
			this.btnClose.Text = "Close Hourglass ☹";
			this.btnClose.UseVisualStyleBackColor = true;
			// 
			// btnDone
			// 
			this.btnDone.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.btnDone.Location = new System.Drawing.Point(290, 299);
			this.btnDone.Name = "btnDone";
			this.btnDone.Size = new System.Drawing.Size(108, 23);
			this.btnDone.TabIndex = 6;
			this.btnDone.Text = "All Done!";
			this.btnDone.UseVisualStyleBackColor = true;
			// 
			// EditorSetupForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.CancelButton = this.btnClose;
			this.ClientSize = new System.Drawing.Size(527, 334);
			this.Controls.Add(this.btnDone);
			this.Controls.Add(this.btnClose);
			this.Controls.Add(this.tbProjDir);
			this.Controls.Add(this.btnSelectDir);
			this.Controls.Add(this.lbProjDir);
			this.Controls.Add(this.richTextBox1);
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "EditorSetupForm";
			this.ShowIcon = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Editor Setup";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.RichTextBox richTextBox1;
		private System.Windows.Forms.Label lbProjDir;
		private System.Windows.Forms.Button btnSelectDir;
		private System.Windows.Forms.Button btnClose;
		private System.Windows.Forms.Button btnDone;
		public System.Windows.Forms.TextBox tbProjDir;
	}
}