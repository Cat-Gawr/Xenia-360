#ifndef LLVM_ADT_BITVECTOR_H
#define LLVM_ADT_BITVECTOR_H

#include "llvm/Support/Compiler.h"
#ifdef LLVM_IGNORE_XENIA
#include "llvm/Support/ErrorHandling.h"
#else
#define llvm_unreachable(msg) assert(false)
#endif // LLVM_IGNORE_XENIA
#include "llvm/Support/MathExtras.h"
#include <algorithm>
#include <cassert>
#include <climits>
#include <cstdlib>

namespace llvm {

class BitVector {
  typedef unsigned long BitWord;

  enum { BITWORD_SIZE = (unsigned)sizeof(BitWord) * CHAR_BIT };

  BitWord  *Bits;        // Actual bits.
  unsigned Size;         // Size of bitvector in bits.
  unsigned Capacity;     // Size of allocated memory in BitWord.

public:
  // Encapsulation of a single bit.
  class reference {
    friend class BitVector;

    BitWord *WordRef;
    unsigned BitPos;

    reference();  // Undefined

  public:
    reference(BitVector &b, unsigned Idx) {
      WordRef = &b.Bits[Idx / BITWORD_SIZE];
      BitPos = Idx % BITWORD_SIZE;
    }

    ~reference() {}

    reference &operator=(reference t) {
      *this = bool(t);
      return *this;
    }

    reference& operator=(bool t) {
      if (t)
        *WordRef |= 1L << BitPos;
      else
        *WordRef &= ~(1L << BitPos);
      return *this;
    }

    operator bool() const {
      return ((*WordRef) & (1L << BitPos)) ? true : false;
    }
  };


  /// BitVector default ctor - Creates an empty bitvector.
  BitVector() : Size(0), Capacity(0) {
    Bits = 0;
  }

  /// BitVector ctor - Creates a bitvector of specified number of bits. All
  /// bits are initialized to the specified value.
  explicit BitVector(unsigned s, bool t = false) : Size(s) {
    Capacity = NumBitWords(s);
    Bits = (BitWord *)std::malloc(Capacity * sizeof(BitWord));
    init_words(Bits, Capacity, t);
    if (t)
      clear_unused_bits();
  }

  /// BitVector copy ctor.
  BitVector(const BitVector &RHS) : Size(RHS.size()) {
    if (Size == 0) {
      Bits = 0;
      Capacity = 0;
      return;
    }

    Capacity = NumBitWords(RHS.size());
    Bits = (BitWord *)std::malloc(Capacity * sizeof(BitWord));
    std::memcpy(Bits, RHS.Bits, Capacity * sizeof(BitWord));
  }

#if LLVM_HAS_RVALUE_REFERENCES
  BitVector(BitVector &&RHS)
    : Bits(RHS.Bits), Size(RHS.Size), Capacity(RHS.Capacity) {
    RHS.Bits = 0;
  }
#endif

  ~BitVector() {
    std::free(Bits);
  }

  /// empty - Tests whether there are no bits in this bitvector.
  bool empty() const { return Size == 0; }

  /// size - Returns the number of bits in this bitvector.
  unsigned size() const { return Size; }

  /// count - Returns the number of bits which are set.
  unsigned count() const {
    unsigned NumBits = 0;
    for (unsigned i = 0; i < NumBitWords(size()); ++i)
      if (sizeof(BitWord) == 4)
        NumBits += CountPopulation_32((uint32_t)Bits[i]);
      else if (sizeof(BitWord) == 8)
        NumBits += CountPopulation_64(Bits[i]);
      else
        llvm_unreachable("Unsupported!");
    return NumBits;
  }

  /// any - Returns true if any bit is set.
  bool any() const {
    for (unsigned i = 0; i < NumBitWords(size()); ++i)
      if (Bits[i] != 0)
        return true;
    return false;
  }

  /// all - Returns true if all bits are set.
  bool all() const {
    for (unsigned i = 0; i < Size / BITWORD_SIZE; ++i)
      if (Bits[i] != ~0UL)
        return false;

    // If bits remain check that they are ones. The unused bits are always zero.
    if (unsigned Remainder = Size % BITWORD_SIZE)
      return Bits[Size / BITWORD_SIZE] == (1UL << Remainder) - 1;

    return true;
  }

  /// none - Returns true if none of the bits are set.
  bool none() const {
    return !any();
  }

  /// find_first - Returns the index of the first set bit, -1 if none
  /// of the bits are set.
  int find_first() const {
    for (unsigned i = 0; i < NumBitWords(size()); ++i)
      if (Bits[i] != 0) {
        if (sizeof(BitWord) == 4)
          return i * BITWORD_SIZE + countTrailingZeros((uint32_t)Bits[i]);
        if (sizeof(BitWord) == 8)
          return i * BITWORD_SIZE + countTrailingZeros(Bits[i]);
        llvm_unreachable("Unsupported!");
      }
    return -1;
  }

  /// find_next - Returns the index of the next set bit following the
  /// "Prev" bit. Returns -1 if the next set bit is not found.
  int find_next(unsigned Prev) const {
    ++Prev;
    if (Prev >= Size)
      return -1;

    unsigned WordPos = Prev / BITWORD_SIZE;
    unsigned BitPos = Prev % BITWORD_SIZE;
    BitWord Copy = Bits[WordPos];
    // Mask off previous bits.
    Copy &= ~0UL << BitPos;

    if (Copy != 0) {
      if (sizeof(BitWord) == 4)
        return WordPos * BITWORD_SIZE + countTrailingZeros((uint32_t)Copy);
      if (sizeof(BitWord) == 8)
        return WordPos * BITWORD_SIZE + countTrailingZeros(Copy);
      llvm_unreachable("Unsupported!");
    }

    // Check subsequent words.
    for (unsigned i = WordPos+1; i < NumBitWords(size()); ++i)
      if (Bits[i] != 0) {
        if (sizeof(BitWord) == 4)
          return i * BITWORD_SIZE + countTrailingZeros((uint32_t)Bits[i]);
        if (sizeof(BitWord) == 8)
          return i * BITWORD_SIZE + countTrailingZeros(Bits[i]);
        llvm_unreachable("Unsupported!");
      }
    return -1;
  }

  /// clear - Clear all bits.
  void clear() {
    Size = 0;
  }

  /// resize - Grow or shrink the bitvector.
  void resize(unsigned N, bool t = false) {
    if (N > Capacity * BITWORD_SIZE) {
      unsigned OldCapacity = Capacity;
      grow(N);
      init_words(&Bits[OldCapacity], (Capacity-OldCapacity), t);
    }

    // Set any old unused bits that are now included in the BitVector. This
    // may set bits that are not included in the new vector, but we will clear
    // them back out below.
    if (N > Size)
      set_unused_bits(t);

    // Update the size, and clear out any bits that are now unused
    unsigned OldSize = Size;
    Size = N;
    if (t || N < OldSize)
      clear_unused_bits();
  }

  void reserve(unsigned N) {
    if (N > Capacity * BITWORD_SIZE)
      grow(N);
  }

  // Set, reset, flip
  BitVector &set() {
    init_words(Bits, Capacity, true);
    clear_unused_bits();
    return *this;
  }

  BitVector &set(unsigned Idx) {
    Bits[Idx / BITWORD_SIZE] |= 1L << (Idx % BITWORD_SIZE);
    return *this;
  }

  /// set - Efficiently set a range of bits in [I, E)
  BitVector &set(unsigned I, unsigned E) {
    assert(I <= E && "Attempted to set backwards range!");
    assert(E <= size() && "Attempted to set out-of-bounds range!");

    if (I == E) return *this;

    if (I / BITWORD_SIZE == E / BITWORD_SIZE) {
      BitWord EMask = 1UL << (E % BITWORD_SIZE);
      BitWord IMask = 1UL << (I % BITWORD_SIZE);
      BitWord Mask = EMask - IMask;
      Bits[I / BITWORD_SIZE] |= Mask;
      return *this;
    }

    BitWord PrefixMask = ~0UL << (I % BITWORD_SIZE);
    Bits[I / BITWORD_SIZE] |= PrefixMask;
    I = RoundUpToAlignment(I, BITWORD_SIZE);

    for (; I + BITWORD_SIZE <= E; I += BITWORD_SIZE)
      Bits[I / BITWORD_SIZE] = ~0UL;

    BitWord PostfixMask = (1UL << (E % BITWORD_SIZE)) - 1;
    if (I < E)
      Bits[I / BITWORD_SIZE] |= PostfixMask;

    return *this;
  }

  BitVector &reset() {
    init_words(Bits, Capacity, false);
    return *this;
  }

  BitVector &reset(unsigned Idx) {
    Bits[Idx / BITWORD_SIZE] &= ~(1L << (Idx % BITWORD_SIZE));
    return *this;
  }

  /// reset - Efficiently reset a range of bits in [I, E)
  BitVector &reset(unsigned I, unsigned E) {
    assert(I <= E && "Attempted to reset backwards range!");
    assert(E <= size() && "Attempted to reset out-of-bounds range!");

    if (I == E) return *this;

    if (I / BITWORD_SIZE == E / BITWORD_SIZE) {
      BitWord EMask = 1UL << (E % BITWORD_SIZE);
      BitWord IMask = 1UL << (I % BITWORD_SIZE);
      BitWord Mask = EMask - IMask;
      Bits[I / BITWORD_SIZE] &= ~Mask;
      return *this;
    }

    BitWord PrefixMask = ~0UL << (I % BITWORD_SIZE);
    Bits[I / BITWORD_SIZE] &= ~PrefixMask;
    I = RoundUpToAlignment(I, BITWORD_SIZE);

    for (; I + BITWORD_SIZE <= E; I += BITWORD_SIZE)
      Bits[I / BITWORD_SIZE] = 0UL;

    BitWord PostfixMask = (1UL << (E % BITWORD_SIZE)) - 1;
    if (I < E)
      Bits[I / BITWORD_SIZE] &= ~PostfixMask;

    return *this;
  }

  BitVector &flip() {
    for (unsigned i = 0; i < NumBitWords(size()); ++i)
      Bits[i] = ~Bits[i];
    clear_unused_bits();
    return *this;
  }

  BitVector &flip(unsigned Idx) {
    Bits[Idx / BITWORD_SIZE] ^= 1L << (Idx % BITWORD_SIZE);
    return *this;
  }

  // Indexing.
  reference operator[](unsigned Idx) {
    assert (Idx < Size && "Out-of-bounds Bit access.");
    return reference(*this, Idx);
  }

  bool operator[](unsigned Idx) const {
    assert (Idx < Size && "Out-of-bounds Bit access.");
    BitWord Mask = 1L << (Idx % BITWORD_SIZE);
    return (Bits[Idx / BITWORD_SIZE] & Mask) != 0;
  }

  bool test(unsigned Idx) const {
    return (*this)[Idx];
  }

  /// Test if any common bits are set.
  bool anyCommon(const BitVector &RHS) const {
    unsigned ThisWords = NumBitWords(size());
    unsigned RHSWords  = NumBitWords(RHS.size());
    for (unsigned i = 0, e = std::min(ThisWords, RHSWords); i != e; ++i)
      if (Bits[i] & RHS.Bits[i])
        return true;
    return false;
  }

  // Comparison operators.
  bool operator==(const BitVector &RHS) const {
    unsigned ThisWords = NumBitWords(size());
    unsigned RHSWords  = NumBitWords(RHS.size());
    unsigned i;
    for (i = 0; i != std::min(ThisWords, RHSWords); ++i)
      if (Bits[i] != RHS.Bits[i])
        return false;

    // Verify that any extra words are all zeros.
    if (i != ThisWords) {
      for (; i != ThisWords; ++i)
        if (Bits[i])
          return false;
    } else if (i != RHSWords) {
      for (; i != RHSWords; ++i)
        if (RHS.Bits[i])
          return false;
    }
    return true;
  }

  bool operator!=(const BitVector &RHS) const {
    return !(*this == RHS);
  }

  /// Intersection, union, disjoint union.
  BitVector &operator&=(const BitVector &RHS) {
    unsigned ThisWords = NumBitWords(size());
    unsigned RHSWords  = NumBitWords(RHS.size());
    unsigned i;
    for (i = 0; i != std::min(ThisWords, RHSWords); ++i)
      Bits[i] &= RHS.Bits[i];

    // Any bits that are just in this bitvector become zero, because they aren't
    // in the RHS bit vector.  Any words only in RHS are ignored because they
    // are already zero in the LHS.
    for (; i != ThisWords; ++i)
      Bits[i] = 0;

    return *this;
  }

  /// reset - Reset bits that are set in RHS. Same as *this &= ~RHS.
  BitVector &reset(const BitVector &RHS) {
    unsigned ThisWords = NumBitWords(size());
    unsigned RHSWords  = NumBitWords(RHS.size());
    unsigned i;
    for (i = 0; i != std::min(ThisWords, RHSWords); ++i)
      Bits[i] &= ~RHS.Bits[i];
    return *this;
  }

  /// test - Check if (This - RHS) is zero.
  /// This is the same as reset(RHS) and any().
  bool test(const BitVector &RHS) const {
    unsigned ThisWords = NumBitWords(size());
    unsigned RHSWords  = NumBitWords(RHS.size());
    unsigned i;
    for (i = 0; i != std::min(ThisWords, RHSWords); ++i)
      if ((Bits[i] & ~RHS.Bits[i]) != 0)
        return true;

    for (; i != ThisWords ; ++i)
      if (Bits[i] != 0)
        return true;

    return false;
  }

  BitVector &operator|=(const BitVector &RHS) {
    if (size() < RHS.size())
      resize(RHS.size());
    for (size_t i = 0, e = NumBitWords(RHS.size()); i != e; ++i)
      Bits[i] |= RHS.Bits[i];
    return *this;
  }

  BitVector &operator^=(const BitVector &RHS) {
    if (size() < RHS.size())
      resize(RHS.size());
    for (size_t i = 0, e = NumBitWords(RHS.size()); i != e; ++i)
      Bits[i] ^= RHS.Bits[i];
    return *this;
  }

  // Assignment operator.
  const BitVector &operator=(const BitVector &RHS) {
    if (this == &RHS) return *this;

    Size = RHS.size();
    unsigned RHSWords = NumBitWords(Size);
    if (Size <= Capacity * BITWORD_SIZE) {
      if (Size)
        std::memcpy(Bits, RHS.Bits, RHSWords * sizeof(BitWord));
      clear_unused_bits();
      return *this;
    }

    // Grow the bitvector to have enough elements.
    Capacity = RHSWords;
    BitWord *NewBits = (BitWord *)std::malloc(Capacity * sizeof(BitWord));
    std::memcpy(NewBits, RHS.Bits, Capacity * sizeof(BitWord));

    // Destroy the old bits.
    std::free(Bits);
    Bits = NewBits;

    return *this;
  }

#if LLVM_HAS_RVALUE_REFERENCES
  const BitVector &operator=(BitVector &&RHS) {
    if (this == &RHS) return *this;

    std::free(Bits);
    Bits = RHS.Bits;
    Size = RHS.Size;
    Capacity = RHS.Capacity;

    RHS.Bits = 0;

    return *this;
  }
#endif

  void swap(BitVector &RHS) {
    std::swap(Bits, RHS.Bits);
    std::swap(Size, RHS.Size);
    std::swap(Capacity, RHS.Capacity);
  }

  //===--------------------------------------------------------------------===//
  // Portable bit mask operations.
  //===--------------------------------------------------------------------===//
  //
  // These methods all operate on arrays of uint32_t, each holding 32 bits. The
  // fixed word size makes it easier to work with literal bit vector constants
  // in portable code.
  //
  // The LSB in each word is the lowest numbered bit.  The size of a portable
  // bit mask is always a whole multiple of 32 bits.  If no bit mask size is
  // given, the bit mask is assumed to cover the entire BitVector.

  /// setBitsInMask - Add '1' bits from Mask to this vector. Don't resize.
  /// This computes "*this |= Mask".
  void setBitsInMask(const uint32_t *Mask, unsigned MaskWords = ~0u) {
    applyMask<true, false>(Mask, MaskWords);
  }

  /// clearBitsInMask - Clear any bits in this vector that are set in Mask.
  /// Don't resize. This computes "*this &= ~Mask".
  void clearBitsInMask(const uint32_t *Mask, unsigned MaskWords = ~0u) {
    applyMask<false, false>(Mask, MaskWords);
  }

  /// setBitsNotInMask - Add a bit to this vector for every '0' bit in Mask.
  /// Don't resize.  This computes "*this |= ~Mask".
  void setBitsNotInMask(const uint32_t *Mask, unsigned MaskWords = ~0u) {
    applyMask<true, true>(Mask, MaskWords);
  }

  /// clearBitsNotInMask - Clear a bit in this vector for every '0' bit in Mask.
  /// Don't resize.  This computes "*this &= Mask".
  void clearBitsNotInMask(const uint32_t *Mask, unsigned MaskWords = ~0u) {
    applyMask<false, true>(Mask, MaskWords);
  }

private:
  unsigned NumBitWords(unsigned S) const {
    return (S + BITWORD_SIZE-1) / BITWORD_SIZE;
  }

  // Set the unused bits in the high words.
  void set_unused_bits(bool t = true) {
    //  Set high words first.
    unsigned UsedWords = NumBitWords(Size);
    if (Capacity > UsedWords)
      init_words(&Bits[UsedWords], (Capacity-UsedWords), t);

    //  Then set any stray high bits of the last used word.
    unsigned ExtraBits = Size % BITWORD_SIZE;
    if (ExtraBits) {
      BitWord ExtraBitMask = ~0UL << ExtraBits;
      if (t)
        Bits[UsedWords-1] |= ExtraBitMask;
      else
        Bits[UsedWords-1] &= ~ExtraBitMask;
    }
  }

  // Clear the unused bits in the high words.
  void clear_unused_bits() {
    set_unused_bits(false);
  }

  void grow(unsigned NewSize) {
    Capacity = std::max(NumBitWords(NewSize), Capacity * 2);
    Bits = (BitWord *)std::realloc(Bits, Capacity * sizeof(BitWord));

    clear_unused_bits();
  }

  void init_words(BitWord *B, unsigned NumWords, bool t) {
    memset(B, 0 - (int)t, NumWords*sizeof(BitWord));
  }

  template<bool AddBits, bool InvertMask>
  void applyMask(const uint32_t *Mask, unsigned MaskWords) {
    assert(BITWORD_SIZE % 32 == 0 && "Unsupported BitWord size.");
    MaskWords = std::min(MaskWords, (size() + 31) / 32);
    const unsigned Scale = BITWORD_SIZE / 32;
    unsigned i;
    for (i = 0; MaskWords >= Scale; ++i, MaskWords -= Scale) {
      BitWord BW = Bits[i];
      // This inner loop should unroll completely when BITWORD_SIZE > 32.
      for (unsigned b = 0; b != BITWORD_SIZE; b += 32) {
        uint32_t M = *Mask++;
        if (InvertMask) M = ~M;
        if (AddBits) BW |=   BitWord(M) << b;
        else         BW &= ~(BitWord(M) << b);
      }
      Bits[i] = BW;
    }
    for (unsigned b = 0; MaskWords; b += 32, --MaskWords) {
      uint32_t M = *Mask++;
      if (InvertMask) M = ~M;
      if (AddBits) Bits[i] |=   BitWord(M) << b;
      else         Bits[i] &= ~(BitWord(M) << b);
    }
    if (AddBits)
      clear_unused_bits();
  }
};

} // End llvm namespace

namespace std {
  /// Implement std::swap in terms of BitVector swap.
  inline void
  swap(llvm::BitVector &LHS, llvm::BitVector &RHS) {
    LHS.swap(RHS);
  }
}

#endif
