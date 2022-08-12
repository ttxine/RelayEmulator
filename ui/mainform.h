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
      ID_STEP,
      ID_INPUT,
      ID_RESET
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
    void OnReset(wxCommandEvent& event);
    void OnRealTimeRun(wxCommandEvent& event);
    void OnInput(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    void EnableRun(bool enable = true);
    void EnableStep(bool enable = true);
    void EnableLoad(bool enable = true);
    void EnableInput(bool enable = true);
    void EnableReset(bool enable = true);

    void CreateControls();
    void CreateMenuBar();
    wxToolBar* CreateToolBar(long style = -1L, wxWindowID winid = wxID_ANY,
                             const wxString& name = wxToolBarNameStr);

  private:
    wxMenu* run_menu_;
    wxMenu* load_menu_;
    wxScrolledWindow* state_window_;

    reRegisterStateBox* registers_;
    reFlagStateBox* flags_;
    reMemoryStateBox* memory_;

    Emulator emulator_ = { true, true };

    wxDECLARE_EVENT_TABLE();
};
