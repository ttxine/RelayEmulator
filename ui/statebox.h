#include <array>

#include <wx/wx.h>

#include "ui/panels.h"
#include "core/rom.h"

class reBaseStateBox : public wxStaticBox
{
  public:
    reBaseStateBox(wxWindow *parent, const wxString& label);

  public:
    wxStaticBoxSizer* GetStaticBoxSizer() const
    {
      return sizer_;
    }

    void SetStaticBoxSizer(wxStaticBoxSizer* sizer)
    {
      sizer_ = sizer;
    }

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
    reRegisterPanel* A_;
    reRegisterPanel* B_;
    reRegisterPanel* C_;
    reRegisterPanel* D_;
    reRegisterPanel* M_;
    reRegisterPanel* S_;
    reRegisterPanel* L_;
    reRegisterPanel* PC_;
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
    reFlagPanel* S_;
    reFlagPanel* Z_;
    reFlagPanel* CY_;
};

class reMemoryStateBox : public reBaseStateBox
{
  public:
    reMemoryStateBox(wxWindow *parent);

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

    void SetUnusedValues(
        const std::array<uint8_t, ROM::kUnusedSize>& values)
    {
      for (int i = 0; i < ROM::kUnusedSize; ++i)
      {
        unused_[i]->SetValue(values[i]);
      }
    }

  private:
    std::array<reMemoryPanel<16>*, ROM::kProgramDataSize> program_;
    std::array<reMemoryPanel<8>*, ROM::kInputSwitchesSize> input_;
    std::array<reMemoryPanel<8>*, ROM::kUnusedSize> unused_;
};
