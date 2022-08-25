#pragma once
#include <wx/wx.h>

class reRegisterStateBox;
class reFlagStateBox;
class reROMStateBox;

template <typename T>
class reBaseStateRow
{
  public:
    virtual void SetValue(T val) = 0;
};

class reRegisterStateRow : public reBaseStateRow<uint8_t>
{
  public:
    reRegisterStateRow(reRegisterStateBox* box,
                       const wxString& label,
                       wxSizer* label_column,
                       wxSizer* first_nibble_column,
                       wxSizer* second_nibble_column,
                       wxSizer* hex_column,
                       wxSizer* signed_column,
                       wxSizer* unsigned_column);

  public:
    void SetValue(uint8_t val) override;

  private:
    wxStaticText* label_;
    wxStaticText* first_nibble_;
    wxStaticText* second_nibble_;
    wxStaticText* hex_;
    wxStaticText* signed_;
    wxStaticText* unsigned_;
};

class reFlagStateRow : public reBaseStateRow<bool>
{
  public:
    reFlagStateRow(reFlagStateBox* box,
                   const wxString& label,
                   wxSizer* label_column,
                   wxSizer* value_column);

  public:
    void SetValue(bool val) override;

  private:
    wxStaticText* label_;
    wxStaticText* value_;
};

class reROMStateRow : public reBaseStateRow<uint16_t>
{
  public:
    reROMStateRow(reROMStateBox* box,
                  int addr,
                  wxSizer* addr_column,
                  wxSizer* ls_byte_ms_nibble_column,
                  wxSizer* ls_byte_ls_nibble_column,
                  wxSizer* ls_byte_hex_column,
                  wxSizer* ms_byte_ms_nibble_column,
                  wxSizer* ms_byte_ls_nibble_column,
                  wxSizer* ms_byte_hex_column,
                  wxSizer* disassembled_column);

  protected:
    wxStaticText* GetAddr() { return addr_; }
    wxStaticText* GetLSByteMSNibble() { return ls_byte_ms_nibble_; }
    wxStaticText* GetLSByteLSNibble() { return ls_byte_ls_nibble_; }
    wxStaticText* GetLSByteHex() { return ls_byte_hex_; }
    wxStaticText* GetMSByteMSNibble() { return ms_byte_ms_nibble_; }
    wxStaticText* GetMSByteLSNibble() { return ms_byte_ls_nibble_; }
    wxStaticText* GetMSByteHex() { return ms_byte_hex_; }
    wxStaticText* GetDisassembled() { return disassembled_; }

  private:
    wxStaticText* addr_;
    wxStaticText* ls_byte_ms_nibble_;
    wxStaticText* ls_byte_ls_nibble_;
    wxStaticText* ls_byte_hex_;
    wxStaticText* ms_byte_ms_nibble_;
    wxStaticText* ms_byte_ls_nibble_;
    wxStaticText* ms_byte_hex_;
    wxStaticText* disassembled_;
};

class reProgramDataStateRow : public reROMStateRow
{
  public:
    reProgramDataStateRow(reROMStateBox* box,
                          int addr,
                          wxSizer* addr_column,
                          wxSizer* ls_byte_ms_nibble_column,
                          wxSizer* ls_byte_ls_nibble_column,
                          wxSizer* ls_byte_hex_column,
                          wxSizer* ms_byte_ms_nibble_column,
                          wxSizer* ms_byte_ls_nibble_column,
                          wxSizer* ms_byte_hex_column,
                          wxSizer* disassembled_column);

  public:
    void SetValue(uint16_t val) override;
};

class reInputSwitchesStateRow : public reROMStateRow
{
  public:
    reInputSwitchesStateRow(reROMStateBox* box,
                            int addr,
                            wxSizer* addr_column,
                            wxSizer* ls_byte_ms_nibble_column,
                            wxSizer* ls_byte_ls_nibble_column,
                            wxSizer* ls_byte_hex_column,
                            wxSizer* ms_byte_ms_nibble_column,
                            wxSizer* ms_byte_ls_nibble_column,
                            wxSizer* ms_byte_hex_column,
                            wxSizer* disassembled_column);

  public:
    void SetValue(uint16_t val) override;
};

class reUnusedStateRow : public reROMStateRow
{
  public:
    reUnusedStateRow(reROMStateBox* box,
                     int addr,
                     wxSizer* addr_column,
                     wxSizer* ls_byte_ms_nibble_column,
                     wxSizer* ls_byte_ls_nibble_column,
                     wxSizer* ls_byte_hex_column,
                     wxSizer* ms_byte_ms_nibble_column,
                     wxSizer* ms_byte_ls_nibble_column,
                     wxSizer* ms_byte_hex_column,
                     wxSizer* disassembled_column);

  public:
    void SetValue(uint16_t val) override;
};
