#include <bitset>

#include "ui/staterow.h"
#include "ui/statebox.h"
#include "core/rom.h"
#include "core/disassembler.h"
#include "utils/str.h"

reRegisterStateRow::reRegisterStateRow(reRegisterStateBox* box,
                                       const wxString& label,
                                       wxSizer* label_column,
                                       wxSizer* first_nibble_column,
                                       wxSizer* second_nibble_column,
                                       wxSizer* hex_column,
                                       wxSizer* signed_column,
                                       wxSizer* unsigned_column)
{
  label_ = new wxStaticText(box, wxID_ANY, label);
  first_nibble_ = new wxStaticText(box, wxID_ANY, "0000");
  second_nibble_ = new wxStaticText(box, wxID_ANY, "0000");
  hex_ = new wxStaticText(box, wxID_ANY, "00");
  unsigned_ = new wxStaticText(box, wxID_ANY, "0");
  signed_ = new wxStaticText(box, wxID_ANY, "0");

  wxFont label_font = label_->GetFont();
  label_font.SetWeight(wxFONTWEIGHT_BOLD);
  label_->SetFont(label_font);

  const int margin = box->GetFont().GetPixelSize().GetY() / 2;
  label_column->Add(label_, 0, wxALIGN_RIGHT | wxBOTTOM, margin);
  first_nibble_column->Add(first_nibble_, 0, wxALIGN_RIGHT | wxBOTTOM,
                           margin);
  second_nibble_column->Add(second_nibble_, 0, wxALIGN_RIGHT | wxBOTTOM,
                            margin);
  hex_column->Add(hex_, 0, wxALIGN_RIGHT | wxBOTTOM, margin);
  signed_column->Add(signed_, 0, wxALIGN_RIGHT | wxBOTTOM, margin);
  unsigned_column->Add(unsigned_, 0, wxALIGN_RIGHT | wxBOTTOM, margin);
}

void reRegisterStateRow::SetValue(uint8_t val)
{
  first_nibble_->SetLabel(std::bitset<4>(val >> 4).to_string());
  second_nibble_->SetLabel(std::bitset<4>(val & 0x0F).to_string());
  hex_->SetLabel(to_hex_string(val, 2));
  signed_->SetLabel(std::to_string(static_cast<int8_t>(val)));
  unsigned_->SetLabel(std::to_string(val));
}

reFlagStateRow::reFlagStateRow(reFlagStateBox* box,
                               const wxString& label,
                               wxSizer* label_column,
                               wxSizer* value_column)
{
  label_ = new wxStaticText(box, wxID_ANY, label);
  wxFont label_font = label_->GetFont();
  label_font.SetWeight(wxFONTWEIGHT_BOLD);
  label_->SetFont(label_font);
  value_ = new wxStaticText(box, wxID_ANY, "0");

  label_column->Add(label_, 0, wxALIGN_RIGHT);
  value_column->Add(value_, 0, wxALIGN_RIGHT);

  const int margin = box->GetFont().GetPixelSize().GetY() / 2;
  label_column->AddSpacer(margin);
  value_column->AddSpacer(margin);
}

void reFlagStateRow::SetValue(bool val)
{
  value_->SetLabel(std::to_string(val));
}

reROMStateRow::reROMStateRow(reROMStateBox* box, int addr,
                             wxSizer* addr_column)
{
  addr_ = new wxStaticText(box, wxID_ANY, to_hex_string(addr, 2));
  wxFont addr_font = addr_->GetFont();
  addr_font.SetWeight(wxFONTWEIGHT_BOLD);
  addr_->SetFont(addr_font);

  addr_column->Add(addr_, 0, wxALIGN_RIGHT);

  const int margin = box->GetFont().GetPixelSize().GetY() / 2;
  addr_column->AddSpacer(margin);
}

reProgramDataStateRow::reProgramDataStateRow(reROMStateBox* box,
                                             int addr,
                                             wxSizer* addr_column,
                                             wxSizer* ls_byte_ms_nibble_column,
                                             wxSizer* ls_byte_ls_nibble_column,
                                             wxSizer* ls_byte_hex_column,
                                             wxSizer* ms_byte_ms_nibble_column,
                                             wxSizer* ms_byte_ls_nibble_column,
                                             wxSizer* ms_byte_hex_column,
                                             wxSizer* disassembled_column)
    : reROMStateRow(box, addr, addr_column)
{
  ls_byte_ms_nibble_ = new wxStaticText(box, wxID_ANY, "0000");
  ls_byte_ls_nibble_ = new wxStaticText(box, wxID_ANY, "0000");
  ls_byte_hex_ = new wxStaticText(box, wxID_ANY, "00");
  ms_byte_ms_nibble_ = new wxStaticText(box, wxID_ANY, "0000");
  ms_byte_ls_nibble_ = new wxStaticText(box, wxID_ANY, "0000");
  ms_byte_hex_ = new wxStaticText(box, wxID_ANY, "00");
  disassembled_ = new wxStaticText(box, wxID_ANY, "NOP");

  ls_byte_ms_nibble_column->Add(ls_byte_ms_nibble_, 0, wxALIGN_RIGHT);
  ls_byte_ls_nibble_column->Add(ls_byte_ls_nibble_, 0, wxALIGN_RIGHT);
  ls_byte_hex_column->Add(ls_byte_hex_, 0, wxALIGN_RIGHT);
  ms_byte_ms_nibble_column->Add(ms_byte_ms_nibble_, 0, wxALIGN_RIGHT);
  ms_byte_ls_nibble_column->Add(ms_byte_ls_nibble_, 0, wxALIGN_RIGHT);
  ms_byte_hex_column->Add(ms_byte_hex_, 0, wxALIGN_RIGHT);
  disassembled_column->Add(disassembled_, 0, wxALIGN_RIGHT);

  const int margin = box->GetFont().GetPixelSize().GetY() / 2;
  ls_byte_ms_nibble_column->AddSpacer(margin);
  ls_byte_ls_nibble_column->AddSpacer(margin);
  ls_byte_hex_column->AddSpacer(margin);
  ms_byte_ms_nibble_column->AddSpacer(margin);
  ms_byte_ls_nibble_column->AddSpacer(margin);
  ms_byte_hex_column->AddSpacer(margin);
  disassembled_column->AddSpacer(margin);
}

void reProgramDataStateRow::SetValue(uint16_t val)
{
  uint8_t ls_byte_val = static_cast<uint8_t>(val);
  ls_byte_hex_->SetLabel(to_hex_string(ls_byte_val, 2));
  ls_byte_ls_nibble_->SetLabel(
      std::bitset<4>(ls_byte_val & 0x0F).to_string());
  ls_byte_ms_nibble_->SetLabel(
      std::bitset<4>(ls_byte_val >> 4).to_string());

  uint8_t ms_byte_val = static_cast<uint8_t>(val >> 8);
  ms_byte_hex_->SetLabel(to_hex_string(ms_byte_val, 2));
  ms_byte_ls_nibble_->SetLabel(
      std::bitset<4>(ms_byte_val & 0x0F).to_string());
  ms_byte_ms_nibble_->SetLabel(
      std::bitset<4>(ms_byte_val >> 4).to_string());
  disassembled_->SetLabel(disassemble(val));
}

reInputSwitchesStateRow::reInputSwitchesStateRow(
    reROMStateBox* box,
    int addr,
    wxSizer* addr_column,
    wxSizer* ls_byte_ms_nibble_column,
    wxSizer* ls_byte_ls_nibble_column,
    wxSizer* ls_byte_hex_column)
    : reROMStateRow(box, addr, addr_column)
{
  bit_ = new wxStaticText(box, wxID_ANY, "---0");
  bit_hex_ = new wxStaticText(box, wxID_ANY, "00");

  ls_byte_ms_nibble_column->Add(new wxStaticText(box, wxID_ANY, "----"), 0, wxALIGN_RIGHT);
  ls_byte_ls_nibble_column->Add(bit_, 0, wxALIGN_RIGHT);
  ls_byte_hex_column->Add(bit_hex_, 0, wxALIGN_RIGHT);

  const int margin = box->GetFont().GetPixelSize().GetY() / 2;
  ls_byte_ms_nibble_column->AddSpacer(margin);
  ls_byte_ls_nibble_column->AddSpacer(margin);
  ls_byte_hex_column->AddSpacer(margin);
}

void reInputSwitchesStateRow::SetValue(uint16_t val)
{
  uint8_t bit = val & 0x0001;
  bit_hex_->SetLabel("0" + std::to_string(bit));
  bit_->SetLabel("---" + std::to_string(bit));
}

reUnusedStateRow::reUnusedStateRow(
    reROMStateBox* box,
    int addr,
    wxSizer* addr_column,
    wxSizer* ls_byte_ms_nibble_column,
    wxSizer* ls_byte_ls_nibble_column,
    wxSizer* ls_byte_hex_column)
    : reROMStateRow(box, addr, addr_column)
{
  ls_byte_ms_nibble_ = new wxStaticText(box, wxID_ANY, "0000");
  ls_byte_ls_nibble_ = new wxStaticText(box, wxID_ANY, "0000");
  ls_byte_hex_ = new wxStaticText(box, wxID_ANY, "00");

  ls_byte_ms_nibble_column->Add(ls_byte_ms_nibble_, 0, wxALIGN_RIGHT);
  ls_byte_ls_nibble_column->Add(ls_byte_ls_nibble_, 0, wxALIGN_RIGHT);
  ls_byte_hex_column->Add(ls_byte_hex_, 0, wxALIGN_RIGHT);

  const int margin = box->GetFont().GetPixelSize().GetY() / 2;
  ls_byte_ms_nibble_column->AddSpacer(margin);
  ls_byte_ls_nibble_column->AddSpacer(margin);
  ls_byte_hex_column->AddSpacer(margin);
}

void reUnusedStateRow::SetValue(uint16_t val)
{
}
