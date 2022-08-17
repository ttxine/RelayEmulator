#include "ui/statebox.h"

reBaseStateBox::reBaseStateBox(wxWindow *parent, const wxString& label, int orient)
    : wxStaticBox(parent, wxID_ANY, label)
{
  SetStaticBoxSizer(new wxStaticBoxSizer(this, orient));
};

reRegisterStateBox::reRegisterStateBox(wxWindow *parent)
    : reBaseStateBox(parent, "Registers", wxHORIZONTAL)
{
  CreateRows();
}

void reRegisterStateBox::CreateRows()
{
  A_ = AddRowToLeft("A");
  B_ = AddRowToLeft("B");
  C_ = AddRowToLeft("C");
  D_ = AddRowToLeft("D");
  M_ = AddRowToRight("M");
  S_ = AddRowToRight("S");
  L_ = AddRowToRight("L");
  PC_ = AddRowToRight("PC");

  CreateSubSizers();
}

reRegisterStateRow* reRegisterStateBox::AddRowToLeft(const wxString& label)
{
  return new reRegisterStateRow(this, label, left_label_column_,
                                left_first_nibble_column_,
                                left_second_nibble_column_, left_hex_column_,
                                left_signed_column_, left_unsigned_column_);
}

reRegisterStateRow* reRegisterStateBox::AddRowToRight(const wxString& label)
{
  return new reRegisterStateRow(this, label, right_label_column_,
                                right_first_nibble_column_,
                                right_second_nibble_column_, right_hex_column_,
                                right_signed_column_, right_unsigned_column_);
}

void reRegisterStateBox::CreateSubSizers()
{
  wxBoxSizer* left_sizer = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* right_sizer = new wxBoxSizer(wxHORIZONTAL);

  const int margin = GetFont().GetPixelSize().GetX();

  left_sizer->Add(left_label_column_, 0, wxRIGHT, 15);
  left_sizer->Add(left_first_nibble_column_, 2, wxEXPAND);
  left_sizer->Add(left_second_nibble_column_, 2, wxLEFT, margin);
  left_sizer->Add(left_hex_column_, 2, wxEXPAND);
  left_sizer->Add(left_signed_column_, 2, wxEXPAND);
  left_sizer->Add(left_unsigned_column_, 2, wxEXPAND);
  right_sizer->Add(right_label_column_, 2, wxRIGHT, 15);
  right_sizer->Add(right_first_nibble_column_, 2, wxEXPAND);
  right_sizer->Add(right_second_nibble_column_, 2, wxLEFT, margin);
  right_sizer->Add(right_hex_column_, 2, wxEXPAND);
  right_sizer->Add(right_signed_column_, 2, wxEXPAND);
  right_sizer->Add(right_unsigned_column_, 2, wxEXPAND);

  left_sizer->AddStretchSpacer();
  right_sizer->AddStretchSpacer();

  sizer_->Add(left_sizer, 0, wxLEFT, 15);
  sizer_->Add(right_sizer, 0);
}

reFlagStateBox::reFlagStateBox(wxWindow *parent)
    : reBaseStateBox(parent, "Flags", wxHORIZONTAL)
{
  CreateRows();
}

void reFlagStateBox::CreateRows()
{
  S_ = AddRow("S");
  Z_ = AddRow("Z");
  CY_ = AddRow("CY");

  sizer_->Add(label_column_, 1, wxEXPAND | wxLEFT, 15);
  sizer_->Add(value_column_, 1, wxRIGHT, 15);
}

reFlagStateRow* reFlagStateBox::AddRow(const wxString& label)
{
  return new reFlagStateRow(this, label, label_column_, value_column_);
}

reROMStateBox::reROMStateBox(wxWindow *parent)
    : reBaseStateBox(parent, "ROM", wxHORIZONTAL)
{
  CreateRows();
}

void reROMStateBox::CreateRows()
{
  const int kROMSize = ROM::kProgramDataSize + ROM::kInputSwitchesSize +
                       ROM::kUnusedSize;
  for (int addr = 0; addr < kROMSize; ++addr)
  {
    AddRow(addr);
  }

  const int margin = GetFont().GetPixelSize().GetX();
  sizer_->Add(addr_column_, 0, wxEXPAND | wxLEFT, 15);
  sizer_->Add(ms_byte_ms_nibble_column_, 1, wxEXPAND);
  sizer_->Add(ms_byte_ls_nibble_column_, 0, wxLEFT, margin);
  sizer_->Add(ls_byte_ms_nibble_column_, 0, wxLEFT, margin);
  sizer_->Add(ls_byte_ls_nibble_column_, 0, wxLEFT, margin);
  sizer_->Add(ms_byte_hex_column_, 1, wxEXPAND);
  sizer_->Add(ls_byte_hex_column_, 0, wxLEFT, GetFont().GetPixelSize().GetX());
  sizer_->Add(disassembled_column_, 1, wxRIGHT, 15);
  sizer_->AddStretchSpacer();
}

void reROMStateBox::AddRow(uint8_t addr)
{
  if (addr < ROM::kProgramDataSize)
  {
    program_[addr] = new reProgramDataStateRow(this, addr, addr_column_,
                                               ls_byte_ms_nibble_column_,
                                               ls_byte_ls_nibble_column_,
                                               ls_byte_hex_column_,
                                               ms_byte_ms_nibble_column_,
                                               ms_byte_ls_nibble_column_,
                                               ms_byte_hex_column_,
                                               disassembled_column_);
  }
  else if (addr < ROM::kProgramDataSize + ROM::kInputSwitchesSize)
  {
    int index = addr - program_.size();
    input_[index] = new reInputSwitchesStateRow(this, addr , addr_column_,
                                                ls_byte_ms_nibble_column_,
                                                ls_byte_ls_nibble_column_,
                                                ls_byte_hex_column_);
    AddNull();
  }
  else
  {
    int index = addr - program_.size() - input_.size();
    unused_[index] = new reUnusedStateRow(this, addr, addr_column_,
                                          ls_byte_ms_nibble_column_,
                                          ls_byte_ls_nibble_column_,
                                          ls_byte_hex_column_);
    AddNull();
  }
}

void reROMStateBox::AddNull()
{
  int margin = GetFont().GetPixelSize().GetY() / 2;

  ms_byte_ms_nibble_column_->Add(new wxStaticText(this, wxID_ANY, "----"), 0,
                                 wxALIGN_RIGHT);
  ms_byte_ls_nibble_column_->Add(new wxStaticText(this, wxID_ANY, "----"), 0,
                                 wxALIGN_RIGHT);
  ms_byte_hex_column_->Add(new wxStaticText(this, wxID_ANY, "--"), 0,
                           wxALIGN_RIGHT);
  disassembled_column_->Add(new wxStaticText(this, wxID_ANY, "---"), 0,
                            wxALIGN_RIGHT);

  ms_byte_ms_nibble_column_->AddSpacer(margin);
  ms_byte_ls_nibble_column_->AddSpacer(margin);
  ms_byte_hex_column_->AddSpacer(margin);
  disassembled_column_->AddSpacer(margin);
}
