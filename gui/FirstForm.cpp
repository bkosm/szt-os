#include "FirstForm.h"
#include "../src/Definitions.hpp"
#include "../src/Lock.hpp"

using namespace System;

[STAThreadAttribute]
int main() {
	Lock lock();

	System::Windows::Forms::Application::Run(gcnew gui::FirstForm());
	return 0;
}