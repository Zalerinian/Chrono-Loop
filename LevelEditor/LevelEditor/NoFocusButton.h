#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace LevelEditor {

	/// <summary>
	/// Summary for NoFocusButton
	/// </summary>
	public ref class NoFocusButton : public System::Windows::Forms::Button
	{
	public:
		NoFocusButton(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			this->SetStyle(ControlStyles::Selectable, false);
			this->MinimumSize = Drawing::Size(1, 1);
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~NoFocusButton()
		{
			if (components)
			{
				delete components;
			}
		}

		bool ShowFocusCues()
		{
			return false;
		}
	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = gcnew System::ComponentModel::Container();
		}
#pragma endregion
	};
}
