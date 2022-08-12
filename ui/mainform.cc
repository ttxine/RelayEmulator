#include "ui/mainform.h"
#include "ui/inputdialog.h"

#include "ui/resources/run.xpm"
#include "ui/resources/step.xpm"
#include "ui/resources/load.xpm"
#include "ui/resources/compileandload.xpm"
#include "ui/resources/reset.xpm"
#include "ui/resources/input.xpm"

wxBEGIN_EVENT_TABLE(reMainForm, wxFrame)
  EVT_TOOL(ID::ID_LOAD, reMainForm::OnLoad)
  EVT_TOOL(ID::ID_COMPILE_AND_LOAD, reMainForm::OnCompileAndLoad)
  EVT_TOOL(ID::ID_RUN, reMainForm::OnRun)
  EVT_TOOL(ID::ID_STEP, reMainForm::OnStep)
  EVT_TOOL(ID::ID_RESET, reMainForm::OnReset)
  EVT_TOOL(ID::ID_INPUT, reMainForm::OnInput)
wxEND_EVENT_TABLE()

reMainForm::reMainForm()
    : wxFrame(nullptr, wxID_ANY, "RelayEmulator", wxDefaultPosition)
{
  CreateMenuBar();
  CreateToolBar();
  CreateControls();

  EnableRun(false);
  EnableStep(false);
  EnableInput(false);
  EnableReset(false);

  SetMinSize(wxSize(state_window_->GetSize().GetX(), 200));
  SetSize(wxSize(state_window_->GetSize().GetX(), 600));
}

void reMainForm::CreateMenuBar()
{
  run_menu_ = new wxMenu();
  run_menu_->Append(ID_RUN, "Run");
  run_menu_->Append(ID_STEP, "Step");

  load_menu_ = new wxMenu();
  load_menu_->Append(ID_LOAD, "Load");
  load_menu_->Append(ID_COMPILE_AND_LOAD, "Compile && Load");

  wxMenu* about_menu = new wxMenu();
  about_menu->Append(wxID_ABOUT, "GitHub");

  wxMenu* menus[3] = { run_menu_, load_menu_, about_menu };
  const wxString 	titles[3] = { wxString("Run"), wxString("Load"),
                                wxString("About") };

  SetMenuBar(new wxMenuBar(3, menus, titles));
}

wxToolBar* reMainForm::CreateToolBar(long style, wxWindowID winid,
                                     const wxString& name)
{
  wxToolBar* tool_bar = wxFrame::CreateToolBar(style, winid, name);

  tool_bar->AddTool(ID_RUN, "Run", wxBITMAP(run), "Run");
  tool_bar->AddTool(ID_STEP, "Step", wxBITMAP(step), "Step");
  tool_bar->AddTool(ID_RESET, "Reset", wxBITMAP(reset), "Reset");
  tool_bar->AddSeparator();
  tool_bar->AddTool(ID_LOAD, "Load", wxBITMAP(load), "Load");
  tool_bar->AddTool(ID_COMPILE_AND_LOAD, "Compile & Load",
                    wxBITMAP(compile_and_load), "Compile & Load");
  tool_bar->AddSeparator();
  tool_bar->AddTool(ID_INPUT, "Input", wxBITMAP(input), "Input");
  tool_bar->Realize();

  return tool_bar;
}

void reMainForm::CreateControls()
{
  state_window_ = new wxScrolledWindow(this);
  state_window_->SetScrollRate(0, 15);
  state_window_->SetBackgroundColour(wxColour(235, 231, 202, 10));

  wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

  registers_ = new reRegisterStateBox(state_window_);
  flags_ = new reFlagStateBox(state_window_);
  memory_ = new reMemoryStateBox(state_window_);

  sizer->Add(registers_->GetStaticBoxSizer(), 0, wxALL, 15);
  sizer->Add(flags_->GetStaticBoxSizer(), 0, wxALL, 15);
  sizer->Add(memory_->GetStaticBoxSizer(), 0, wxALL, 15);

  state_window_->SetSizerAndFit(sizer);
}

void reMainForm::EnableRun(bool enable)
{
  run_menu_->Enable(ID_RUN, enable);
  GetToolBar()->EnableTool(ID_RUN, enable);
}

void reMainForm::EnableStep(bool enable)
{
  run_menu_->Enable(ID_STEP, enable);
  GetToolBar()->EnableTool(ID_STEP, enable);
}

void reMainForm::EnableLoad(bool enable)
{
  load_menu_->Enable(ID_LOAD, enable);
  load_menu_->Enable(ID_COMPILE_AND_LOAD, enable);
  GetToolBar()->EnableTool(ID_LOAD, enable);
  GetToolBar()->EnableTool(ID_COMPILE_AND_LOAD, enable);
}

void reMainForm::EnableInput(bool enable)
{
  GetToolBar()->EnableTool(ID_INPUT, enable);
}

void reMainForm::EnableReset(bool enable)
{
  GetToolBar()->EnableTool(ID_RESET, enable);
}

void reMainForm::Update()
{
  Bus::DebugInfo info = emulator_.GetBusDebugInfo();

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
    emulator_.Load(path);
  }
  catch (const std::runtime_error& e)
  {
    Raise(e.what());
  }

  SetTitle("RelayEmulator - " + path);

  EnableRun();
  EnableStep();
  EnableInput();

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
  EnableStep(false);
  EnableLoad(false);
  EnableInput(false);
  emulator_.Run();

  Update();
  Reset();

  EnableStep();
  EnableLoad();
  EnableInput();

  event.Skip();
}

void reMainForm::OnStep(wxCommandEvent& event)
{
  EnableRun(false);
  EnableLoad(false);
  EnableInput(false);
  EnableReset();
  emulator_.Step();

  Update();
  event.Skip();
}

void reMainForm::OnReset(wxCommandEvent& event)
{
  Reset();

  EnableRun();
  EnableStep();
  EnableInput();
  EnableLoad();
  EnableReset(false);

  event.Skip();
}

void reMainForm::OnInput(wxCommandEvent& event)
{
  reInputDialog* input_dialog = new reInputDialog(this);

  if (input_dialog->ShowModal() == wxID_CANCEL)
  {
    return;
  }

  emulator_.Input(input_dialog->GetFirst(), input_dialog->GetSecond());

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
  emulator_.Reset();
}
