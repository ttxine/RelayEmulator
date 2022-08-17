#include "inputdialog.h"

wxBEGIN_EVENT_TABLE(reInputDialog, wxDialog)
  EVT_BUTTON(wxID_APPLY, reInputDialog::OnApply)
wxEND_EVENT_TABLE()

void reInputDialog::CreateControls()
{
  wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

  wxIntegerValidator<int> first_val(&first_);
  wxIntegerValidator<int> second_val(&second_);
  
  first_val.SetMin(-128);
  first_val.SetMax(255);
  second_val.SetMin(-128);
  second_val.SetMax(255);

  first_input_ = new wxTextCtrl(this, wxID_ANY, "0", wxDefaultPosition,
                                wxDefaultSize, 0L, first_val, "0x80");
  second_input_ = new wxTextCtrl(this, wxID_ANY, "0", wxDefaultPosition,
                                  wxDefaultSize, 0L, second_val, "0x81");

  int input_height;
  first_input_->GetSize(nullptr, &input_height);
  first_input_->SetMinSize(wxSize(250, input_height));
  second_input_->SetMinSize(wxSize(250, input_height));

  sizer->AddSpacer(15);
  sizer->Add(new wxStaticText(this, wxID_ANY, "0x80"), 0, wxLEFT, 15);
  sizer->AddSpacer(5);
  sizer->Add(first_input_, 0, wxLEFT | wxRIGHT, 15);
  sizer->AddSpacer(15);
  sizer->Add(new wxStaticText(this, wxID_ANY, "0x81"), 0, wxLEFT, 15);
  sizer->AddSpacer(5);
  sizer->Add(second_input_, 0, wxLEFT | wxRIGHT, 15);
  sizer->AddSpacer(15);
  sizer->Add(CreateButtonSizer(wxAPPLY),
             wxSizerFlags().Align(wxALIGN_CENTER_HORIZONTAL));
  sizer->AddSpacer(15);

  SetSizerAndFit(sizer);
  SetMaxSize(GetSize());
}

void reInputDialog::OnApply(wxCommandEvent& event)
{
  if (Validate() && TransferDataFromWindow())
  {
    EndModal(wxID_APPLY);
  }
  else
  {
    SetReturnCode(wxID_CANCEL);
    Show(false);
  }
  event.Skip();
}
