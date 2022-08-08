#pragma once
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
      ID_STEP
    };

  public:
    reMainForm();

  private:
    void Update();
    void Reset();
    void Raise(const std::string& msg);

    void OnLoad(wxCommandEvent& event);
    void OnCompileAndLoad(wxCommandEvent& event);
    void OnRun(wxCommandEvent& event);
    void OnStep(wxCommandEvent& event);
    void OnRealTimeRun(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    void EnableRunMenu(bool enable = true);

    void CreateControls();
    void CreateMenuBar();

  private:
    wxMenu* run_menu_;
    wxScrolledWindow* state_window_;

    reRegisterStateBox* registers_;
    reFlagStateBox* flags_;
    reMemoryStateBox* memory_;

    std::unique_ptr<Emulator> emulator_;
};
