#include "ui/mainform.h"
#include "ui/inputdialog.h"
#include "compiler/run.h"

#include "ui/resources/run.xpm"
#include "ui/resources/step.xpm"
#include "ui/resources/load.xpm"
#include "ui/resources/compileandload.xpm"
#include "ui/resources/stop.xpm"
#include "ui/resources/reset.xpm"
#include "ui/resources/input.xpm"

wxBEGIN_EVENT_TABLE(reMainForm, wxFrame)
  EVT_TOOL(ID::ID_LOAD, reMainForm::OnLoad)
  EVT_TOOL(ID::ID_COMPILE_AND_LOAD, reMainForm::OnCompileAndLoad)
  EVT_TOOL(ID::ID_RUN, reMainForm::OnRun)
  EVT_TOOL(ID::ID_STEP, reMainForm::OnStep)
  EVT_TOOL(ID::ID_STOP, reMainForm::OnStop)
  EVT_TOOL(ID::ID_RESET, reMainForm::OnReset)
  EVT_TOOL(ID::ID_INPUT, reMainForm::OnInput)
  EVT_CLOSE(reMainForm::OnClose)
wxEND_EVENT_TABLE()

reMainForm::reMainForm()
    : wxFrame(nullptr, wxID_ANY, "RelayEmulator", wxDefaultPosition)
{
  SetFont(wxFont(11, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL,
                 wxFONTWEIGHT_NORMAL));

  CreateMenuBar();
  CreateToolBar();
  CreateControls();

  EnableRun(false);
  EnableStep(false);
  EnableInput(false);
  EnableStop(false);
  EnableReset(false);

  SetMinSize(wxSize(700, 200));
  SetMaxSize(wxSize(700, GetMaxSize().GetY()));
  SetSize(wxSize(700, 800));
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
  speed_slider_ = new wxSlider(tool_bar, wxID_ANY, 1, 1, 20, wxDefaultPosition,
                               wxSize(128, 16));

  tool_bar->AddTool(ID_RUN, "Run", wxBITMAP(run), "Run");
  tool_bar->AddTool(ID_STEP, "Step", wxBITMAP(step), "Step");
  tool_bar->AddTool(ID_STOP, "Stop", wxBITMAP(stop), "Stop");
  tool_bar->AddTool(ID_RESET, "Reset", wxBITMAP(reset), "Reset");
  tool_bar->AddSeparator();
  tool_bar->AddTool(ID_LOAD, "Load", wxBITMAP(load), "Load");
  tool_bar->AddTool(ID_COMPILE_AND_LOAD, "Compile & Load",
                    wxBITMAP(compile_and_load), "Compile & Load");
  tool_bar->AddSeparator();
  tool_bar->AddTool(ID_INPUT, "Input", wxBITMAP(input), "Input");
  tool_bar->AddSeparator();
  tool_bar->AddControl(speed_slider_, "Speed");

  tool_bar->Realize();
  return tool_bar;
}

void reMainForm::CreateControls()
{
  state_window_ = new wxScrolledWindow(this);
  state_window_->SetScrollRate(0, 15);
  state_window_->SetBackgroundColour(wxColour(235, 231, 202, 10));

  wxBoxSizer* state_sizer = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer* reg_and_flag_sizer = new wxBoxSizer(wxHORIZONTAL);

  registers_ = new reRegisterStateBox(state_window_);
  flags_ = new reFlagStateBox(state_window_);
  rom_ = new reROMStateBox(state_window_);

  reg_and_flag_sizer->Add(registers_->GetStaticBoxSizer(), 1, wxEXPAND);
  reg_and_flag_sizer->Add(flags_->GetStaticBoxSizer(), 0, wxLEFT, 15);
  state_sizer->Add(reg_and_flag_sizer, 0, wxALL | wxEXPAND, 15);
  state_sizer->Add(rom_->GetStaticBoxSizer(), 0, wxALL | wxEXPAND, 15);

  state_window_->SetSizerAndFit(state_sizer);
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

void reMainForm::EnableStop(bool enable)
{
  GetToolBar()->EnableTool(ID_STOP, enable);
}

void reMainForm::RunEmulatorThread()
{
  auto FinalAction = [this]() { background_thread_.join(); Stop(); };

  while (!emulator_.Stopped())
  {
    std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(
        kMillisecondsPerCycle / speed_slider_->GetValue()));

    emulator_.Step();
    CallAfter([this]() { Update(); });

    if (state_ == State::kClosing)
    {
      CallAfter([this]() { background_thread_.join(); Destroy(); });
      return;
    }
    else if (state_ == State::kStopping)
    {
      CallAfter(FinalAction);
      return;
    }
  }

  CallAfter(FinalAction);
}

void reMainForm::Update()
{
  Bus::DebugInfo info = emulator_.GetDebugInfo();

  registers_->SetAValue(info.registers.A);
  registers_->SetBValue(info.registers.B);
  registers_->SetCValue(info.registers.C);
  registers_->SetDValue(info.registers.D);
  registers_->SetMValue(info.registers.M);
  registers_->SetSValue(info.registers.S);
  registers_->SetLValue(info.registers.L);
  registers_->SetPCValue(info.registers.PC);

  flags_->SetSValue(info.flags.S);
  flags_->SetZValue(info.flags.Z);
  flags_->SetCYValue(info.flags.CY);

  rom_->SetProgramDataValues(info.memory.program_data);
  rom_->SetInputSwitchesValues(info.memory.input_switches);
}

void reMainForm::Stop()
{
  emulator_.Reset();
  EnableRun();
  EnableStep();
  EnableInput();
  EnableLoad();
  EnableReset();
  EnableStop(false);

  state_ = State::kWaiting;
}

void reMainForm::Load(const wxString& path)
{
  try
  {
    emulator_.Load(path.ToStdString());
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

  last_input_ = { 0, 0 };
}

void reMainForm::Raise(const std::string& msg)
{
  wxMessageDialog err_msg(nullptr, "Error: " + msg, "Error",
                          wxICON_ERROR | wxCANCEL);

  err_msg.ShowModal();

  if (state_ == State::kRunning)
  {
    Stop();
  }
}

void reMainForm::OnLoad(wxCommandEvent& event)
{
  wxFileDialog file_dialog(this, "Open binary file with program", "", "", "",
                           wxFD_OPEN | wxFD_FILE_MUST_EXIST);

  if (!(file_dialog.ShowModal() == wxID_CANCEL))
  {
    Load(file_dialog.GetPath());
  }

  event.Skip();
}

void reMainForm::OnCompileAndLoad(wxCommandEvent& event)
{
  wxFileDialog file_dialog(this, "Open assembler source file", "", "",
                           "ASM files (*.s;*.asm;*.S)|*.s;*.asm;*.S",
                           wxFD_OPEN | wxFD_FILE_MUST_EXIST);

  if (!(file_dialog.ShowModal() == wxID_CANCEL))
  {
    try
    {
      TemporaryFile compiled = run_compiler(
          file_dialog.GetPath().ToStdString());
      compiled.Close();

      Load(compiled.GetPath());
    }
    catch (const std::runtime_error& e)
    {
      Raise(e.what());
    }
  }

  event.Skip();
}

void reMainForm::OnRun(wxCommandEvent& event)
{
  EnableRun(false);
  EnableStep(false);
  EnableLoad(false);
  EnableInput(false);
  EnableReset(false);
  EnableStop();

  state_ = State::kRunning;
  background_thread_ = std::thread(&reMainForm::RunEmulatorThread, this);

  event.Skip();
}

void reMainForm::OnStep(wxCommandEvent& event)
{
  EnableRun(false);
  EnableLoad(false);
  EnableInput(false);
  EnableReset(false);
  EnableStop();

  state_ = State::kStepping;
  emulator_.Step();

  Update();
  if (emulator_.Stopped())
  {
    Stop();
  }

  event.Skip();
}

void reMainForm::OnStop(wxCommandEvent& event)
{
  if (state_ == State::kRunning)
  {
    state_ = State::kStopping;
  }
  else if (state_ == State::kStepping)
  {
    Stop();
  }

  event.Skip();
}

void reMainForm::OnReset(wxCommandEvent& event)
{
  Update();
  EnableReset(false);

  event.Skip();
}

void reMainForm::OnInput(wxCommandEvent& event)
{
  reInputDialog* input_dialog = new reInputDialog(this, last_input_[0],
                                                  last_input_[1]);

  if (!(input_dialog->ShowModal() == wxID_CANCEL))
  {
    emulator_.Input(input_dialog->GetFirst(), input_dialog->GetSecond());
    Update();
    EnableReset(false);

    last_input_[0] = input_dialog->GetFirst();
    last_input_[1] = input_dialog->GetSecond();
  }

  event.Skip();
}

void reMainForm::OnAbout(wxCommandEvent& event)
{
  wxLaunchDefaultBrowser("https://github.com/Dovgalyuk/Relay");
  event.Skip();
}

void reMainForm::OnClose(wxCloseEvent& event)
{
  if (state_ == State::kRunning)
  {
    event.Veto();
    state_ = State::kClosing;
  }
  else
  {
    Destroy();
  }
}
