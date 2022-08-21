#pragma once
#include <thread>
#include <memory>

#include <wx/wx.h>

#include "ui/statebox.h"
#include "core/emulator.h"

class reMainForm : public wxFrame
{
  public:
    enum ID
    {
      ID_LOAD = wxID_HIGHEST + 1,
      ID_COMPILE_AND_LOAD,
      ID_RUN,
      ID_STEP,
      ID_INPUT,
      ID_STOP,
      ID_RESET
    };

    enum class State
    {
      kWaiting,
      kRunning,
      kStepping,
      kReseting,
      kClosing
    };

  public:
    const int kMillisecondsPerCycle = 2000;

  public:
    reMainForm();

  private:
    void Update();
    void Stop();
    void Load(const wxString& path);
    void Raise(const std::string& msg);

    void OnLoad(wxCommandEvent& event);
    void OnCompileAndLoad(wxCommandEvent& event);
    void OnRun(wxCommandEvent& event);
    void OnStep(wxCommandEvent& event);
    void OnStop(wxCommandEvent& event);
    void OnReset(wxCommandEvent& event);
    void OnInput(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);

    void EnableRun(bool enable = true);
    void EnableStep(bool enable = true);
    void EnableLoad(bool enable = true);
    void EnableInput(bool enable = true);
    void EnableStop(bool enable = true);
    void EnableReset(bool enable = true);

    void CreateControls();
    void CreateMenuBar();
    wxToolBar* CreateToolBar(long style = -1L, wxWindowID winid = wxID_ANY,
                             const wxString& name = wxToolBarNameStr);

    void RunEmulatorThread();

  private:
    State state_ = State::kWaiting;

    wxMenu* run_menu_;
    wxMenu* load_menu_;
    wxSlider* speed_slider_;
    wxScrolledWindow* state_window_;

    reRegisterStateBox* registers_;
    reFlagStateBox* flags_;
    reROMStateBox* rom_;

    std::array<uint8_t, 2> last_input_ = {};

    Emulator emulator_ = { true, true };
    std::thread background_thread_;

    wxDECLARE_EVENT_TABLE();
};
