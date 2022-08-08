#include "ui/statebox.h"

reBaseStateBox::reBaseStateBox(wxWindow *parent, const wxString& label)
    : wxStaticBox(parent, wxID_ANY, label)
{
  SetStaticBoxSizer(new wxStaticBoxSizer(this, wxVERTICAL));
};

reRegisterStateBox::reRegisterStateBox(wxWindow *parent)
    : reBaseStateBox(parent, "Registers")
{
  wxFlexGridSizer* flex_sizer = new wxFlexGridSizer(3);

  A_ = new reRegisterPanel(this, "A");
  B_ = new reRegisterPanel(this, "B");
  C_ = new reRegisterPanel(this, "C");
  D_ = new reRegisterPanel(this, "D");
  M_ = new reRegisterPanel(this, "M");
  S_ = new reRegisterPanel(this, "S");
  L_ = new reRegisterPanel(this, "L");
  PC_ = new reRegisterPanel(this, "PC");

  flex_sizer->Add(A_, 0, wxLEFT | wxRIGHT, 15);
  flex_sizer->Add(B_, 0, wxLEFT | wxRIGHT, 15);
  flex_sizer->Add(C_, 0, wxLEFT | wxRIGHT, 15);
  flex_sizer->Add(D_, 0, wxLEFT | wxRIGHT, 15);
  flex_sizer->Add(M_, 0, wxLEFT | wxRIGHT, 15);
  flex_sizer->Add(S_, 0, wxLEFT | wxRIGHT, 15);
  flex_sizer->Add(L_, 0, wxLEFT | wxRIGHT, 15);
  flex_sizer->Add(PC_, 0, wxLEFT | wxRIGHT, 15);
  sizer_->Add(flex_sizer);
}

reFlagStateBox::reFlagStateBox(wxWindow *parent)
    : reBaseStateBox(parent, "Flags")
{
  wxFlexGridSizer* flex_sizer = new wxFlexGridSizer(3);

  S_ = new reFlagPanel(this, "S");
  Z_ = new reFlagPanel(this, "Z");
  CY_ = new reFlagPanel(this, "CY");

  flex_sizer->Add(S_, 0, wxLEFT | wxRIGHT, 15);
  flex_sizer->Add(Z_, 0, wxLEFT | wxRIGHT, 15);
  flex_sizer->Add(CY_, 0, wxLEFT | wxRIGHT, 15);
  sizer_->Add(flex_sizer);
}

reMemoryStateBox::reMemoryStateBox(wxWindow *parent)
    : reBaseStateBox(parent, "Memory")
{
  wxFlexGridSizer* flex_sizer = new wxFlexGridSizer(3);

  for (int addr = 0; addr < program_.size(); ++addr)
  {
    program_[addr] = new reMemoryPanel<16>(this, addr);
    flex_sizer->Add(program_[addr], 0, wxLEFT | wxRIGHT, 15);
  }

  for (int addr = 0; addr < input_.size(); ++addr)
  {
    int offset = program_.size();

    input_[addr] = new reMemoryPanel<8>(this, addr + offset);
    flex_sizer->Add(input_[addr], 0, wxLEFT | wxRIGHT, 15);
  }

  for (int addr = 0; addr < unused_.size(); ++addr)
  {
    int offset = program_.size() + input_.size();

    unused_[addr] = new reMemoryPanel<8>(this, addr + offset);
    flex_sizer->Add(unused_[addr], 0, wxLEFT | wxRIGHT, 15);
  }

  sizer_->Add(flex_sizer);
}
