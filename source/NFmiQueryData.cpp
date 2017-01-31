// ======================================================================
/*!
 * \file NFmiQueryData.cpp
 * \brief Implementation of class NFmiQueryData
 */
// ======================================================================
/*!
 * \class NFmiQueryData
 *
 * Undocumented
 *
 */
// ======================================================================

#include "NFmiVersion.h"

#include "NFmiQueryData.h"
#include "NFmiQueryInfo.h"
#include "NFmiGrid.h"
#include "NFmiFileSystem.h"
#include "NFmiStationBag.h"
#include "NFmiSaveBaseFactory.h"

#include <boost/make_shared.hpp>

#include <fstream>
#include <fcntl.h>

#ifndef UNIX
#include <io.h>
#endif

#ifdef FMI_COMPRESSION
#include <boost/algorithm/string/predicate.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/bzip2.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#endif

//#ifndef  NDEBUG
int NFmiQueryData::itsConstructorCalls = 0;
int NFmiQueryData::itsDestructorCalls = 0;
//#endif // NDEBUG

using namespace std;

// Staattiset versiot querydatan luku/kirjoituksesta, ottavat huomioon mm. VC++:n bin��ri
// asetuksista.
// Voivat heitt�� poikkeuksia.

static void ForceBinaryFormatWrite(const NFmiQueryData &queryData)
{
  // binaryformat tarkoittaa queryDatan talletus formaattia, k�yt� aina bin��ri formaattia tehokkuus
  // syist�, jos mahdollista.
  queryData.UseBinaryStorage(true);
  if (queryData.InfoVersion() < 6.)  // jos datan infoversio on alle 6, pit�� muuttaa 6:ksi
    queryData.InfoVersion(6.);       // Huom! ei voi muuttaa 7:ksi tai ylemm�ksi, koska se saattaa
                                     // sekoittaaa yhdistelm�parametrien (W&C ja TotWind) arvot
}

void NFmiQueryData::Read()
{
// Muunnetaan "stdin" bin��ri moodiin --> pystyy lukemaan bin��ri�
#ifdef _MSC_VER
  int result = ::_setmode(_fileno(stdin), _O_BINARY);
  if (result == -1)
    throw std::runtime_error(
        "Error in NFmiQueryData::Read: Could not set standard input into binary mode.");
#endif
  cin >> *this;

  if (!cin.good())
    throw std::runtime_error("Error in NFmiQueryData::Read, while reading standard input.");
}

void NFmiQueryData::Write(bool forceBinaryFormat) const
{
  if (forceBinaryFormat) ::ForceBinaryFormatWrite(*this);

// Asetetaan 'stdout' bin��ri moodiin --> kirjoittaa bin��ri�
#ifdef _MSC_VER
  int result = ::_setmode(_fileno(stdout), _O_BINARY);
  if (result == -1)
    throw std::runtime_error(
        "Error in NFmiQueryData::WriteCout: Could not set standard input into binary mode.");
#endif

  cout << *this;

  if (!cout.good())
    throw std::runtime_error("Error in NFmiQueryData::Write,while writing to the standard output.");
}

void NFmiQueryData::Write(const std::string &filename, bool forceBinaryFormat) const
{
  if (filename == "-")
    Write(forceBinaryFormat);
  else
  {
    if (forceBinaryFormat) ::ForceBinaryFormatWrite(*this);

    ofstream dataFile(filename.c_str(), ios::binary | ios::out);
    if (dataFile)
      dataFile << *this;
    else
      throw std::runtime_error(
          std::string("Error in NFmiQueryData::Write: Could not write in file '") + filename +
          "'.");

    if (!dataFile.good())
      throw std::runtime_error(
          std::string("Error in NFmiQueryData::Write, while writing to file '") + filename + "'.");
  }
}

// ----------------------------------------------------------------------
/*!
 * Destructor
 */
// ----------------------------------------------------------------------

NFmiQueryData::~NFmiQueryData()
{
  Destroy();
  //#ifndef  NDEBUG
  NFmiQueryData::itsDestructorCalls++;
  //#endif // NDEBUG
}

// ----------------------------------------------------------------------
/*!
 * Constructor
 */
// ----------------------------------------------------------------------

NFmiQueryData::NFmiQueryData()
    : itsRawData(0),
      itsFirst(false),
      itsQueryInfo(0),
      itsLatLonCache(),
      itsLatLonCacheFlag(BOOST_ONCE_INIT)
{
  //#ifndef  NDEBUG
  NFmiQueryData::itsConstructorCalls++;
  //#endif // NDEBUG
}

// ----------------------------------------------------------------------
/*!
 * Constructor
 *
 * \param theInfo Undocumented
 * \param theType Undocumented
 */
// ----------------------------------------------------------------------

NFmiQueryData::NFmiQueryData(const NFmiQueryInfo &theInfo)
    : itsRawData(new NFmiRawData()),
      itsFirst(false),
      itsQueryInfo(new NFmiQueryInfo(theInfo)),
      itsLatLonCache(),
      itsLatLonCacheFlag(BOOST_ONCE_INIT)
{
  //#ifndef  NDEBUG
  NFmiQueryData::itsConstructorCalls++;
  //#endif // NDEBUG
}

// ----------------------------------------------------------------------
/*!
 * Copy constructor
 *
 * \param theData The object being copied
 */
// ----------------------------------------------------------------------

NFmiQueryData::NFmiQueryData(const NFmiQueryData &theData)
    : itsRawData(new NFmiRawData(*theData.itsRawData)),
      itsFirst(theData.itsFirst),
      itsQueryInfo(new NFmiQueryInfo(*theData.itsQueryInfo)),
      itsLatLonCache(theData.itsLatLonCache),
      itsLatLonCacheFlag(theData.itsLatLonCacheFlag)
{
  //#ifndef  NDEBUG
  NFmiQueryData::itsConstructorCalls++;
  //#endif // NDEBUG
}

// ----------------------------------------------------------------------
/*!
 * \brief Construct from file
 */
// ----------------------------------------------------------------------

NFmiQueryData::NFmiQueryData(const string &thePath, bool theMemoryMapFlag)
    : itsRawData(0), itsFirst(false), itsQueryInfo(0), itsLatLonCacheFlag(BOOST_ONCE_INIT)
{
  //#ifndef  NDEBUG
  NFmiQueryData::itsConstructorCalls++;
  //#endif // NDEBUG

  // Filename "-" implies standard input

  if (thePath == "-")
  {
    // Laitetaan yleinen infoversio numero talteen, ett� se
    // voidaan palauttaa luvun j�lkeen voimaan taas
    unsigned short oldInfoVersion = FmiInfoVersion;
    Read();
    // palautetaan yleinen infoversion takaisen ennalleen
    FmiInfoVersion = oldInfoVersion;
  }
  else
  {
    // If the string is a directory, scan it for the latest querydata

    const string filename = NFmiFileSystem::FindQueryData(thePath);

    // Laitetaan yleinen infoversio numero talteen, ett� se
    // voidaan palauttaa luvun j�lkeen voimaan taas
    unsigned short oldInfoVersion = FmiInfoVersion;

    try
    {
      ifstream file(filename.c_str(), ios::in | ios::binary);
      if (!file) throw runtime_error("Could not open '" + filename + "' for reading");

      itsQueryInfo = new NFmiQueryInfo;

#ifdef FMI_COMPRESSION
      if (NFmiFileSystem::IsCompressed(filename))
      {
        using namespace boost;
        using namespace boost::iostreams;
        filtering_stream<input> filter;
        if (iends_with(filename, ".gz"))
          filter.push(gzip_decompressor());
        else if (iends_with(filename, ".bz2"))
          filter.push(bzip2_decompressor());
        filter.push(file);
        filter >> *itsQueryInfo;
        itsRawData =
            new NFmiRawData(filter, itsQueryInfo->Size(), itsQueryInfo->DoEndianByteSwap());
        if (!filter.good()) throw runtime_error("Error while reading '" + filename + "'");
      }
      else
#endif
      {
        // Olion sis�inen infoversio numero j�� itsQueryInfo:on talteen.

        file >> *itsQueryInfo;

        bool use_mmap = theMemoryMapFlag;

        if (FmiInfoVersion < 6) use_mmap = false;

        if (itsQueryInfo->DoEndianByteSwap()) use_mmap = false;

        if (use_mmap)
        {
          itsRawData = new NFmiRawData(filename, file, itsQueryInfo->Size());
        }
        else
        {
          itsRawData =
              new NFmiRawData(file, itsQueryInfo->Size(), itsQueryInfo->DoEndianByteSwap());
        }

        if (!file.good()) throw runtime_error("Error while reading '" + filename + "'");
        file.close();
      }
    }
    catch (...)
    {
      // palautetaan yleinen infoversion takaisen ennalleen
      delete itsQueryInfo;
      delete itsRawData;
      FmiInfoVersion = oldInfoVersion;
      throw;
    }

    // palautetaan yleinen infoversion takaisen ennalleen
    FmiInfoVersion = oldInfoVersion;
  }
}

// ----------------------------------------------------------------------
/*!
 * \brief Swap contents. Not thread safe!
 */
// ----------------------------------------------------------------------

void NFmiQueryData::swap(NFmiQueryData &theOther)
{
  std::swap(itsRawData, theOther.itsRawData);
  std::swap(itsFirst, theOther.itsFirst);
  std::swap(itsQueryInfo, theOther.itsQueryInfo);
  std::swap(itsLatLonCache, theOther.itsLatLonCache);
  std::swap(itsLatLonCacheFlag, theOther.itsLatLonCacheFlag);
}

// ----------------------------------------------------------------------
/*!
 *
 */
// ----------------------------------------------------------------------

void NFmiQueryData::Destroy(void)
{
  if (itsQueryInfo) delete itsQueryInfo;
  if (itsRawData) delete itsRawData;
}

// ----------------------------------------------------------------------
/*!
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiQueryData::Init(void)
{
  bool status = (itsQueryInfo && itsRawData->Init(itsQueryInfo->Size()));
  return status;
}

// ----------------------------------------------------------------------
/*!
 * \param theInfo Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiQueryData::Init(const NFmiQueryInfo &theInfo)
{
  if (itsQueryInfo) delete itsQueryInfo;

  itsQueryInfo = theInfo.Clone();
  if (itsRawData) delete itsRawData;

  itsRawData = new NFmiRawData();
  itsRawData->Init(itsQueryInfo->Size());

  return (itsQueryInfo != 0);
}

// ----------------------------------------------------------------------
/*!
 * \brief Initialize memory mapped querydata
 */
// ----------------------------------------------------------------------

bool NFmiQueryData::Init(const std::string &theHeader,
                         const std::string &theFilename,
                         bool fInitialize)
{
  if (!itsQueryInfo) return false;

  return itsRawData->Init(itsQueryInfo->Size(), theHeader, theFilename, fInitialize);
}

// ----------------------------------------------------------------------
/*!
 * \return Undocumented
 * \todo Should return an boost::shared_ptr
 */
// ----------------------------------------------------------------------

NFmiQueryData *NFmiQueryData::Clone(void) const { return new NFmiQueryData(*this); }
// ----------------------------------------------------------------------
/*!
 *
 */
// ----------------------------------------------------------------------

void NFmiQueryData::Reset()
{
  itsFirst = false;
  itsQueryInfo->Reset();
  //  itsDataPool->Reset();
}

// ----------------------------------------------------------------------
/*!
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiQueryData::First()
{
  Reset();
  NextParam();
  NextTime();
  NextLocation();
  NextLevel();
  itsFirst = true;
  return true;
}

// ----------------------------------------------------------------------
/*!
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiQueryData::Next()
{
  if (!itsFirst) return First();

  if (NextParam()) return true;

  ResetParam();
  NextParam();
  if (NextLocation()) return true;

  ResetLocation();
  NextLocation();
  if (NextTime()) return true;

  return false;
}

// ----------------------------------------------------------------------
/*!
 * \return Undocumented
 */
// ----------------------------------------------------------------------

const NFmiMetTime NFmiQueryData::Time(void) const
{
  if (itsQueryInfo->itsTimeDescriptor->IsValidTime())
    return itsQueryInfo->itsTimeDescriptor->ValidTime();

  return itsQueryInfo->itsTimeDescriptor->OriginTime();
}

// ----------------------------------------------------------------------
/*!
 * \return Undocumented
 */
// ----------------------------------------------------------------------

float NFmiQueryData::FloatValue(void) { return itsRawData->GetValue(itsQueryInfo->Index()); }
// ----------------------------------------------------------------------
/*!
 * \return Undocumented
 */
// ----------------------------------------------------------------------

const NFmiGrid &NFmiQueryData::GridInfo(void) { return *Info()->Grid(); }
// ----------------------------------------------------------------------
/*!
 * Equality comparison
 *
 * \param theQueryData The object being compared to
 * \return True if the objects are equal
 */
// ----------------------------------------------------------------------

bool NFmiQueryData::operator==(NFmiQueryData &theQueryData) { return IsEqual(theQueryData); }
// ----------------------------------------------------------------------
/*!
 * Assignment operator
 *
 * \param theQueryData The object being copied
 * \return The object assigned to
 * \bug Must protect from self assignment
 */
// ----------------------------------------------------------------------

NFmiQueryData &NFmiQueryData::operator=(const NFmiQueryData &theQueryData)
{
  Destroy();
  itsQueryInfo = new NFmiQueryInfo(*theQueryData.itsQueryInfo);
  itsRawData = new NFmiRawData(*theQueryData.itsRawData);
  itsFirst = theQueryData.itsFirst;

  return *this;
}

// ----------------------------------------------------------------------
/*!
 * \return Undocumented
 */
// ----------------------------------------------------------------------

float NFmiQueryData::Quality() const
{
  size_t totalsize = itsRawData->Size();
  size_t oksize = 0;

  for (size_t index = 0; index < totalsize; index++)
    if (itsRawData->GetValue(index) != kFloatMissing) oksize++;

  return oksize * 100.0f / totalsize;
}

// ----------------------------------------------------------------------
/*!
 * \return Undocumented
 */
// ----------------------------------------------------------------------

const NFmiString NFmiQueryData::Header1(void) const
{
  itsQueryInfo->ResetText();

  if (itsQueryInfo->NextText()) return itsQueryInfo->Text();

  return NFmiString();
}

// ----------------------------------------------------------------------
/*!
 * \return Undocumented
 */
// ----------------------------------------------------------------------

const NFmiString NFmiQueryData::Header2(void) const
{
  itsQueryInfo->ResetText();

  if (itsQueryInfo->NextText())
  {
    if (itsQueryInfo->NextText())
    {
      return itsQueryInfo->Text();
    }
  }
  return NFmiString();
}

// ----------------------------------------------------------------------
/*!
 * \return Undocumented
 */
// ----------------------------------------------------------------------

const NFmiString NFmiQueryData::Header3(void) const
{
  itsQueryInfo->ResetText();

  if (itsQueryInfo->NextText())
  {
    if (itsQueryInfo->NextText())
    {
      if (itsQueryInfo->NextText())
      {
        return itsQueryInfo->Text();
      }
    }
  }
  return NFmiString();
}

// ----------------------------------------------------------------------
/*!
 * \param theQueryData Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiQueryData::IsEqual(const NFmiQueryData &theQueryData) const
{
  return (itsQueryInfo == theQueryData.itsQueryInfo && itsRawData == theQueryData.itsRawData);
}

// ----------------------------------------------------------------------
/*!
 * Write the object to the given output stream
 *
 * \param file The output stream to write to
 * \return The output stream written to
 */
// ----------------------------------------------------------------------

std::ostream &NFmiQueryData::Write(std::ostream &file) const
{
  double oldInfoVersion = FmiInfoVersion;  // Laitetaan yleinen infoversio numero talteen, ett� se
                                           // voidaan palauttaa luvun j�lkeen voimaan taas
  FmiInfoVersion = static_cast<unsigned short>(InfoVersion());

  if (InfoVersion() >= 6) UseBinaryStorage(true);

  file << *itsQueryInfo;
  itsRawData->Write(file);

  FmiInfoVersion = static_cast<unsigned short>(
      oldInfoVersion);  // palautetaan yleinen infoversion takaisen ennalleen
  return file;
}

// ----------------------------------------------------------------------
/*!
 * Read new object contents from the given input stream
 *
 * \param file The input stream to read from
 * \return The input stream read from
 */
// ----------------------------------------------------------------------

std::istream &NFmiQueryData::Read(std::istream &file)
{
  // Laitetaan yleinen infoversio numero talteen, ett� se
  // voidaan palauttaa luvun j�lkeen voimaan taas
  unsigned short oldInfoVersion = FmiInfoVersion;

  // Olion sis�inen infoversio numero j�� itsQueryInfo:on talteen.
  itsQueryInfo = new NFmiQueryInfo();
  file >> *itsQueryInfo;

  itsRawData = new NFmiRawData(file, itsQueryInfo->Size(), itsQueryInfo->DoEndianByteSwap());

  // palautetaan yleinen infoversion takaisen ennalleen
  FmiInfoVersion = oldInfoVersion;

  return file;
}

// THIS IS NOT THREAD SAFE!!
void NFmiQueryData::SetHPlaceDescriptor(const NFmiHPlaceDescriptor &newDesc)
{
  if ((Info()->HPlaceDescriptor() == newDesc) == false)
  {
    itsQueryInfo->SetHPlaceDescriptor(newDesc);
    MakeLatLonCache();  // t�m� alustaa latlon cachen uudestaan
  }
}

// ----------------------------------------------------------------------
/*!
 * \brief Advise memory mapped region
 *
 * Returns false on failure.
 */
// ----------------------------------------------------------------------

bool NFmiQueryData::Advise(FmiAdvice theAdvice)
{
  if (!itsRawData) return false;

  return itsRawData->Advise(theAdvice);
}

// ----------------------------------------------------------------------
/*!
 * \brief Return the latlon cache
 *
 * Note: This must not be done in constructors, since smartmet-server
 * reads thousands of radar files and holding the coordinates
 * for all of them would take > 10 GB.
 */
// ----------------------------------------------------------------------

boost::shared_ptr<std::vector<NFmiPoint> > NFmiQueryData::LatLonCache() const
{
  // If not already set by SetLatLonCache, initialize once
  if (!itsLatLonCache)
    boost::call_once(boost::bind(&NFmiQueryData::MakeLatLonCache, this), itsLatLonCacheFlag);
  return itsLatLonCache;
}

// ----------------------------------------------------------------------
/*!
 * \brief Set the latlon cache
 *
 * Note: smartmet-server caches the created LatLonCaches and uses this
 *       method to set the same value for all equivalent grids.
 *
 * Note: We assume there is no race condition between LatLonCache()
 *       SetLatLonCache(), code should always call the latter before
 *       allowing anything else to request access to the cache.
 */
// ----------------------------------------------------------------------

void NFmiQueryData::SetLatLonCache(boost::shared_ptr<std::vector<NFmiPoint> > newCache)
{
  itsLatLonCache = newCache;
}

// ----------------------------------------------------------------------
/*!
 * Initialize coordinate cache
 *
 * \return Coordinate cache
 */
// ----------------------------------------------------------------------

void NFmiQueryData::MakeLatLonCache() const
{
  itsLatLonCache = boost::make_shared<std::vector<NFmiPoint> >();
  HPlaceDesc()->CreateLatLonCache(*itsLatLonCache);
}

// ----------------------------------------------------------------------
/*!
 * \brief Unique hash value for the grid
 *
 * Used by smartmet-server to cache LatLonCache objects and set them
 * to newly read NFmiQueryData using SetLatLonCache so that MakeLatLonCache
 * is not needed and data can be shared between similar objects.
 */
// ----------------------------------------------------------------------

std::size_t NFmiQueryData::GridHashValue() const { return itsQueryInfo->GridHashValue(); }
