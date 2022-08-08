#include <sstream>

#include "ui/panels.h"

reRegisterPanel::reRegisterPanel(wxWindow *parent, const wxString& register_name)
    : reBasePanel(parent)
{
  wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

  register_name_ = new wxStaticText(this, wxID_ANY, register_name,
                                    wxDefaultPosition, wxSize(30, 30));
  value_ = new wxStaticText(this, wxID_ANY, "00000000");
  as_unsigned_ = new wxStaticText(this, wxID_ANY, "0", wxDefaultPosition,
                                  wxSize(20, 30));
  as_signed_ = new wxStaticText(this, wxID_ANY, "0", wxDefaultPosition,
                                wxSize(20, 30));

  sizer->Add(register_name_);
  sizer->Add(value_);
  sizer->AddSpacer(15);
  sizer->Add(as_unsigned_);
  sizer->AddSpacer(15);
  sizer->Add(as_signed_);

  SetSizer(sizer);
}

void reRegisterPanel::SetValue(int value)
{
  value_->SetLabel(std::bitset<8>(value).to_string());
  as_unsigned_->SetLabel(std::to_string(static_cast<uint8_t>(value)));
  as_signed_->SetLabel(std::to_string(static_cast<int8_t>(value)));
}

reFlagPanel::reFlagPanel(wxWindow *parent, const wxString& flag_name)
    : reBasePanel(parent)
{
  wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

  flag_name_ = new wxStaticText(this, wxID_ANY, flag_name,
                                    wxDefaultPosition, wxSize(30, 30));
  value_ = new wxStaticText(this, wxID_ANY, "0");

  sizer->Add(flag_name_);
  sizer->Add(value_);

  SetSizer(sizer);
}

void reFlagPanel::SetValue(int value)
{
  value_->SetLabel(std::bitset<1>(value).to_string());
}
