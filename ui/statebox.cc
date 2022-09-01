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
  CreateSubSizers();

  A_ = new reRegisterStateRow(this, "A", left_label_column_,
                              left_first_nibble_column_,
                              left_second_nibble_column_, left_hex_column_,
                              left_signed_column_, left_unsigned_column_);
  B_ = new reRegisterStateRow(this, "B", left_label_column_,
                              left_first_nibble_column_,
                              left_second_nibble_column_, left_hex_column_,
                              left_signed_column_, left_unsigned_column_);
  C_ = new reRegisterStateRow(this, "C", left_label_column_,
                              left_first_nibble_column_,
                              left_second_nibble_column_, left_hex_column_,
                              left_signed_column_, left_unsigned_column_);
  D_ = new reRegisterStateRow(this, "D", left_label_column_,
                              left_first_nibble_column_,
                              left_second_nibble_column_, left_hex_column_,
                              left_signed_column_, left_unsigned_column_);
  M_ = new reRegisterStateRow(this, "M", right_label_column_,
                              right_first_nibble_column_,
                              right_second_nibble_column_, right_hex_column_,
                              right_signed_column_, right_unsigned_column_);
  S_ = new reRegisterStateRow(this, "S", right_label_column_,
                              right_first_nibble_column_,
                              right_second_nibble_column_, right_hex_column_,
                              right_signed_column_, right_unsigned_column_);
  L_ = new reRegisterStateRow(this, "L", right_label_column_,
                              right_first_nibble_column_,
                              right_second_nibble_column_, right_hex_column_,
                              right_signed_column_, right_unsigned_column_);
  PC_ = new reRegisterStateRow(this, "PC", right_label_column_,
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

  GetStaticBoxSizer()->Add(left_sizer, 0, wxLEFT, 15);
  GetStaticBoxSizer()->Add(right_sizer, 0);
}

reFlagStateBox::reFlagStateBox(wxWindow *parent)
    : reBaseStateBox(parent, "Flags", wxHORIZONTAL)
{
  CreateRows();
}

void reFlagStateBox::CreateRows()
{
  S_ = new reFlagStateRow(this, "S", label_column_, value_column_);;
  Z_ = new reFlagStateRow(this, "Z", label_column_, value_column_);;
  CY_ = new reFlagStateRow(this, "CY", label_column_, value_column_);;

  GetStaticBoxSizer()->Add(label_column_, 1, wxEXPAND | wxLEFT, 15);
  GetStaticBoxSizer()->Add(value_column_, 1, wxRIGHT, 15);
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

  int addr = 0;
  for (addr; addr < ROM::kProgramDataSize; ++addr)
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
  for (addr; addr < ROM::kProgramDataSize + ROM::kInputSwitchesSize; ++addr)
  {
    int index = addr - program_.size();
    input_[index] = new reInputSwitchesStateRow(this, addr, addr_column_,
                                                ls_byte_ms_nibble_column_,
                                                ls_byte_ls_nibble_column_,
                                                ls_byte_hex_column_,
                                                ms_byte_ms_nibble_column_,
                                                ms_byte_ls_nibble_column_,
                                                ms_byte_hex_column_,
                                                disassembled_column_);
  }
  for (addr; addr < kROMSize; ++addr)
  {
    int index = addr - program_.size() - input_.size();
    new reUnusedStateRow(this, addr, addr_column_, ls_byte_ms_nibble_column_,
                        ls_byte_ls_nibble_column_, ls_byte_hex_column_,
                        ms_byte_ms_nibble_column_, ms_byte_ls_nibble_column_,
                        ms_byte_hex_column_, disassembled_column_);
  }

  const int margin = GetFont().GetPixelSize().GetX();
  GetStaticBoxSizer()->Add(addr_column_, 0, wxEXPAND | wxLEFT, 15);
  GetStaticBoxSizer()->Add(ms_byte_ms_nibble_column_, 1, wxEXPAND);
  GetStaticBoxSizer()->Add(ms_byte_ls_nibble_column_, 0, wxLEFT, margin);
  GetStaticBoxSizer()->Add(ls_byte_ms_nibble_column_, 0, wxLEFT, margin);
  GetStaticBoxSizer()->Add(ls_byte_ls_nibble_column_, 0, wxLEFT, margin);
  GetStaticBoxSizer()->Add(ms_byte_hex_column_, 1, wxEXPAND);
  GetStaticBoxSizer()->Add(ls_byte_hex_column_, 0, wxLEFT, GetFont().GetPixelSize().GetX());
  GetStaticBoxSizer()->Add(disassembled_column_, 1, wxRIGHT, 15);
  GetStaticBoxSizer()->AddStretchSpacer();
}

void reROMStateBox::SetProgramDataValues(
    const std::array<uint16_t, ROM::kProgramDataSize>& values)
{
  for (int i = 0; i < ROM::kProgramDataSize; ++i)
  {
    program_[i]->SetValue(values[i]);
  }
}

void reROMStateBox::SetInputSwitchesValues(
    const std::array<uint8_t, ROM::kInputSwitchesSize>& values)
{
  for (int i = 0; i < ROM::kInputSwitchesSize; ++i)
  {
    input_[i]->SetValue(values[i]);
  }
}
