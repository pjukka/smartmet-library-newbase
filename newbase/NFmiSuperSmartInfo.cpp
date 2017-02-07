// ======================================================================
/*!
 * \file NFmiSuperSmartInfo.cpp
 * \brief Implementation of class NFmiSuperSmartInfo
 */
// ======================================================================
/*!
 * \class NFmiSuperSmartInfo
 *
 * Undocumented
 *
 */
// ======================================================================

#include "NFmiSuperSmartInfo.h"
#include "NFmiGrid.h"
#include "NFmiCalculator.h"
#include "NFmiAreaMask.h"
#include "NFmiDataModifierDescriptor.h"
#include "NFmiDataModifierClasses.h"
#include "NFmiParamDataModifier.h"
#include "NFmiRelativeDataIterator.h"
#include "NFmiBitmapAreaMask.h"

using namespace std;

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

NFmiSuperSmartInfo::NFmiSuperSmartInfo(void)
    : NFmiFastQueryInfo(),
      itsFloatValueAccessLevel(kNormalAccess),
      fUseCalculator(false),
      fUseVarianceCalculator(false),
      fUseVariationCalculator(false),
      fUseTimeIntegrationCalculator(false),
      itsCalculator(0),
      itsVariationCalculator(0)  // laitetaan t‰‰ll‰ tyhj‰t calculaattorit valmiiksi
      ,
      itsTimeIntegrationCalculator(0),
      itsCurrentVarianceCalculator(new NFmiCalculator(
          static_cast<NFmiQueryInfo*>(0), 0))  // laitetaan t‰‰ll‰ tyhj‰t calculaattorit valmiiksi
      ,
      itsDataModifierDescriptor(0),
      itsAreaMask(0),
      fUseAreaMask(false),
      fUseHelperBinaryMasks(false),
      itsCurrentHelperBinaryMask(0),
      itsXYMaskBoundingBox(),
      itsHelperBinaryMaskList()
{
}

// ----------------------------------------------------------------------
/*!
 * Constructor
 *
 * \param theParamDescriptor Undocumented
 * \param theTimeDescriptor Undocumented
 * \param theHPlaceDescriptor Undocumented
 * \param theVPlaceDescriptor Undocumented
 */
// ----------------------------------------------------------------------

NFmiSuperSmartInfo::NFmiSuperSmartInfo(const NFmiParamDescriptor& theParamDescriptor,
                                       const NFmiTimeDescriptor& theTimeDescriptor,
                                       const NFmiHPlaceDescriptor& theHPlaceDescriptor,
                                       const NFmiVPlaceDescriptor& theVPlaceDescriptor)
    : NFmiFastQueryInfo(
          theParamDescriptor, theTimeDescriptor, theHPlaceDescriptor, theVPlaceDescriptor),
      itsFloatValueAccessLevel(kNormalAccess),
      fUseCalculator(false),
      fUseVarianceCalculator(false),
      fUseVariationCalculator(false),
      fUseTimeIntegrationCalculator(false),
      itsCalculator(0),
      itsVariationCalculator(0)  // laitetaan t‰‰ll‰ tyhj‰t calculaattorit valmiiksi
      ,
      itsTimeIntegrationCalculator(0),
      itsCurrentVarianceCalculator(new NFmiCalculator(
          static_cast<NFmiQueryInfo*>(0), 0))  // laitetaan t‰‰ll‰ tyhj‰t calculaattorit valmiiksi
      ,
      itsDataModifierDescriptor(0),
      itsAreaMask(0),
      fUseAreaMask(false),
      fUseHelperBinaryMasks(false),
      itsCurrentHelperBinaryMask(0),
      itsXYMaskBoundingBox(),
      itsHelperBinaryMaskList()
{
}

// ----------------------------------------------------------------------
/*!
 * Constructor
 *
 * \param data Undocumented
 * \param theParamDescriptor Undocumented
 * \param theTimeDescriptor Undocumented
 * \param theHPlaceDescriptor Undocumented
 * \param theVPlaceDescriptor Undocumented
 */
// ----------------------------------------------------------------------

NFmiSuperSmartInfo::NFmiSuperSmartInfo(NFmiQueryData* data,
                                       NFmiParamDescriptor* theParamDescriptor,
                                       NFmiTimeDescriptor* theTimeDescriptor,
                                       NFmiHPlaceDescriptor* theHPlaceDescriptor,
                                       NFmiVPlaceDescriptor* theVPlaceDescriptor)
    : NFmiFastQueryInfo(
          data, theParamDescriptor, theTimeDescriptor, theHPlaceDescriptor, theVPlaceDescriptor),
      itsFloatValueAccessLevel(kNormalAccess),
      fUseCalculator(false),
      fUseVarianceCalculator(false),
      fUseVariationCalculator(false),
      fUseTimeIntegrationCalculator(false),
      itsCalculator(0),
      itsVariationCalculator(0)  // laitetaan t‰‰ll‰ tyhj‰t calculaattorit valmiiksi
      ,
      itsTimeIntegrationCalculator(0),
      itsCurrentVarianceCalculator(new NFmiCalculator(
          static_cast<NFmiQueryInfo*>(0), 0))  // laitetaan t‰‰ll‰ tyhj‰t calculaattorit valmiiksi
      ,
      itsDataModifierDescriptor(0),
      itsAreaMask(0),
      fUseAreaMask(false),
      fUseHelperBinaryMasks(false),
      itsCurrentHelperBinaryMask(0),
      itsXYMaskBoundingBox(),
      itsHelperBinaryMaskList()
{
}

// ----------------------------------------------------------------------
/*!
 * Constructor
 *
 * \param theInfo Undocumented
 */
// ----------------------------------------------------------------------

NFmiSuperSmartInfo::NFmiSuperSmartInfo(const NFmiQueryInfo& theInfo)
    : NFmiFastQueryInfo(theInfo),
      itsFloatValueAccessLevel(kNormalAccess),
      fUseCalculator(false),
      fUseVarianceCalculator(false),
      fUseVariationCalculator(false),
      fUseTimeIntegrationCalculator(false),
      itsCalculator(0),
      itsVariationCalculator(0)  // laitetaan t‰‰ll‰ tyhj‰t calculaattorit valmiiksi
      ,
      itsTimeIntegrationCalculator(0),
      itsCurrentVarianceCalculator(new NFmiCalculator(
          static_cast<NFmiQueryInfo*>(0), 0))  // laitetaan t‰‰ll‰ tyhj‰t calculaattorit valmiiksi
      ,
      itsDataModifierDescriptor(0),
      itsAreaMask(0),
      fUseAreaMask(false),
      fUseHelperBinaryMasks(false),
      itsCurrentHelperBinaryMask(0),
      itsXYMaskBoundingBox(),
      itsHelperBinaryMaskList()
{
}
// ----------------------------------------------------------------------
/*!
 * Copy constructor
 *
 * \param theInfo The object being copied
 */
// ----------------------------------------------------------------------

NFmiSuperSmartInfo::NFmiSuperSmartInfo(const NFmiSuperSmartInfo& theInfo)
    : NFmiFastQueryInfo(theInfo),
      itsFloatValueAccessLevel(theInfo.itsFloatValueAccessLevel),
      fUseCalculator(theInfo.fUseCalculator),
      fUseVarianceCalculator(theInfo.fUseVarianceCalculator),
      fUseVariationCalculator(theInfo.fUseVariationCalculator),
      fUseTimeIntegrationCalculator(theInfo.fUseTimeIntegrationCalculator),
      itsCalculator(theInfo.itsCalculator),
      itsVariationCalculator(
          theInfo.itsVariationCalculator)  // laitetaan t‰‰ll‰ tyhj‰t calculaattorit valmiiksi
      ,
      itsTimeIntegrationCalculator(0)
      // HUOM!!!! TƒMƒ PITƒƒ HOITAA SITTEN KUNTOON (itsCurrentVarianceCalculator)
      //,itsCurrentVarianceCalculator(new NFmiCalculator((NFmiQueryInfo *)0, 0)) // laitetaan t‰‰ll‰
      //tyhj‰t calculaattorit valmiiksi
      ,
      itsCurrentVarianceCalculator(0),
      itsDataModifierDescriptor(theInfo.itsDataModifierDescriptor),
      itsAreaMask(theInfo.itsAreaMask),
      fUseAreaMask(theInfo.fUseAreaMask),
      fUseHelperBinaryMasks(theInfo.fUseHelperBinaryMasks),
      itsCurrentHelperBinaryMask(
          0)  // t‰m‰ alustetaan (virheellisesti) CopyHelperBinaryMasksAndInfo:issa
      ,
      itsXYMaskBoundingBox()  // t‰m‰ alustetaan (virheellisesti) CopyHelperBinaryMasksAndInfo:issa
      ,
      itsHelperBinaryMaskList()  // t‰m‰ alustetaan (virheellisesti)
                                 // CopyHelperBinaryMasksAndInfo:issa
{
  //	InitHelperBinaryMasks(); // t‰m‰ on v‰‰rin kopi-konstruktorissa, mutta
  //  en jaksa tehd‰ todellista kopiota itsHelperBinaryMaskList:ta ja muista
  CopyHelperBinaryMasksAndInfo(theInfo);
}

// ----------------------------------------------------------------------
/*!
 * Destructor
 */
// ----------------------------------------------------------------------

NFmiSuperSmartInfo::~NFmiSuperSmartInfo(void) { Destroy(); }
// ----------------------------------------------------------------------
/*!
 * Kopioi helperbinarymaskit omaan k‰yttˆˆns‰
 *
 * \param theSSInfo Undocumented
 */
// ----------------------------------------------------------------------

void NFmiSuperSmartInfo::CopyHelperBinaryMasksAndInfo(const NFmiSuperSmartInfo& theSSInfo)
{
  fUseHelperBinaryMasks = theSSInfo.fUseHelperBinaryMasks;
  itsXYMaskBoundingBox = theSSInfo.itsXYMaskBoundingBox;

  const checkedVector<NFmiBitmapAreaMask*>& helperBinaryMaskList =
      theSSInfo.itsHelperBinaryMaskList;
  size_t size = helperBinaryMaskList.size();
  itsHelperBinaryMaskList.resize(size);

  // poikkeustapauksessa voi olla 0-pointteri, pit‰‰ tarkistaa kloonattaessa
  for (size_t i = 0; i < size; i++)
    if (helperBinaryMaskList[i])
      itsHelperBinaryMaskList[i] =
          static_cast<NFmiBitmapAreaMask*>(helperBinaryMaskList[i]->Clone());
  SetCurrentHelperBinaryMask();
}
// ----------------------------------------------------------------------
/*!
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiSuperSmartInfo::FirstLocation(void)
{
  // asettaa 1. maskatun locationin
  ResetLocation();
  return NextLocation();
}

// ----------------------------------------------------------------------
/*!
 * NextLocation hypp‰‰ seuraavaa maskattuun locationiin
 * (jos ei maskia, kaikki on maskattu!)
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiSuperSmartInfo::NextLocation(void)
{
  if (!fUseAreaMask) return NFmiFastQueryInfo::NextLocation();

  bool status = false;
  do
    status = NFmiFastQueryInfo::NextLocation();
  while (status && !IsLocationMasked(LocationIndex()));
  return status;
}

// ----------------------------------------------------------------------
/*!
 * PreviousLocation hypp‰‰ edelliseen maskattuun locationiin
 * (jos ei maskia, kaikki on maskattu!)
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiSuperSmartInfo::PreviousLocation(void)
{
  if (!fUseAreaMask) return NFmiFastQueryInfo::PreviousLocation();

  bool status = false;
  do
    status = NFmiFastQueryInfo::PreviousLocation();
  while (status && IsLocationMasked(LocationIndex()));
  return status;
}

// ----------------------------------------------------------------------
/*!
 * HUOM! ei etsi l‰hint‰ maskattua paikkaa viel‰ vaan vain paikan
 * (pit‰isikˆ toteuttaa?!?!?!?)
 *
 * \param theLocation Undocumented
 * \param theMaxDistance Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiSuperSmartInfo::NearestLocation(const NFmiLocation& theLocation, double theMaxDistance)
{
  return NFmiFastQueryInfo::NearestLocation(theLocation, theMaxDistance);
}

// ----------------------------------------------------------------------
/*!
 * HUOM! ei etsi l‰hint‰ maskattua paikkaa viel‰ vaan vain paikan
 * (pit‰isikˆ toteuttaa?!?!?!?)
 *
 * \param theLatLonPoint Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiSuperSmartInfo::NearestPoint(const NFmiPoint& theLatLonPoint)
{
  return NFmiFastQueryInfo::NearestPoint(theLatLonPoint);
}

// ----------------------------------------------------------------------
/*!
 * HUOM! En tied‰ pit‰isikˆ ottaa huomioon vain maskatut paikat?!?!?
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

unsigned long NFmiSuperSmartInfo::SizeLocations(void) const
{
  return NFmiFastQueryInfo::SizeLocations();
}

// ----------------------------------------------------------------------
/*!
 * HUOM! En tied‰ pit‰isikˆ ottaa huomioon vain maskatut paikat?!?!?
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

unsigned long NFmiSuperSmartInfo::SizeActiveLocations(void) const
{
  return NFmiFastQueryInfo::SizeActiveLocations();
}

// ----------------------------------------------------------------------
/*!
 * \return Undocumented
 */
// ----------------------------------------------------------------------

float NFmiSuperSmartInfo::FloatValue(void) const
{
  // K‰ytt‰j‰ on etuk‰teen m‰‰r‰nnyt mill‰ tasolla haluaa dataa
  // m‰‰r‰m‰ll‰ itsFloatValueAccessLevel.
  // HUOM!! t‰t‰ ei kannata optimoida switch-lauseella, koska
  // eniten p‰‰dyt‰‰n kuitenkin normalaccessiin!!!

  // 'normal' == raw-access eli FastInfo:n vastaava
  if (itsFloatValueAccessLevel == kNormalAccess) return RawFloatValue();

  // pyydet‰‰n varianssia
  if (itsFloatValueAccessLevel == kVarianceAccess) return VarianceFloatValue();

  // pyydet‰‰n aikaintegraatiota
  if (itsFloatValueAccessLevel == kTimeIntegrationAccess) return TimeIntegrationFloatValue();

  // ep‰varmuus laatikko laskut
  if (itsFloatValueAccessLevel == kVariationAccess) return VariationFloatValue();

  // 'integraattori'/calculaattori (korkeimman tason kutsu)
  if (itsFloatValueAccessLevel == kCalculatorAccess) return CalculationFloatValue();

  return kFloatMissing;
}

// ----------------------------------------------------------------------
/*!
 * \return Undocumented
 */
// ----------------------------------------------------------------------

NFmiCombinedParam* NFmiSuperSmartInfo::CombinedValue(void)
{
// K‰ytt‰j‰ on etuk‰teen m‰‰r‰nnyt mill‰ tasolla haluaa dataa
// m‰‰r‰m‰ll‰ itsFloatValueAccessLevel.
// kommenteissa on oikea k‰yntij‰rjestys. Jos joku lis‰‰ jonkun
// haaran k‰yttˆˆn (esim. TimeIntegrationCombinedValue:n), pit‰‰
// se kommentoida esiin ett‰ 'vesiputous' toimisi oikeassa j‰rjestyksess‰.

#if 0
  if(itsFloatValueAccessLevel == kNormalAccess)
	return 0;//RawFloatValue();
  if(itsFloatValueAccessLevel == kVarianceAccess)
	return 0;//VarianceCombinedValue();
  if(itsFloatValueAccessLevel == kTimeIntegrationAccess)
	return 0;//TimeIntegrationCombinedValue();
#endif

  if (itsFloatValueAccessLevel == kVariationAccess) return VariationCombinedValue();
  if (itsFloatValueAccessLevel == kCalculatorAccess) return CalculationCombinedValue();
  return 0;

#if 0
  if(itsFloatValueAccessLevel == kCalculatorAccess)
	return CalculationCombinedValue();
  if(itsFloatValueAccessLevel == kVariationAccess)
	return VariationCombinedValue();
  if(itsFloatValueAccessLevel == kTimeIntegrationAccess)
	return 0;//TimeIntegrationCombinedValue();
  if(itsFloatValueAccessLevel == kVarianceAccess)
	return 0;//VarianceCombinedValue();
  return 0;//RawFloatValue();

#endif
}

// ----------------------------------------------------------------------
/*!
 * \param doVariationCalculation Undocumented
 * \param doCalculation Undocumented
 * \param doTimeIntegration Undocumented
 * \param doVarianceCalculation Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

float NFmiSuperSmartInfo::FloatValue(bool doVariationCalculation,
                                     bool doCalculation,
                                     bool doTimeIntegration,
                                     bool doVarianceCalculation)
{
  bool oldUseCalc = fUseCalculator;
  bool oldUseVari = fUseVariationCalculator;
  bool oldUseTimeIntegration = fUseTimeIntegrationCalculator;
  bool oldUseVariance = fUseVarianceCalculator;
  DataAccessLevel oldAccessLevel = itsFloatValueAccessLevel;

  itsFloatValueAccessLevel = kCalculatorAccess;

  fUseCalculator = doCalculation;
  fUseVariationCalculator = doVariationCalculation;
  fUseTimeIntegrationCalculator = doTimeIntegration;
  fUseVarianceCalculator = doVarianceCalculation;

  float value = FloatValue();

  fUseCalculator = oldUseCalc;
  fUseVariationCalculator = oldUseVari;
  fUseTimeIntegrationCalculator = oldUseTimeIntegration;
  fUseVarianceCalculator = oldUseVariance;
  itsFloatValueAccessLevel = oldAccessLevel;
  return value;
}

// ----------------------------------------------------------------------
/*!
 * \param doVariationCalculation Undocumented
 * \param doCalculation Undocumented
 * \param doTimeIntegration Undocumented
 * \param doVarianceCalculation Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

NFmiCombinedParam* NFmiSuperSmartInfo::CombinedValue(bool doVariationCalculation,
                                                     bool doCalculation,
                                                     bool doTimeIntegration,
                                                     bool doVarianceCalculation)
{
  bool oldUseCalc = fUseCalculator;
  bool oldUseVari = fUseVariationCalculator;
  bool oldUseTimeIntegration = fUseTimeIntegrationCalculator;
  bool oldUseVariance = fUseVarianceCalculator;
  DataAccessLevel oldAccessLevel = itsFloatValueAccessLevel;
  itsFloatValueAccessLevel = kCalculatorAccess;

  fUseCalculator = doCalculation;
  fUseVariationCalculator = doVariationCalculation;
  fUseTimeIntegrationCalculator = doTimeIntegration;
  fUseVarianceCalculator = doVarianceCalculation;

  NFmiCombinedParam* value = 0;
  value = CombinedValue();

  fUseCalculator = oldUseCalc;
  fUseVariationCalculator = oldUseVari;
  fUseTimeIntegrationCalculator = oldUseTimeIntegration;
  fUseVarianceCalculator = oldUseVariance;
  itsFloatValueAccessLevel = oldAccessLevel;
  return value;
}

// ----------------------------------------------------------------------
/*!
 * \param theXOffset Undocumented
 * \param theYOffset Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

float NFmiSuperSmartInfo::PeekLocationValue(int theXOffset, int theYOffset) const
{
  // Peek...Value()-metodit pit‰‰ tehd‰ niin, ett‰ alkuper‰iset
  // indeksit otetaan talteen, ett‰ tilanne voidaan palauttaa
  // datan kyselyn j‰lkeen.
  // Eri access-tasoisia Calculaattoreita varten paikka pit‰‰
  // asettaa ensin kohdalleen, ett‰ Calculaattorit toimisivat oikein.

  unsigned long oldLocationIndex = LocationIndex();
  int wantedIndex = CalcPeekLocationIndex(oldLocationIndex, theXOffset, theYOffset);
  if (!const_cast<NFmiSuperSmartInfo*>(this)->LocationIndex(wantedIndex)) return kFloatMissing;

  float value = FloatValue();  // FloatValue hoitaa nyt 'peekkauksen'
  const_cast<NFmiSuperSmartInfo*>(this)->LocationIndex(
      oldLocationIndex);  // aseta vanhat indeksit paikoilleen eli info vanhaan 'asentoon'
  return value;
}

// ----------------------------------------------------------------------
/*!
 *†\param theTimeOffset Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

float NFmiSuperSmartInfo::PeekTimeValue(int theTimeOffset)
{
  // Peek...Value()-metodit pit‰‰ tehd‰ niin, ett‰ alkuper‰iset
  // indeksit otetaan talteen, ett‰ tilanne voidaan palauttaa datan
  // kyselyn j‰lkeen. Eri access-tasoisia Calculaattoreita varten
  // paikka pit‰‰ asettaa ensin kohdalleen, ett‰ Calculaattorit
  // toimisivat oikein.

  unsigned long oldTimeIndex = TimeIndex();

  // jos ep‰onnistui, palauta missing arvo, ei tarvitse asettaa oldTimeIndex:i‰
  if (!TimeIndex(oldTimeIndex + theTimeOffset))
  {
    TimeIndex(oldTimeIndex);  // aseta vanhat indeksit paikoilleen
    return kFloatMissing;
  }
  float value = FloatValue();  // FloatValue hoitaa nyt 'peekkauksen'
  TimeIndex(oldTimeIndex);     // aseta vanhat indeksit paikoilleen
  return value;
}

// ----------------------------------------------------------------------
/*!
 * \param theTimeOffset Undocumented
 * \param theXOffset Undocumented
 * \param theYOffset Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

float NFmiSuperSmartInfo::PeekValue(int theTimeOffset, int theXOffset, int theYOffset)
{
  // Peek...Value()-metodit pit‰‰ tehd‰ niin, ett‰ alkuper‰iset
  // indeksit otetaan talteen,ett‰ tilanne voidaan palauttaa datan
  // kyselyn j‰lkeen. Eri access-tasoisia Calculaattoreita varten
  // paikka pit‰‰ asettaa ensin kohdalleen, ett‰ Calculaattorit
  // toimisivat oikein.

  unsigned long oldLocationIndex = LocationIndex();
  unsigned long oldTimeIndex = TimeIndex();
  int wantedIndex = CalcPeekLocationIndex(oldLocationIndex, theXOffset, theYOffset);
  bool status1 = LocationIndex(wantedIndex);
  bool status2 = TimeIndex(oldTimeIndex + theTimeOffset);
  float value =
      (status1 && status2) ? FloatValue() : kFloatMissing;  // FloatValue hoitaa nyt 'peekkauksen'
  LocationIndex(oldLocationIndex);  // aseta vanhat indeksit paikoilleen eli info vanhaan 'asentoon'
  TimeIndex(oldTimeIndex);          // aseta vanhat indeksit paikoilleen eli info vanhaan 'asentoon'
  return value;
}

// ----------------------------------------------------------------------
/*!
 * \param currentIndex Undocumented
 * \param theXOffset Undocumented
 * \param theYOffset Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

unsigned long NFmiSuperSmartInfo::CalcPeekLocationIndex(unsigned long currentIndex,
                                                        int theXOffset,
                                                        int theYOffset) const
{
  if (!IsGrid()) return (currentIndex + theXOffset);

  int currentXIndex = (currentIndex % itsGridXNumber) + theXOffset;
  int currentYIndex = (currentIndex / itsGridXNumber) + theYOffset;

  // voiko t‰m‰n seuraavan tarkistuksen poistaa, kun indeksi
  // tarkistetaan kuitenkin Index-metodissa??

  // x- ja y-indeksien pit‰‰ pysy‰ gridin sis‰ll‰ offsettien kera!
  if (currentXIndex >= 0 && currentYIndex >= 0 && currentXIndex < int(itsGridXNumber) &&
      currentYIndex < int(itsGridYNumber))
    return (currentYIndex * itsGridXNumber + currentXIndex);

  // palauttaa -1 indeksin, jos peek menee hilaruudukon ulkopuolelle
  return static_cast<unsigned long>(-1);
}

// ----------------------------------------------------------------------
/*!
 * Laskee kurrentista location indeksist‰ (1-ulotteinen) kaksi
 * ulotteiseksi x ja y-indeksiksi. Indeksit alkavat 0:sta, virhetilanteessa
 * x ja y on -1:i‰
 *
 * \param theIndex Undocumented
 * \param theXOffset Undocumented
 * \param theYOffset Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiSuperSmartInfo::LocationIndex2XYPosition(unsigned long theIndex,
                                                  int* theXOffset,
                                                  int* theYOffset)
{
  if (theIndex > itsLocationSize)
  {
    *theXOffset = -1;
    *theYOffset = -1;
    return false;
  }
  else if (IsGrid())
  {
    *theXOffset = (theIndex % itsGridXNumber);
    *theYOffset = (theIndex / itsGridXNumber);
  }
  else
  {
    *theXOffset = theIndex;
    *theYOffset = -1;
  }
  return true;
}

// ----------------------------------------------------------------------
/*!
 * \return Undocumented
 * \todo Should return boost::shared_ptr
 */
// ----------------------------------------------------------------------

NFmiQueryInfo* NFmiSuperSmartInfo::Clone(void) const { return new NFmiSuperSmartInfo(*this); }
// ----------------------------------------------------------------------
/*!
 * Write the object to the given output stream
 *
 * \param file The output stream to write to
 * \return The output stream written to
 */
// ----------------------------------------------------------------------

std::ostream& NFmiSuperSmartInfo::Write(std::ostream& file) const
{
  NFmiFastQueryInfo::Write(file);
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

std::istream& NFmiSuperSmartInfo::Read(std::istream& file)
{
  NFmiFastQueryInfo::Read(file);
  return file;
}

// ----------------------------------------------------------------------
/*!
 * Assignment operaor
 *
 * \param theInfo The object being copied
 * \return The object assigned to
 */
// ----------------------------------------------------------------------

NFmiSuperSmartInfo& NFmiSuperSmartInfo::operator=(const NFmiSuperSmartInfo& theInfo)
{
  if (this != &theInfo)
  {
    NFmiFastQueryInfo::operator=(theInfo);

    itsFloatValueAccessLevel = theInfo.itsFloatValueAccessLevel;
    fUseCalculator = theInfo.fUseCalculator;
    fUseVarianceCalculator = theInfo.fUseVarianceCalculator;
    fUseVariationCalculator = theInfo.fUseVariationCalculator;
    fUseTimeIntegrationCalculator = theInfo.fUseTimeIntegrationCalculator;
    itsCalculator = theInfo.itsCalculator;
    itsVariationCalculator = theInfo.itsVariationCalculator;

    // HUOM!!!! TƒMƒ PITƒƒ HOITAA SITTEN KUNTOON
    // itsCurrentVarianceCalculator = theInfo.itsCurrentVarianceCalculator;

    itsDataModifierDescriptor = theInfo.itsDataModifierDescriptor;
    itsAreaMask = theInfo.itsAreaMask;
    fUseAreaMask = theInfo.fUseAreaMask;

    // t‰m‰ alustetaan (virheellisesti) CopyHelperBinaryMasksAndInfo:issa
    itsCurrentHelperBinaryMask = 0;
    CopyHelperBinaryMasksAndInfo(theInfo);
  }
  return *this;
}

// ----------------------------------------------------------------------
/*!
 * \brief Tuhoa sis‰iset dynaamiset muuttujat
 */
// ----------------------------------------------------------------------

void NFmiSuperSmartInfo::Destroy(void)
{
  delete itsCurrentVarianceCalculator;
  ClearHelperBinaryMasks();

  NFmiFastQueryInfo::Destroy();
}

// ----------------------------------------------------------------------
/*!
 * \return Undocumented
 */
// ----------------------------------------------------------------------

size_t NFmiSuperSmartInfo::Index(void) const
{
  // t‰m‰ saattaa muuttua, jos datan j‰rjestyst‰ muutetaan
  // (locationit sisimm‰iseen looppiin!)

  return NFmiFastQueryInfo::Index();
}

// ----------------------------------------------------------------------
/*!
 * \param theLocationIndex Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiSuperSmartInfo::IsLocationMasked(unsigned long theLocationIndex) const
{
  if (fUseAreaMask && itsAreaMask)
  {
    if (fUseHelperBinaryMasks && itsCurrentHelperBinaryMask)
      return itsCurrentHelperBinaryMask->IsMasked(theLocationIndex);
    else
      return itsAreaMask->IsMasked(HPlaceDescriptor().LatLon(theLocationIndex));
  }

  return true;  // jos ei maskia k‰ytˆss‰, on maski aina p‰‰ll‰!!!
}

// ----------------------------------------------------------------------
/*!
 * \return Undocumented
 */
// ----------------------------------------------------------------------

float NFmiSuperSmartInfo::VarianceFloatValue(void) const
{
  if (fUseVarianceCalculator && itsCurrentVarianceCalculator)
  {
    // accessleveli‰ pit‰‰ s‰‰t‰‰ t‰ss‰ v‰liaikaisesti, ett‰
    // calculator p‰‰see k‰siksi 'alemman' tason dataan
    DataAccessLevel oldAccessLevel = itsFloatValueAccessLevel;
    itsFloatValueAccessLevel = kNormalAccess;

    float value = static_cast<float>(itsCurrentVarianceCalculator->FloatValue());

    // palautetaan accesslevel
    itsFloatValueAccessLevel = oldAccessLevel;
    return value;
  }

  return RawFloatValue();
}

// ----------------------------------------------------------------------
/*!
 * \return Undocumented
 */
// ----------------------------------------------------------------------

float NFmiSuperSmartInfo::VariationFloatValue(void) const
{
  if (fUseVariationCalculator && itsVariationCalculator)
  {
    // accessleveli‰ pit‰‰ s‰‰t‰‰ t‰ss‰ v‰liaikaisesti, ett‰
    // calculator p‰‰see k‰siksi 'alemman' tason dataan
    DataAccessLevel oldAccessLevel = itsFloatValueAccessLevel;
    itsFloatValueAccessLevel = kTimeIntegrationAccess;

    float value = static_cast<float>(itsVariationCalculator->FloatValue());

    // palautetaan accesslevel
    itsFloatValueAccessLevel = oldAccessLevel;
    return value;
  }

  // mielest‰ni pit‰‰ kutsua seuraavaa tasoa, jos t‰lt‰ tasolta
  // ei lˆydy calculaattoria
  return TimeIntegrationFloatValue();
}

// ----------------------------------------------------------------------
/*!
 * \return Undocumented
 */
// ----------------------------------------------------------------------

float NFmiSuperSmartInfo::TimeIntegrationFloatValue(void) const
{
  if (fUseTimeIntegrationCalculator && itsTimeIntegrationCalculator)
  {
    // accessleveli‰ pit‰‰ s‰‰t‰‰ t‰ss‰ v‰liaikaisesti, ett‰
    // calculator p‰‰see k‰siksi 'alemman' tason dataan
    DataAccessLevel oldAccessLevel = itsFloatValueAccessLevel;
    itsFloatValueAccessLevel = kVarianceAccess;

    float value = static_cast<float>(itsTimeIntegrationCalculator->FloatValue());

    itsFloatValueAccessLevel = oldAccessLevel;
    return value;
  }

  // mielest‰ni pit‰‰ kutsua seuraavaa tasoa, jos t‰lt‰ tasolta
  // ei lˆydy calculaattoria
  return VarianceFloatValue();
}

// ----------------------------------------------------------------------
/*!
 * \return Undocumented
 */
// ----------------------------------------------------------------------

NFmiCombinedParam* NFmiSuperSmartInfo::VariationCombinedValue(void)
{
  if (fUseVariationCalculator && itsVariationCalculator)
  {
    // accessleveli‰ pit‰‰ s‰‰t‰‰ t‰ss‰ v‰liaikaisesti, ett‰
    // calculator p‰‰see k‰siksi 'alemman' tason dataan
    DataAccessLevel oldAccessLevel = itsFloatValueAccessLevel;
    itsFloatValueAccessLevel = kVarianceAccess;

    NFmiCombinedParam* value = itsVariationCalculator->CombinedValue();

    itsFloatValueAccessLevel = oldAccessLevel;
    return value;
  }

  // mielest‰ni pit‰‰ kutsua seuraavaa tasoa, jos t‰lt‰ tasolta
  // ei lˆydy calculaattoria

  return 0;  // VarianceFloatValue();
}

// ----------------------------------------------------------------------
/*!
 * \return Undocumented
 */
// ----------------------------------------------------------------------

NFmiCombinedParam* NFmiSuperSmartInfo::CalculationCombinedValue(void)
{
  if (fUseCalculator && itsCalculator)
  {
    // accessleveli‰ pit‰‰ s‰‰t‰‰ t‰ss‰ v‰liaikaisesti, ett‰
    // calculator p‰‰see k‰siksi 'alemman' tason dataan
    DataAccessLevel oldAccessLevel = itsFloatValueAccessLevel;
    itsFloatValueAccessLevel = kVariationAccess;

    NFmiCombinedParam* value = itsCalculator->CombinedValue();

    itsFloatValueAccessLevel = oldAccessLevel;
    return value;
  }

  // mielest‰ni pit‰‰ kutsua seuraavaa tasoa, jos t‰lt‰ tasolta
  // ei lˆydy calculaattoria

  return VariationCombinedValue();  // VarianceFloatValue();
}

// ----------------------------------------------------------------------
/*!
 * \return Undocumented
 */
// ----------------------------------------------------------------------

float NFmiSuperSmartInfo::CalculationFloatValue(void) const
{
  if (fUseCalculator && itsCalculator)
  {
    // accessleveli‰ pit‰‰ s‰‰t‰‰ t‰ss‰ v‰liaikaisesti, ett‰
    // calculator p‰‰see k‰siksi 'alemman' tason dataan
    DataAccessLevel oldAccessLevel = itsFloatValueAccessLevel;
    itsFloatValueAccessLevel = kVariationAccess;

    float value = static_cast<float>(itsCalculator->FloatValue());

    itsFloatValueAccessLevel = oldAccessLevel;
    return value;
  }

  // mielest‰ni pit‰‰ kutsua seuraavaa tasoa, jos t‰lt‰ tasolta
  // ei lˆydy calculaattoria

  return VariationFloatValue();
}

// ----------------------------------------------------------------------
/*!
 * \param theCalculator Undocumented
 * \param useCalculator Undocumented
 */
// ----------------------------------------------------------------------

void NFmiSuperSmartInfo::SetCalculator(NFmiCalculator* theCalculator, bool useCalculator)
{
  itsCalculator = theCalculator;
  if (theCalculator)
    fUseCalculator = useCalculator;
  else  // jos 0-pointteri, otetaan pois k‰ytˆst‰!!!
    fUseCalculator = false;
}

// ----------------------------------------------------------------------
/*!
 * \param theDescriptor Undocumented
 */
// ----------------------------------------------------------------------

void NFmiSuperSmartInfo::SetDataModifierDescriptor(NFmiDataModifierDescriptor* theDescriptor)
{
  itsDataModifierDescriptor = theDescriptor;  // HUOM!! Ei omista, pit‰isikˆ???
  UpdateVarianceCalculator();
}

// ----------------------------------------------------------------------
/*!
 * \param value Undocumented
 */
// ----------------------------------------------------------------------

void NFmiSuperSmartInfo::UseVarianceCalculator(bool value)
{
  fUseVarianceCalculator = value;
  UpdateVarianceCalculator();
}

// ----------------------------------------------------------------------
/*!
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiSuperSmartInfo::FirstParam(bool fIgnoreSubParam)
{
  bool status = NFmiFastQueryInfo::FirstParam(fIgnoreSubParam);
  UpdateVarianceCalculator();
  return status;
}

// ----------------------------------------------------------------------
/*!
 * \param fIgnoreSubParam Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiSuperSmartInfo::NextParam(bool fIgnoreSubParam)
{
  bool status = NFmiFastQueryInfo::NextParam(fIgnoreSubParam);
  UpdateVarianceCalculator();
  return status;
}

// ----------------------------------------------------------------------
/*!
 * \param fIgnoreSubParam Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiSuperSmartInfo::PreviousParam(bool fIgnoreSubParam)
{
  bool status = NFmiFastQueryInfo::PreviousParam(fIgnoreSubParam);
  UpdateVarianceCalculator();
  return status;
}

// ----------------------------------------------------------------------
/*!
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiSuperSmartInfo::LastParam(bool fIgnoreSubParam)
{
  bool status = NFmiFastQueryInfo::LastParam(fIgnoreSubParam);
  UpdateVarianceCalculator();
  return status;
}

// ----------------------------------------------------------------------
/*!
 * \param theParam Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiSuperSmartInfo::Param(const NFmiParam& theParam)
{
  bool status = NFmiFastQueryInfo::Param(theParam);
  UpdateVarianceCalculator();
  return status;
}

// ----------------------------------------------------------------------
/*!
 * \param theParam Undocumented
 * \param theSubParam Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiSuperSmartInfo::Param(const NFmiParam& theParam, const NFmiParam& theSubParam)
{
  bool status = NFmiFastQueryInfo::Param(theParam, theSubParam);
  UpdateVarianceCalculator();
  return status;
}

// ----------------------------------------------------------------------
/*!
 * \param theDataIdent Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiSuperSmartInfo::Param(const NFmiDataIdent& theDataIdent)
{
  bool status = NFmiFastQueryInfo::Param(theDataIdent);
  UpdateVarianceCalculator();
  return status;
}

// ----------------------------------------------------------------------
/*!
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiSuperSmartInfo::FirstTime(void)
{
  // Ajansiirto-funktioissa pit‰‰ p‰ivitt‰‰ myˆs maskin aikaa
  unsigned long oldTimeIndex = itsTimeIndex;
  bool status = NFmiFastQueryInfo::FirstTime();
  TimeChanged(oldTimeIndex);
  return status;
}

// ----------------------------------------------------------------------
/*!
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiSuperSmartInfo::NextTime(void)
{
  unsigned long oldTimeIndex = itsTimeIndex;
  bool status = NFmiFastQueryInfo::NextTime();
  TimeChanged(oldTimeIndex);
  return status;
}

// ----------------------------------------------------------------------
/*!
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiSuperSmartInfo::LastTime(void)
{
  unsigned long oldTimeIndex = itsTimeIndex;
  bool status = NFmiFastQueryInfo::LastTime();
  TimeChanged(oldTimeIndex);
  return status;
}

// ----------------------------------------------------------------------
/*!
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiSuperSmartInfo::PreviousTime(void)
{
  unsigned long oldTimeIndex = itsTimeIndex;
  bool status = NFmiFastQueryInfo::PreviousTime();
  TimeChanged(oldTimeIndex);
  return status;
}

// ----------------------------------------------------------------------
/*!
 * \param theTime Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiSuperSmartInfo::Time(const NFmiMetTime& theTime)
{
  unsigned long oldTimeIndex = itsTimeIndex;
  // TimeChanged metodia ei saa kutsua, jos asetus feilaa
  bool status = NFmiFastQueryInfo::Time(theTime);
  if (status) TimeChanged(oldTimeIndex);
  return status;
}

// ----------------------------------------------------------------------
/*!
 * \param theIndex Undocumented
 * \result Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiSuperSmartInfo::TimeIndex(unsigned long theIndex)
{
  unsigned long oldTimeIndex = itsTimeIndex;
  bool status = NFmiFastQueryInfo::TimeIndex(theIndex);
  TimeChanged(oldTimeIndex);
  return status;
}

// ----------------------------------------------------------------------
/*!
 *
 */
// ----------------------------------------------------------------------
void NFmiSuperSmartInfo::UpdateVarianceCalculator(void)
{
  if (itsDataModifierDescriptor)
  {
    const NFmiDataIdent& param = NFmiFastQueryInfo::Param();
    const NFmiLevel* level = Level();
    itsCurrentVarianceCalculator->SetData(this);
    NFmiParamDataModifier* modifier = itsDataModifierDescriptor->VarianceModifier(param, level);
    itsCurrentVarianceCalculator->SetDataModifier(modifier);
  }
}

// ----------------------------------------------------------------------
/*!
 *
 */
// ----------------------------------------------------------------------

void NFmiSuperSmartInfo::UpdateAreaMaskTime(void)
{
  if (fUseAreaMask && itsAreaMask)
  {
    SetCurrentHelperBinaryMask();
  }
}

// ----------------------------------------------------------------------
/*!
 *
 */
// ----------------------------------------------------------------------

void NFmiSuperSmartInfo::SetCurrentHelperBinaryMask(void)
{
  itsCurrentHelperBinaryMask = HelperBinaryMask(CalcAreaUnCertainty());
}

// ----------------------------------------------------------------------
/*!
 * \param theUsedVariationFactor
 * \result Undocumented
 */
// ----------------------------------------------------------------------

NFmiBitmapAreaMask* NFmiSuperSmartInfo::HelperBinaryMask(int theUsedVariationFactor)
{
  size_t size = itsHelperBinaryMaskList.size();
  if (theUsedVariationFactor < 0 || static_cast<std::size_t>(theUsedVariationFactor) >= size)
    return 0;
  else
    return itsHelperBinaryMaskList[theUsedVariationFactor];
}

// ----------------------------------------------------------------------
/*!
 * \param newStatus Undocumented
 */
// ----------------------------------------------------------------------

void NFmiSuperSmartInfo::UseHelperBinaryMasks(bool newStatus)
{
  fUseHelperBinaryMasks = newStatus;
  InitHelperBinaryMasks();  // pit‰‰ varmuuden vuoksi aina alustaa (voisi optimoida)
}

// ----------------------------------------------------------------------
/*!
 * Alustaa kaikki tarvittavat harvennusapu maskit
 * (eli kaikille ep‰varmuuksille 0-n).
 *
 * \return  Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiSuperSmartInfo::InitHelperBinaryMasks(void)
{
  if (fUseHelperBinaryMasks)
  {
    ClearHelperBinaryMasks();
    CalcXYMaskBoundingBox();
    // int start = AreaUnCertaintyStart();
    int end = AreaUnCertaintyEnd();
    itsHelperBinaryMaskList.resize(end + 1);
    for (int i = 0; i <= end; i++)
      itsHelperBinaryMaskList[i] = CreateHelperBinaryMask(i);
    return true;
  }
  return false;
}

// ----------------------------------------------------------------------
/*!
 * \param theUsedVariationFactor Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

NFmiBitmapAreaMask* NFmiSuperSmartInfo::CreateHelperBinaryMask(int theUsedVariationFactor)
{
  if (theUsedVariationFactor == 0) return CreateZeroVariationHelperBinaryMask();

  NFmiBitmapAreaMask* helperMask =
      new NFmiBitmapAreaMask(itsGridXNumber, itsGridYNumber, Area(), &Param(), Level());
  if (!helperMask) return 0;

  int moveByX = theUsedVariationFactor * 2 + 1;
  int moveByY = theUsedVariationFactor * 2 + 1;
  int left = static_cast<int>(itsXYMaskBoundingBox.Left());
  int right = static_cast<int>(itsXYMaskBoundingBox.Right());

  int moveXFromStart = 0;
  int moveXFactor = (right - left) % (moveByX);
  bool specialXCase = ((moveByX > 3) && ((moveByX - moveXFactor) ==
                                         1));  // jos totta, tihennet‰‰n harvennushilaa yhdell‰
  if (specialXCase)
    moveByX--;
  else if (moveXFactor > 1)
    moveXFromStart = moveXFactor / 2;
  int top = static_cast<int>(itsXYMaskBoundingBox.Top());
  int bottom = static_cast<int>(itsXYMaskBoundingBox.Bottom());
  int moveYFromStart = 0;
  int moveYFactor = (bottom - top) % (moveByY);
  bool specialYCase = ((moveByY > 3) && ((moveByY - moveYFactor) == 1));
  if (specialYCase)
    moveByY--;
  else if (moveYFactor > 1)
    moveYFromStart = moveYFactor / 2;
  helperMask->Mask((bottom + top) / 2 * itsGridXNumber + (right + left) / 2, true);
  for (int j = top + moveYFromStart; j <= bottom; j += moveByY)
  {
    for (int i = left + moveXFromStart; i <= right; i += moveByX)
    {
      int idx = j * itsGridXNumber + i;
      helperMask->Mask(idx, true);
    }
  }
  return helperMask;
}

// ----------------------------------------------------------------------
/*!
 * T‰m‰ luo itseasiassa maskin muotoisen bin‰‰rimaskin, kun ei ole
 * ep‰varmuutta mukana
 */
// ----------------------------------------------------------------------

NFmiBitmapAreaMask* NFmiSuperSmartInfo::CreateZeroVariationHelperBinaryMask(void)
{
  NFmiAreaMask* mask = AreaMask();
  NFmiBitmapAreaMask* helperMask = new NFmiBitmapAreaMask;
  if (mask && mask->Info() && helperMask)
  {
    unsigned int oldLocationIndex = LocationIndex();
    bool oldUseAreaMaskStatus = UseAreaMask();
    // laitetaan maski pois p‰‰lt‰, niin bitmapmaski voi tehd‰ hommansa
    UseAreaMask(false);

    helperMask->Init(mask->Info(), AreaMask()->Condition());

    LocationIndex(oldLocationIndex);
    UseAreaMask(oldUseAreaMaskStatus);

    return helperMask;
  }
  return 0;
}

// ----------------------------------------------------------------------
/*!
 * Tuhoaa maskit
 */
// ----------------------------------------------------------------------

void NFmiSuperSmartInfo::ClearHelperBinaryMasks(void)
{
  size_t size = itsHelperBinaryMaskList.size();
  for (size_t i = 0; i < size; i++)
    delete itsHelperBinaryMaskList[i];
  itsHelperBinaryMaskList.clear();
}

// ----------------------------------------------------------------------
/*!
 * Laskee k‰ytetyn maskin xy-indeksi-boundingboxin
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

void NFmiSuperSmartInfo::CalcXYMaskBoundingBox(void)
{
  unsigned int oldLocationIndex = LocationIndex();
  bool oldUseAreaMaskStatus = UseAreaMask();
  // laitetaan maski p‰‰lle, niin liikutaan vain maskatuissa paikoissa
  UseAreaMask(true);
  bool oldUseHelperBinaryMasksState = fUseHelperBinaryMasks;
  fUseHelperBinaryMasks = false;
  NFmiDataModifierMinMax xMinMax;
  NFmiDataModifierMinMax yMinMax;
  int xOffset = -1, yOffset = -1;
  for (ResetLocation(); NextLocation();)
  {
    // t‰m‰n pit‰isi olla turha testi
    if (LocationIndex2XYPosition(LocationIndex(), &xOffset, &yOffset))
    {
      xMinMax.Calculate(static_cast<float>(xOffset));
      yMinMax.Calculate(static_cast<float>(yOffset));
    }
  }
  itsXYMaskBoundingBox =
      NFmiRect(xMinMax.MinValue(), yMinMax.MaxValue(), xMinMax.MaxValue(), yMinMax.MinValue());

  LocationIndex(oldLocationIndex);
  UseAreaMask(oldUseAreaMaskStatus);
  fUseHelperBinaryMasks = oldUseHelperBinaryMasksState;
}

// ----------------------------------------------------------------------
/*!
 * Testi funktio. Tekee stringin tietylle ep‰varmuudelle tehdyst‰
 * harvennusmaskista
 *
 * Sis‰lt‰‰ seuraavaa:
 *
 *  - n x m merkki‰ (eli kuvaa hilaa)
 *  - '-' = ei maskia, ei harvennuspistett‰
 *  - 'a' = maski, ei harvennuspistett‰
 *  - '@' = maski ja harvennuspiste
 *  - 'C' = ei maskia, harvennuspiste
 *
 * \param theUsedVariationFactor
 * \return Undocumented
 */
// ----------------------------------------------------------------------

NFmiString NFmiSuperSmartInfo::HelperBinaryMaskTestString(int theUsedVariationFactor)
{
  NFmiString returnStr;
  NFmiAreaMask* mask = AreaMask();
  NFmiBitmapAreaMask* helperMask = HelperBinaryMask(theUsedVariationFactor);
  if (mask && helperMask)
  {
    unsigned int oldLocationIndex = LocationIndex();
    bool oldUseAreaMaskStatus = UseAreaMask();
    UseAreaMask(false);  // laitetaan maski pois p‰‰lt‰, niin liikutaan kaikissa paikoissa

    int locIndex = 0;
    int i = 0;
    string rowStr;
    checkedVector<string> lines;
    int x = 0, y = 0;
    for (ResetLocation(); NextLocation();)
    {
      locIndex = LocationIndex();
      bool status1 = mask->IsMasked(locIndex);
      bool status2 = helperMask->IsMasked(locIndex);
      if (status1 && status2)
        rowStr += "@";
      else if (status1)
        rowStr += "a";
      else if (status2)
        rowStr += "C";
      else
      {
        if (LocationIndex2XYPosition(locIndex, &x, &y))
        {
          if ((x == itsXYMaskBoundingBox.Left() || x == itsXYMaskBoundingBox.Right()) &&
              (y <= itsXYMaskBoundingBox.Bottom() && y >= itsXYMaskBoundingBox.Top()))
            rowStr += "|";
          else if ((x >= itsXYMaskBoundingBox.Left() && x <= itsXYMaskBoundingBox.Right()) &&
                   (y == itsXYMaskBoundingBox.Bottom() || y == itsXYMaskBoundingBox.Top()))
            rowStr += "-";
          else
            rowStr += ",";
        }
        else
          rowStr += ",";
      }
      i++;
      if (i % itsGridXNumber == 0)
      {
        rowStr += "\n";
        lines.push_back(rowStr);
        rowStr = "";
      }
    }
    rowStr += "\n";  // lis‰t‰‰n viel‰ viimeinen rivi vectoriin
    lines.push_back(rowStr);

    LocationIndex(oldLocationIndex);
    UseAreaMask(oldUseAreaMaskStatus);
    for (i = lines.size() - 1; i >= 0;
         i--)  // laitetaan rivit k‰‰nteisess‰ j‰rjestyksess‰, ett‰ 'kuva' tulee oikein p‰in
      returnStr += lines[i];
  }
  else
    returnStr += "Ei ollut maskia tai harvennusmaskia k‰ytett‰viss‰";

  return returnStr;
}

// ----------------------------------------------------------------------
/*!
 *
 */
// ----------------------------------------------------------------------

void NFmiSuperSmartInfo::UpdateHelperBinaryMasks(void)
{
  InitHelperBinaryMasks();
  SetCurrentHelperBinaryMask();
}

// ----------------------------------------------------------------------
/*!
 * Kutsu t‰t‰ aina kun aika mahdollisesti muuttuu SSInfossa!!!!
 * Pit‰‰ ottaa vanha timeindex talteen ennen muutosta ja antaa parametrina.
 *
 * \param theOldTimeIndex Undocumented
 */
// ----------------------------------------------------------------------

void NFmiSuperSmartInfo::TimeChanged(unsigned long theOldTimeIndex)
{
  if (theOldTimeIndex != itsTimeIndex)
    UpdateAreaMaskTime();  // hoitaa myˆs SetCurrentHelperBinaryMask-kutsun
  if (itsVariationCalculator)
  {
    int uncertainty = CalcAreaUnCertainty();
    itsVariationCalculator->DataIterator()->SetDimensions(
        uncertainty, uncertainty, 0);  // dt toistaiseksi 0
  }
}

// ----------------------------------------------------------------------
/*!
 *
 */
// ----------------------------------------------------------------------

void NFmiSuperSmartInfo::InitEmptyAreaMask(void)
{
  NFmiDataIdent dummyParam;
  NFmiLevel dummyLevel;
  delete itsAreaMask;
  itsAreaMask = new NFmiBitmapAreaMask(*Grid(), &dummyParam, &dummyLevel);
}

// ----------------------------------------------------------------------
/*!
 * \param theLatLonPoint Undocumented
 * \param newValue Undocumented
 * \param fClearFirst Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiSuperSmartInfo::SetNearestPointMask(const NFmiPoint& theLatLonPoint,
                                             bool newValue,
                                             bool fClearFirst)
{
  if (itsAreaMask)
  {
    unsigned long oldIndex = LocationIndex();
    bool status = Location(theLatLonPoint);
    if (status)
    {
      if (fClearFirst) itsAreaMask->SetAll(false);
      itsAreaMask->Mask(LocationIndex(), newValue);
    }
    LocationIndex(oldIndex);
    return status;
  }
  return false;
}

// ======================================================================
