#pragma once
#include <memory>

#include "core/instructionset.h"

// Forward declaration to prevent circular inclusion. This is necessary because
// the Bus class and the CPU class have pointers to each other.

class Bus;

class CPU {
  public:
    enum RegisterCode : uint8_t
    {
      // General purpose
      kA, kB, kC, kD,

      // Memory access
      kM, kS,

      // Register for subroutines return address and memory access
      kL,

      // Program counter
      kPC
    };

    enum class Flag
    {
      // Carry flag
      kCY,

      // Zero flag
      kZ,

      // Sign flag
      kS
    };

  public:
    CPU(Bus* bus) : bus_(bus)
    {
    };

  public:
    // Performs one instruction cycle.
    void Cycle();

    // Sets all registers to 0 and halted_ to false.
    void Reset() noexcept;

    uint8_t GetRegister(uint8_t code) const;
    void SetRegister(uint8_t code, uint8_t value);
    bool GetFlag(Flag flag) const;
    void SetFlag(Flag flag, bool value);

    uint16_t Read(uint8_t addr) noexcept;
    void Write(uint8_t addr, uint8_t value) noexcept;

    bool Halted() const
    {
      return halted_;
    };

    uint16_t GetInstructionRegister() const
    {
      return instruction_;
    }

  private:
    // Fetches an instruction.
    void Fetch();

    // Decodes and executes an instruction.
    void Execute();

    void HALT();
    void NOP();
    void LOAD();
    void LOADI();
    void STORE();
    void STOREI();
    void CALL();
    void JMP();
    void MOVI();
    void MOV();
    void ALU();

    uint8_t ADC(uint8_t Gs1, uint8_t Op2);
    uint8_t ADD(uint8_t Gs1, uint8_t Op2);
    uint8_t SBC(uint8_t Gs1, uint8_t Op2);
    uint8_t SUB(uint8_t Gs1, uint8_t Op2);
    uint8_t AND(uint8_t Gs1, uint8_t Op2);
    uint8_t OR(uint8_t Gs1, uint8_t Op2);
    uint8_t XOR(uint8_t Gs1, uint8_t Op2);
    uint8_t NOT(uint8_t Gs);
    uint8_t ROR(uint8_t Gs);
    uint8_t SHR(uint8_t Gs);
    uint8_t RCR(uint8_t Gs);

    void BinaryALU();
    void UnaryAlU();

    bool CheckCondition(uint8_t cond);

    // Returns true if register is one of the memory pointers (M, S, L or PC).
    bool IsAddressRegister(uint8_t code) { return code > 4; };

  private:
    // Bus has a smart pointer to the CPU, so no need to free bus_.
    Bus* bus_;

    // Instruction register
    uint16_t instruction_ = 0x0000;

    uint8_t A_ = 0x00;
    uint8_t B_ = 0x00;
    uint8_t C_ = 0x00;
    uint8_t D_ = 0x00;
    uint8_t M_ = 0x00;
    uint8_t S_ = 0x00;
    uint8_t L_ = 0x00;
    uint8_t PC_ = 0x00;

    bool sign_ = false;
    bool zero_ = false;
    bool carry_ = false;

    bool halted_ = false;
};
