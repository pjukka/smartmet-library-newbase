// ======================================================================
/*!
 * \file NFmiRawData.cpp
 * \brief Implementation of class NFmiRawData
 */
// ======================================================================

#ifdef _MSC_VER
#pragma warning(disable : 4503)  // VC++ 2012 kääntäjällä tulee paljon ruman näköistä varoitusta
                                 // ilman tätä (decorated name length exceeded (4096 bytes), name
                                 // was truncated)
#pragma warning(disable : 4101)  // VC++ 2012 kääntäjällä tulee paljon 'lock' unreferenced
                                 // -varoitusta ilman tätä.  Scoped variableen ei ole
                                 // tarkoitustakaan viitata, sen tarkoitus on purkaa jotain scopen
                                 // loppuessa.
#endif

#include "NFmiRawData.h"
#include "NFmiVersion.h"

#include <boost/lexical_cast.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/thread.hpp>

#include <iostream>
#include <stdexcept>

#if 0
// paranoid mode
typedef boost::shared_mutex MutexType;
typedef boost::shared_lock<MutexType> ReadLock;
typedef boost::unique_lock<MutexType> WriteLock;
#else
// trust kernel to handle it
struct FooBar
{
};
typedef FooBar MutexType;
typedef FooBar ReadLock;
typedef FooBar WriteLock;
#endif

typedef boost::interprocess::file_mapping FileMapping;
typedef boost::interprocess::mapped_region MappedRegion;

using namespace std;

// ----------------------------------------------------------------------
/*!
 * \brief Pimple class
 */
// ----------------------------------------------------------------------

class NFmiRawData::Pimple
{
 public:
  ~Pimple();
  Pimple();
  Pimple(const Pimple &other);
  Pimple(istream &file, size_t size, bool endianswap);
  Pimple(const string &filename, istream &file, size_t size);
  bool Init(size_t size);
  size_t Size() const;
  float GetValue(size_t index) const;
  bool SetValue(size_t index, float value);
  void SetBinaryStorage(bool flag) const;
  bool IsBinaryStorageUsed() const;
  ostream &Write(ostream &file) const;
  void Backup(char *ptr) const;
  void Undo(char *ptr);
  bool Advise(FmiAdvice advice);

 private:
#if NFMIRAWDATA_ENABLE_UNDO_REDO
  void Unmap() const;
#endif

  mutable MutexType itsMutex;
  mutable float *itsData;
  mutable boost::scoped_ptr<FileMapping> itsFileMapping;
  mutable boost::scoped_ptr<MappedRegion> itsMappedRegion;
  size_t itsSize;
  mutable bool itsSaveAsBinaryFlag;
  bool itsEndianSwapFlag;
};

// ----------------------------------------------------------------------
/*!
 * \brief Destructor
 */
// ----------------------------------------------------------------------

NFmiRawData::Pimple::~Pimple() { delete[] itsData; }
// ----------------------------------------------------------------------
/*!
 * \brief Default constructor
 */
// ----------------------------------------------------------------------

NFmiRawData::Pimple::Pimple()
    : itsMutex(),
      itsData(0),
      itsFileMapping(0),
      itsMappedRegion(0),
      itsSize(0),
      itsSaveAsBinaryFlag(true),
      itsEndianSwapFlag(false)
{
}

// ----------------------------------------------------------------------
/*!
 * \brief Copy constructor
 */
// ----------------------------------------------------------------------

NFmiRawData::Pimple::Pimple(const Pimple &other)
    : itsMutex(),
      itsData(0),
      itsFileMapping(0),
      itsMappedRegion(0),
      itsSize(other.itsSize),
      itsSaveAsBinaryFlag(other.itsSaveAsBinaryFlag),
      itsEndianSwapFlag(false)
{
  // We assume copied data will be changed so that copying mmapping would
  // be a wasted effort

  WriteLock lock(itsMutex);
  itsData = new float[itsSize];

  if (other.itsData != 0)
  {
    memcpy(itsData, other.itsData, itsSize * sizeof(float));
  }
  else
  {
    char *dst = reinterpret_cast<char *>(itsData);
    char *src = reinterpret_cast<char *>(other.itsMappedRegion->get_address());
    memcpy(dst, src, itsSize * sizeof(float));
  }
}

// ----------------------------------------------------------------------
/*!
 * \brief File constructor (memory mapping)
 */
// ----------------------------------------------------------------------

NFmiRawData::Pimple::Pimple(const string &filename, istream &file, size_t size)
    : itsMutex(),
      itsData(0),
      itsFileMapping(0),
      itsMappedRegion(0),
      itsSize(size),
      itsSaveAsBinaryFlag(true),
      itsEndianSwapFlag(false)
{
  using namespace boost::interprocess;

  WriteLock lock(itsMutex);

  // Backward compatibility:
  long datatype;
  file >> datatype;

  file >> itsSaveAsBinaryFlag;

  size_t poolsize = 0;
  file >> poolsize;

  if (poolsize != itsSize * sizeof(float))
    throw runtime_error("Invalid datapool size in querydata");

  // Skip last newline charater after poolsize
  char ch;
  file.get(ch);

  // Sanity check on the size of the data. Note that old querydata
  // has an extra endl at the end, hence we must permit 2 extra
  // characters at the end for legacy data to work

  std::size_t filesize = boost::filesystem::file_size(filename);
  std::size_t filepos = file.tellg();

  if (filepos + poolsize > filesize)
    throw std::runtime_error("Querydata file " +
                             boost::lexical_cast<std::string>(filepos + poolsize - filesize) +
                             " bytes too short: '" + filename + "'");

  else if (filesize - filepos - poolsize > 2)
    throw std::runtime_error("Querydata file " +
                             boost::lexical_cast<std::string>(filesize - filepos - poolsize) +
                             " bytes too long: '" + filename + "'");

  // memory map starting from this file position
  // only if itsSaveAsBinaryFlag is true

  if (itsSaveAsBinaryFlag)
  {
    itsFileMapping.reset(new FileMapping(filename.c_str(), read_only));

    istream::pos_type offset = file.tellg();

    itsMappedRegion.reset(
        new MappedRegion(*itsFileMapping, read_only, offset, itsSize * sizeof(float)));
  }
  else
  {
    itsData = new float[itsSize];

    for (size_t idx = 0; idx < itsSize; ++idx)
      file >> itsData[idx];
  }
}

// ----------------------------------------------------------------------
/*!
 * \brief Stream constructor (no memory mapping)
 */
// ----------------------------------------------------------------------

NFmiRawData::Pimple::Pimple(istream &file, size_t size, bool endianswap)
    : itsMutex(),
      itsData(0),
      itsFileMapping(0),
      itsMappedRegion(0),
      itsSize(size),
      itsSaveAsBinaryFlag(true),
      itsEndianSwapFlag(endianswap)
{
  WriteLock lock(itsMutex);

  delete itsData;
  itsData = new float[itsSize];

  // Backward compatibility:
  long datatype;
  file >> datatype;

  if (FmiInfoVersion >= 6)
    file >> itsSaveAsBinaryFlag;
  else
    itsSaveAsBinaryFlag = false;

  size_t poolsize = 0;
  file >> poolsize;

  if (!itsSaveAsBinaryFlag)
  {
    for (size_t idx = 0; idx < itsSize; ++idx)
      file >> itsData[idx];
  }
  else
  {
    if (poolsize != itsSize * sizeof(float))
      throw runtime_error("Invalid datapool size in querydata");

    // Skip last newline charater after poolsize
    char ch;
    file.get(ch);

    char *ptr = reinterpret_cast<char *>(itsData);
    file.read(ptr, poolsize);
  }

  if (file.fail()) throw runtime_error("Failed to read rawdata from input stream");

  if (itsEndianSwapFlag)
  {
    char tmp1, tmp2, tmp3, tmp4;
    char *ptr = reinterpret_cast<char *>(itsData);
    for (size_t i = 3; i < itsSize * sizeof(float); i += 4)
    {
      tmp1 = ptr[i - 3];
      tmp2 = ptr[i - 2];
      tmp3 = ptr[i - 1];
      tmp4 = ptr[i - 0];

      ptr[i - 3] = tmp4;
      ptr[i - 2] = tmp3;
      ptr[i - 1] = tmp2;
      ptr[i - 0] = tmp1;
    }
  }
}

// ----------------------------------------------------------------------
/*!
 * \brief Init
 */
// ----------------------------------------------------------------------

bool NFmiRawData::Pimple::Init(size_t size)
{
  WriteLock lock(itsMutex);

  itsData = 0;
  itsSize = size;
  if (itsSize > 0)
  {
    itsData = new float[itsSize];
    for (size_t i = 0; i < itsSize; i++)
      itsData[i] = kFloatMissing;
  }
  return true;
}

// ----------------------------------------------------------------------
/*!
 * \brief Get size
 */
// ----------------------------------------------------------------------

size_t NFmiRawData::Pimple::Size() const { return itsSize; }
// ----------------------------------------------------------------------
/*!
 * \brief Set storage mode to binary/ascii
 */
// ----------------------------------------------------------------------

void NFmiRawData::Pimple::SetBinaryStorage(bool flag) const
{
  WriteLock lock(itsMutex);
  itsSaveAsBinaryFlag = flag;
}

// ----------------------------------------------------------------------
/*!
 * \brief Return true if binary output is used
 */
// ----------------------------------------------------------------------

bool NFmiRawData::Pimple::IsBinaryStorageUsed() const
{
  ReadLock lock(itsMutex);
  return itsSaveAsBinaryFlag;
}

// ----------------------------------------------------------------------
/*!
 * \brief Switch from mmapped data to dynamically allocated array
 */
// ----------------------------------------------------------------------

#if NFMIRAWDATA_ENABLE_UNDO_REDO
void NFmiRawData::Pimple::Unmap() const
{
  if (itsData) return;  // oli jo alustettu

  itsData = new float[itsSize];
  char *dst = reinterpret_cast<char *>(itsData);

  char *src = reinterpret_cast<char *>(itsMappedRegion->get_address());
  memcpy(dst, src, itsSize * sizeof(float));

  itsMappedRegion.reset();
  itsFileMapping.reset();
}
#endif

// ----------------------------------------------------------------------
/*!
 * \brief Get value
 */
// ----------------------------------------------------------------------

float NFmiRawData::Pimple::GetValue(size_t index) const
{
  ReadLock lock(itsMutex);

  if (index >= itsSize) return kFloatMissing;

  if (itsData) return itsData[index];

  float *ptr = reinterpret_cast<float *>(itsMappedRegion->get_address());
  return ptr[index];
}

// ----------------------------------------------------------------------
/*!
 * \brief Set value
 */
// ----------------------------------------------------------------------

bool NFmiRawData::Pimple::SetValue(size_t index, float value)
{
  WriteLock lock(itsMutex);

  if (index >= itsSize) return false;

  if (itsData)
  {
    itsData[index] = value;
    return true;
  }

// copy-on-write semantics: switch to memory buffer on a write

#if 0
  Unmap();
  itsData[index] = value;
#endif

  return true;
}

// ----------------------------------------------------------------------
/*!
 * \brief Write to output
 */
// ----------------------------------------------------------------------

ostream &NFmiRawData::Pimple::Write(ostream &file) const
{
  ReadLock lock(itsMutex);

  // Backward compatibility when other than floats were supported
  const int kFloat = 6;
  file << kFloat << endl;

  if (FmiInfoVersion >= 6) file << itsSaveAsBinaryFlag << endl;

  file << itsSize * sizeof(float) << endl;

  if (itsSaveAsBinaryFlag && FmiInfoVersion >= 6)
  {
    if (itsData != 0)
    {
      char *ptr = reinterpret_cast<char *>(itsData);
      file.write(ptr, itsSize * sizeof(float));
    }
    else
    {
      char *ptr = reinterpret_cast<char *>(itsMappedRegion->get_address());
      file.write(ptr, itsSize * sizeof(float));
    }

    // Backward compatibility - not sure if needed:
    file << endl;
  }
  else
  {
    for (size_t idx = 0; idx < itsSize; ++idx)
      file << GetValue(idx) << ' ';
  }
  return file;
}

// ----------------------------------------------------------------------
/*!
 * \brief Backup the raw data
 */
// ----------------------------------------------------------------------

void NFmiRawData::Pimple::Backup(char *ptr) const
{
  ReadLock lock(itsMutex);

// we assume data which is backed up is edited, so might as well unmap
#if NFMIRAWDATA_ENABLE_UNDO_REDO
  Unmap();
#endif

  char *src = reinterpret_cast<char *>(itsData);
  memcpy(ptr, src, itsSize * sizeof(float));
}

// ----------------------------------------------------------------------
/*!
 * \brief Undo changes from backup
 */
// ----------------------------------------------------------------------

void NFmiRawData::Pimple::Undo(char *ptr)
{
  WriteLock lock(itsMutex);

// This may be slower than necessary when mmapped, but since Backup
// unmaps this really should never actually unmap

#if NFMIRAWDATA_ENABLE_UNDO_REDO
  Unmap();
#endif
  char *src = reinterpret_cast<char *>(itsData);
  memcpy(src, ptr, itsSize * sizeof(float));
}

// ----------------------------------------------------------------------
/*!
 * \brief Advice the memory mapping
 */
// ----------------------------------------------------------------------

bool NFmiRawData::Pimple::Advise(FmiAdvice advice)
{
  if (!itsMappedRegion) return false;

  // unknown whether this is really required
  WriteLock lock(itsMutex);

  switch (advice)
  {
    case kFmiAdviceNormal:
      return itsMappedRegion->advise(boost::interprocess::mapped_region::advice_normal);
    case kFmiAdviceSequential:
      return itsMappedRegion->advise(boost::interprocess::mapped_region::advice_sequential);
    case kFmiAdviceRandom:
      return itsMappedRegion->advise(boost::interprocess::mapped_region::advice_random);
    case kFmiAdviceWillNeed:
      return itsMappedRegion->advise(boost::interprocess::mapped_region::advice_willneed);
    case kFmiAdviceDontNeed:
      return itsMappedRegion->advise(boost::interprocess::mapped_region::advice_dontneed);
  }

  // NOT REACHED
  return false;
}

// ----------------------------------------------------------------------
/*!
 * \brief Destructor
 */
// ----------------------------------------------------------------------

NFmiRawData::~NFmiRawData() { delete itsPimple; }
// ----------------------------------------------------------------------
/*!
 * \brief Default constructor
 */
// ----------------------------------------------------------------------

NFmiRawData::NFmiRawData() : itsPimple(new Pimple()) {}
// ----------------------------------------------------------------------
/*!
 * \brief Copy constructor
 */
// ----------------------------------------------------------------------

NFmiRawData::NFmiRawData(const NFmiRawData &other) : itsPimple(new Pimple(*other.itsPimple)) {}
// ----------------------------------------------------------------------
/*!
 * \brief Constructor anticipating a memory mapped file
 */
// ----------------------------------------------------------------------

NFmiRawData::NFmiRawData(const string &filename, istream &file, size_t size)
    : itsPimple(new Pimple(filename, file, size))
{
}

// ----------------------------------------------------------------------
/*!
 * \brief Constructor not anticipating a memory mapped file
 */
// ----------------------------------------------------------------------

NFmiRawData::NFmiRawData(istream &file, size_t size, bool endianswap)
    : itsPimple(new Pimple(file, size, endianswap))
{
}

// ----------------------------------------------------------------------
/*!
 * \brief Init
 */
// ----------------------------------------------------------------------

bool NFmiRawData::Init(size_t size) { return itsPimple->Init(size); }
// ----------------------------------------------------------------------
/*!
 * \brief Return size of raw data
 */
// ----------------------------------------------------------------------

size_t NFmiRawData::Size() const { return itsPimple->Size(); }
// ----------------------------------------------------------------------
/*!
 * \brief Get value at given index
 */
// ----------------------------------------------------------------------

float NFmiRawData::GetValue(size_t index) const { return itsPimple->GetValue(index); }
// ----------------------------------------------------------------------
/*!
 * \brief Set value at given index
 */
// ----------------------------------------------------------------------

bool NFmiRawData::SetValue(size_t index, float value) { return itsPimple->SetValue(index, value); }
// ----------------------------------------------------------------------
/*!
 * \brief Set binary storage mode based on given flag
 */
// ----------------------------------------------------------------------

void NFmiRawData::SetBinaryStorage(bool flag) const { itsPimple->SetBinaryStorage(flag); }
// ----------------------------------------------------------------------
/*!
 * \brief Is binary storage mode on?
 */
// ----------------------------------------------------------------------

bool NFmiRawData::IsBinaryStorageUsed() const { return itsPimple->IsBinaryStorageUsed(); }
// ----------------------------------------------------------------------
/*!
 * \brief Write raw data
 */
// ----------------------------------------------------------------------

ostream &NFmiRawData::Write(ostream &file) const { return itsPimple->Write(file); }
// ----------------------------------------------------------------------
/*!
 * \brief Backup data to given pointer
 */
// ----------------------------------------------------------------------

void NFmiRawData::Backup(char *ptr) const { itsPimple->Backup(ptr); }
// ----------------------------------------------------------------------
/*!
 * \brief Restore data from given pointer
 */
// ----------------------------------------------------------------------

void NFmiRawData::Undo(char *ptr) { itsPimple->Undo(ptr); }
// ----------------------------------------------------------------------
/*!
 * \brief Advise memory mapped region
 *
 * Returns false on failure.
 */
// ----------------------------------------------------------------------

bool NFmiRawData::Advise(FmiAdvice theAdvice) { return itsPimple->Advise(theAdvice); }
// ======================================================================
