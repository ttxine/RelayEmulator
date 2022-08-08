#include "ui/app.h"

wxIMPLEMENT_APP(reRelayEmulatorApp);

bool reRelayEmulatorApp::OnInit()
{
  reMainForm* main_form = new reMainForm();
  main_form->Show();

  return true;
}
