#include <bitset>
#include <sstream>

#include <wx/wx.h>

class reBasePanel : public wxPanel
{
  public:
    reBasePanel(wxWindow *parent) : wxPanel(parent)
    {
    }

  public:
    virtual void SetValue(int value) = 0;

  protected:
    wxStaticText* value_;
};

class reRegisterPanel : public reBasePanel
{
  public:
    reRegisterPanel(wxWindow *parent, const wxString& register_name);

  public:
    void SetValue(int value);

  private:
    wxStaticText* register_name_;
    wxStaticText* as_unsigned_;
    wxStaticText* as_signed_;
};

class reFlagPanel : public reBasePanel
{
  public:
    reFlagPanel(wxWindow *parent, const wxString& flag_name);

  public:
    void SetValue(int value);

  private:
    wxStaticText* flag_name_;
};

template<int width>
class reMemoryPanel : public reBasePanel
{
  public:
    reMemoryPanel(wxWindow *parent, int addr)
        : reBasePanel(parent)
    {
      wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

      std::stringstream addr_stream;
      addr_stream << "0x" << std::hex << addr;

      addr_ = new wxStaticText(this, wxID_ANY, addr_stream.str(),
                                        wxDefaultPosition, wxSize(40, 30));
      value_ = new wxStaticText(this, wxID_ANY,
                                std::bitset<width>(0).to_string());

      sizer->Add(addr_);
      sizer->Add(value_);

      SetSizer(sizer);
    }

  public:
    void SetValue(int value)
    {
      value_->SetLabel(std::bitset<width>(value).to_string());
    }

  private:
    wxStaticText* addr_;
};
