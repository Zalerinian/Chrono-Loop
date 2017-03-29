namespace LevelEditor
{
    partial class OptionsForm
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
            this.btnOK = new System.Windows.Forms.Button();
            this.btnCancel = new System.Windows.Forms.Button();
            this.lbSoundbank = new System.Windows.Forms.Label();
            this.tbSoundbankText = new System.Windows.Forms.TextBox();
            this.btnSoundbankSelector = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // btnOK
            // 
            this.btnOK.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.btnOK.Location = new System.Drawing.Point(174, 152);
            this.btnOK.Name = "btnOK";
            this.btnOK.Size = new System.Drawing.Size(75, 23);
            this.btnOK.TabIndex = 0;
            this.btnOK.Text = "OK";
            this.btnOK.UseVisualStyleBackColor = true;
            // 
            // btnCancel
            // 
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Location = new System.Drawing.Point(255, 152);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(75, 23);
            this.btnCancel.TabIndex = 1;
            this.btnCancel.Text = "Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            // 
            // lbSoundbank
            // 
            this.lbSoundbank.AutoSize = true;
            this.lbSoundbank.Location = new System.Drawing.Point(12, 22);
            this.lbSoundbank.Name = "lbSoundbank";
            this.lbSoundbank.Size = new System.Drawing.Size(108, 13);
            this.lbSoundbank.TabIndex = 2;
            this.lbSoundbank.Text = "Soundbank Text File:";
            // 
            // tbSoundbankText
            // 
            this.tbSoundbankText.Location = new System.Drawing.Point(126, 19);
            this.tbSoundbankText.Name = "tbSoundbankText";
            this.tbSoundbankText.Size = new System.Drawing.Size(150, 20);
            this.tbSoundbankText.TabIndex = 3;
            // 
            // btnSoundbankSelector
            // 
            this.btnSoundbankSelector.Location = new System.Drawing.Point(282, 18);
            this.btnSoundbankSelector.Name = "btnSoundbankSelector";
            this.btnSoundbankSelector.Size = new System.Drawing.Size(26, 22);
            this.btnSoundbankSelector.TabIndex = 4;
            this.btnSoundbankSelector.Text = "...";
            this.btnSoundbankSelector.UseVisualStyleBackColor = true;
            this.btnSoundbankSelector.Click += new System.EventHandler(this.btnSoundbankSelector_Click);
            // 
            // OptionsForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(342, 187);
            this.Controls.Add(this.btnSoundbankSelector);
            this.Controls.Add(this.tbSoundbankText);
            this.Controls.Add(this.lbSoundbank);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnOK);
            this.Name = "OptionsForm";
            this.Text = "Options";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnOK;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.Label lbSoundbank;
        private System.Windows.Forms.Button btnSoundbankSelector;
        public System.Windows.Forms.TextBox tbSoundbankText;
    }
}