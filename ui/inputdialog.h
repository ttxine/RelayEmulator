#pragma once
#include <wx/wx.h>
#include <wx/valnum.h>

class reInputDialog : public wxDialog
{
  public:
    reInputDialog(wxWindow* parent)
        : wxDialog(parent, wxID_ANY, "Input", wxDefaultPosition, wxDefaultSize,
                   wxDEFAULT_DIALOG_STYLE)
    {
      CreateControls();
    }

  public:
    uint8_t GetFirst() const { return first_; }
    uint8_t GetSecond() const { return second_; }

  private:
    void CreateControls();
    void OnApply(wxCommandEvent& event);

  private:
    wxTextCtrl* first_input_;
    wxTextCtrl* second_input_;

    int first_ = 0;
    int second_ = 0;

    wxDECLARE_EVENT_TABLE();
};
