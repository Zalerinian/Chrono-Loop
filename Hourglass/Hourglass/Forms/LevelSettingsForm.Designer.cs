namespace Hourglass.Forms
{
    partial class LevelSettingsForm
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
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.nPX = new System.Windows.Forms.NumericUpDown();
            this.nPY = new System.Windows.Forms.NumericUpDown();
            this.nPZ = new System.Windows.Forms.NumericUpDown();
            this.nRZ = new System.Windows.Forms.NumericUpDown();
            this.nRY = new System.Windows.Forms.NumericUpDown();
            this.nRX = new System.Windows.Forms.NumericUpDown();
            this.btnOK = new System.Windows.Forms.Button();
            this.btnCancel = new System.Windows.Forms.Button();
            this.MaxCloneLabel = new System.Windows.Forms.Label();
            this.nMaxClone = new System.Windows.Forms.NumericUpDown();
            ((System.ComponentModel.ISupportInitialize)(this.nPX)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nPY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nPZ)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nRZ)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nRY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nRX)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nMaxClone)).BeginInit();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(21, 21);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(101, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Player Start Position";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(21, 146);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(143, 13);
            this.label2.TabIndex = 1;
            this.label2.Text = "Player Start Rotation (debug)";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(42, 48);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(139, 13);
            this.label3.TabIndex = 2;
            this.label3.Text = "X                  Y                  Z ";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(42, 168);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(139, 13);
            this.label4.TabIndex = 3;
            this.label4.Text = "X                  Y                  Z ";
            // 
            // nPX
            // 
            this.nPX.DecimalPlaces = 2;
            this.nPX.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.nPX.Location = new System.Drawing.Point(24, 74);
            this.nPX.Maximum = new decimal(new int[] {
            50000,
            0,
            0,
            0});
            this.nPX.Minimum = new decimal(new int[] {
            50000,
            0,
            0,
            -2147483648});
            this.nPX.Name = "nPX";
            this.nPX.Size = new System.Drawing.Size(57, 20);
            this.nPX.TabIndex = 4;
            // 
            // nPY
            // 
            this.nPY.DecimalPlaces = 2;
            this.nPY.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.nPY.Location = new System.Drawing.Point(87, 74);
            this.nPY.Maximum = new decimal(new int[] {
            50000,
            0,
            0,
            0});
            this.nPY.Minimum = new decimal(new int[] {
            50000,
            0,
            0,
            -2147483648});
            this.nPY.Name = "nPY";
            this.nPY.Size = new System.Drawing.Size(57, 20);
            this.nPY.TabIndex = 5;
            // 
            // nPZ
            // 
            this.nPZ.DecimalPlaces = 2;
            this.nPZ.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.nPZ.Location = new System.Drawing.Point(150, 74);
            this.nPZ.Maximum = new decimal(new int[] {
            50000,
            0,
            0,
            0});
            this.nPZ.Minimum = new decimal(new int[] {
            50000,
            0,
            0,
            -2147483648});
            this.nPZ.Name = "nPZ";
            this.nPZ.Size = new System.Drawing.Size(57, 20);
            this.nPZ.TabIndex = 6;
            // 
            // nRZ
            // 
            this.nRZ.DecimalPlaces = 2;
            this.nRZ.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.nRZ.Location = new System.Drawing.Point(150, 184);
            this.nRZ.Maximum = new decimal(new int[] {
            50000,
            0,
            0,
            0});
            this.nRZ.Minimum = new decimal(new int[] {
            50000,
            0,
            0,
            -2147483648});
            this.nRZ.Name = "nRZ";
            this.nRZ.Size = new System.Drawing.Size(57, 20);
            this.nRZ.TabIndex = 9;
            // 
            // nRY
            // 
            this.nRY.DecimalPlaces = 2;
            this.nRY.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.nRY.Location = new System.Drawing.Point(87, 184);
            this.nRY.Maximum = new decimal(new int[] {
            50000,
            0,
            0,
            0});
            this.nRY.Minimum = new decimal(new int[] {
            50000,
            0,
            0,
            -2147483648});
            this.nRY.Name = "nRY";
            this.nRY.Size = new System.Drawing.Size(57, 20);
            this.nRY.TabIndex = 8;
            // 
            // nRX
            // 
            this.nRX.DecimalPlaces = 2;
            this.nRX.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.nRX.Location = new System.Drawing.Point(24, 184);
            this.nRX.Maximum = new decimal(new int[] {
            50000,
            0,
            0,
            0});
            this.nRX.Minimum = new decimal(new int[] {
            50000,
            0,
            0,
            -2147483648});
            this.nRX.Name = "nRX";
            this.nRX.Size = new System.Drawing.Size(57, 20);
            this.nRX.TabIndex = 7;
            // 
            // btnOK
            // 
            this.btnOK.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.btnOK.Location = new System.Drawing.Point(24, 335);
            this.btnOK.Name = "btnOK";
            this.btnOK.Size = new System.Drawing.Size(75, 23);
            this.btnOK.TabIndex = 10;
            this.btnOK.Text = "OK";
            this.btnOK.UseVisualStyleBackColor = true;
            // 
            // btnCancel
            // 
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Location = new System.Drawing.Point(132, 335);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(75, 23);
            this.btnCancel.TabIndex = 11;
            this.btnCancel.Text = "Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            // 
            // MaxCloneLabel
            // 
            this.MaxCloneLabel.AutoSize = true;
            this.MaxCloneLabel.Location = new System.Drawing.Point(20, 246);
            this.MaxCloneLabel.Name = "MaxCloneLabel";
            this.MaxCloneLabel.Size = new System.Drawing.Size(102, 13);
            this.MaxCloneLabel.TabIndex = 12;
            this.MaxCloneLabel.Text = "Max Clones Allowed";
            // 
            // nMaxClone
            // 
            this.nMaxClone.Location = new System.Drawing.Point(23, 271);
            this.nMaxClone.Name = "nMaxClone";
            this.nMaxClone.Size = new System.Drawing.Size(57, 20);
            this.nMaxClone.TabIndex = 13;
            // 
            // LevelSettingsForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(238, 370);
            this.Controls.Add(this.nMaxClone);
            this.Controls.Add(this.MaxCloneLabel);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnOK);
            this.Controls.Add(this.nRZ);
            this.Controls.Add(this.nRY);
            this.Controls.Add(this.nRX);
            this.Controls.Add(this.nPZ);
            this.Controls.Add(this.nPY);
            this.Controls.Add(this.nPX);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Name = "LevelSettingsForm";
            this.Text = "LevelSettingsForm";
            this.Load += new System.EventHandler(this.LevelSettingsForm_Load);
            ((System.ComponentModel.ISupportInitialize)(this.nPX)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nPY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nPZ)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nRZ)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nRY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nRX)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nMaxClone)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.NumericUpDown nPX;
        private System.Windows.Forms.NumericUpDown nPY;
        private System.Windows.Forms.NumericUpDown nPZ;
        private System.Windows.Forms.NumericUpDown nRZ;
        private System.Windows.Forms.NumericUpDown nRY;
        private System.Windows.Forms.NumericUpDown nRX;
        private System.Windows.Forms.Button btnOK;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.Label MaxCloneLabel;
        private System.Windows.Forms.NumericUpDown nMaxClone;
    }
}