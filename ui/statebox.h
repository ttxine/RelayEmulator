#include <array>
#include <memory>

#include <wx/wx.h>

#include "ui/staterow.h"
#include "core/rom.h"

class reBaseStateBox : public wxStaticBox
{
  public:
    reBaseStateBox(wxWindow* parent, const wxString& label, int orient);

  public:
    virtual void CreateRows() = 0;

    wxStaticBoxSizer* GetStaticBoxSizer() const { return sizer_; }
    void SetStaticBoxSizer(wxStaticBoxSizer* sizer) { sizer_ = sizer; }

  protected:
    wxStaticBoxSizer* sizer_;
};

class reRegisterStateBox : public reBaseStateBox
{
  public:
    reRegisterStateBox(wxWindow *parent);

  public:
    void SetAValue(uint8_t value) { A_->SetValue(value); }
    void SetBValue(uint8_t value) { B_->SetValue(value); }
    void SetCValue(uint8_t value) { C_->SetValue(value); }
    void SetDValue(uint8_t value) { D_->SetValue(value); }
    void SetMValue(uint8_t value) { M_->SetValue(value); }
    void SetSValue(uint8_t value) { S_->SetValue(value); }
    void SetLValue(uint8_t value) { L_->SetValue(value); }
    void SetPCValue(uint8_t value) { PC_->SetValue(value); }

  private:
    void CreateRows();
    void CreateSubSizers();
    reRegisterStateRow* AddRowToLeft(const wxString& label);
    reRegisterStateRow* AddRowToRight(const wxString& label);

  private:
    reRegisterStateRow* A_;
    reRegisterStateRow* B_;
    reRegisterStateRow* C_;
    reRegisterStateRow* D_;

    reRegisterStateRow* M_;
    reRegisterStateRow* S_;
    reRegisterStateRow* L_;
    reRegisterStateRow* PC_;

    wxBoxSizer* left_label_column_ = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* left_first_nibble_column_ = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* left_second_nibble_column_ = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* left_hex_column_ = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* left_signed_column_ = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* left_unsigned_column_ = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* right_label_column_ = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* right_first_nibble_column_ = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* right_second_nibble_column_ = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* right_hex_column_ = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* right_signed_column_ = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* right_unsigned_column_ = new wxBoxSizer(wxVERTICAL);
};

class reFlagStateBox : public reBaseStateBox
{
  public:
    reFlagStateBox(wxWindow *parent);

  public:
    void SetSValue(uint8_t value) { S_->SetValue(value); }
    void SetZValue(uint8_t value) { Z_->SetValue(value); }
    void SetCYValue(uint8_t value) { CY_->SetValue(value); }

  private:
    void CreateRows();
    reFlagStateRow* AddRow(const wxString& label);

  private:
    reFlagStateRow* S_;
    reFlagStateRow* Z_;
    reFlagStateRow* CY_;

    wxBoxSizer* label_column_ = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* value_column_ = new wxBoxSizer(wxVERTICAL);
};

class reROMStateBox : public reBaseStateBox
{
  public:
    reROMStateBox(wxWindow *parent);

  public:
    void SetProgramDataValues(
        const std::array<uint16_t, ROM::kProgramDataSize>& values)
    {
      for (int i = 0; i < ROM::kProgramDataSize; ++i)
      {
        program_[i]->SetValue(values[i]);
      }
    }

    void SetInputSwitchesValues(
        const std::array<uint8_t, ROM::kInputSwitchesSize>& values)
    {
      for (int i = 0; i < ROM::kInputSwitchesSize; ++i)
      {
        input_[i]->SetValue(values[i]);
      }
    }

  private:
    void CreateRows();
    void CreateSubSizers();
    void AddNull();
    void AddRow(uint8_t addr);

  private:
    std::array<reProgramDataStateRow*, ROM::kProgramDataSize> program_;
    std::array<reInputSwitchesStateRow*, ROM::kInputSwitchesSize> input_;

    wxSizer* addr_column_ = new wxBoxSizer(wxVERTICAL);
    wxSizer* ls_byte_ms_nibble_column_ = new wxBoxSizer(wxVERTICAL);
    wxSizer* ls_byte_ls_nibble_column_ = new wxBoxSizer(wxVERTICAL);
    wxSizer* ls_byte_hex_column_ = new wxBoxSizer(wxVERTICAL);
    wxSizer* ms_byte_ms_nibble_column_ = new wxBoxSizer(wxVERTICAL);
    wxSizer* ms_byte_ls_nibble_column_ = new wxBoxSizer(wxVERTICAL);
    wxSizer* ms_byte_hex_column_ = new wxBoxSizer(wxVERTICAL);
    wxSizer* disassembled_column_ = new wxBoxSizer(wxVERTICAL);
};
