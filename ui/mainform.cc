#include "ui/mainform.h"

reMainForm::reMainForm()
    : wxFrame(nullptr, wxID_ANY, "RelayEmulator", wxDefaultPosition)
{
  CreateMenuBar();
  CreateControls();

  SetMinSize(wxSize(state_window_->GetSize().GetX(), 200));
  SetSize(wxSize(state_window_->GetSize().GetX(), 600));
}

void reMainForm::CreateMenuBar()
{
  run_menu_ = new wxMenu();
  run_menu_->Append(ID_RUN, "Run");
  run_menu_->Append(ID_STEP, "Step");
  Bind(wxEVT_MENU, &reMainForm::OnRun, this, ID_RUN);
  Bind(wxEVT_MENU, &reMainForm::OnStep, this, ID_STEP);

  wxMenu* load_menu = new wxMenu();
  load_menu->Append(ID_LOAD, "Load");
  load_menu->Append(ID_COMPILE_AND_LOAD, "Compile && Load");
  Bind(wxEVT_MENU, &reMainForm::OnLoad, this, ID_LOAD);
  Bind(wxEVT_MENU, &reMainForm::OnCompileAndLoad, this, ID_COMPILE_AND_LOAD);

  wxMenu* about_menu = new wxMenu();
  about_menu->Append(wxID_ABOUT, "GitHub");
  Bind(wxEVT_MENU, &reMainForm::OnAbout, this, wxID_ABOUT);

  wxMenu* menus[3] = { run_menu_, load_menu, about_menu };
  const wxString 	titles[3] = { wxString("Run"), wxString("Load"),
                                wxString("About") };

  SetMenuBar(new wxMenuBar(3, menus, titles));
  EnableRunMenu(false);
}

void reMainForm::CreateControls()
{
  state_window_ = new wxScrolledWindow(this);
  state_window_->SetScrollRate(0, 5);

  wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

  registers_ = new reRegisterStateBox(state_window_);
  flags_ = new reFlagStateBox(state_window_);
  memory_ = new reMemoryStateBox(state_window_);

  sizer->Add(registers_->GetStaticBoxSizer(), 0, wxALL, 15);
  sizer->Add(flags_->GetStaticBoxSizer(), 0, wxALL, 15);
  sizer->Add(memory_->GetStaticBoxSizer(), 0, wxALL, 15);

  state_window_->SetSizerAndFit(sizer);
}

void reMainForm::EnableRunMenu(bool enable)
{
  run_menu_->Enable(ID_RUN, enable);
  run_menu_->Enable(ID_STEP, enable);
}

void reMainForm::Update()
{
  Bus::DebugInfo info = emulator_->GetBusDebugInfo();

  registers_->SetAValue(info.r_A);
  registers_->SetBValue(info.r_B);
  registers_->SetCValue(info.r_C);
  registers_->SetDValue(info.r_D);
  registers_->SetMValue(info.r_M);
  registers_->SetSValue(info.r_S);
  registers_->SetLValue(info.r_L);
  registers_->SetPCValue(info.r_PC);

  flags_->SetSValue(info.f_S);
  flags_->SetZValue(info.f_Z);
  flags_->SetCYValue(info.f_CY);

  memory_->SetProgramDataValues(info.m_program_data);
  memory_->SetInputSwitchesValues(info.m_input_switches);
  memory_->SetUnusedValues(info.m_unused);
}

void reMainForm::OnLoad(wxCommandEvent& event)
{
  wxFileDialog file_dialog(this, "Open binary file with program", "", "", "",
                           wxFD_OPEN | wxFD_FILE_MUST_EXIST);

  if (file_dialog.ShowModal() == wxID_CANCEL)
  {
    return;
  }

  std::string path = file_dialog.GetPath().ToStdString();
  try
  {
    emulator_ = std::unique_ptr<Emulator>(
        new Emulator(path, false, { 15, 63 }, true));
  }
  catch (const std::runtime_error& e)
  {
    Raise(e.what());
  }

  EnableRunMenu(true);
  Update();
  event.Skip();
}

void reMainForm::OnCompileAndLoad(wxCommandEvent& event)
{
  OnLoad(event);
}

void reMainForm::Raise(const std::string& msg)
{
  wxMessageDialog err_msg(nullptr, "Error: " + msg, "Error",
                          wxICON_ERROR | wxCANCEL);

  err_msg.ShowModal();
  Reset();
}

void reMainForm::OnRun(wxCommandEvent& event)
{
  emulator_->Run();

  Update();
  event.Skip();
}

void reMainForm::OnStep(wxCommandEvent& event)
{
  emulator_->Step();

  Update();
  event.Skip();
}

void reMainForm::OnAbout(wxCommandEvent& event)
{
  wxLaunchDefaultBrowser("https://github.com/ttxine/RelayEmulator");
  event.Skip();
}

void reMainForm::Reset()
{
}
