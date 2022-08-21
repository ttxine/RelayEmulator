#pragma once
#include <wx/wx.h>
#include <wx/valnum.h>

class reInputDialog : public wxDialog
{
  public:
    reInputDialog(wxWindow* parent, uint8_t first_default = 0,
                  uint8_t second_default = 0)
        : wxDialog(parent, wxID_ANY, "Input", wxDefaultPosition, wxDefaultSize,
                   wxDEFAULT_DIALOG_STYLE),
          first_(first_default),
          second_(second_default)
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

    int first_;
    int second_;

    wxDECLARE_EVENT_TABLE();
};
