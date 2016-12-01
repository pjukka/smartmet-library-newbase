// ======================================================================
/*!
 * \file NFmiMultiPolygonAreaMask.cpp
 * \brief Implementation of class NFmiMultiPolygonAreaMask
 */
// ======================================================================
/*!
 * \class NFmiMultiPolygonAreaMask
 *
 *  Luokka hoitaa maskauksen, jossa on annettu useita polygoneja/polkuja
 *  maskialueiksi. Jolloin jokaisella erillisell‰ alueella on oma maski arvonsa.
 *
 *  Apuna k‰ytet‰‰n listaa NFmiSvgPath-otuksia, jotka kukin hoitaa yhden maskialueen.
 *
 *  Osaa lukea ja kirjoittaa itsens‰ (poiketen muista areamaski-luokista), koska
 *  data on hankalassa muodossa ja halutaan tallettaa ett‰ k‰yttˆ olisi
 *  mahdollisimman yksinkertaista.
 *
 *  HUOM! Ei ole optimoitu k‰ytt‰m‰‰n valmiiksi laskettuja bin‰‰ri kentti‰ (esim.
 *  NFmiBitmask-luokkaa k‰ytt‰en). Polygoni lasketa voi olla raskasta ja optimointia
 *  pit‰‰ varmaan joskus tehd‰.
 *
 *  HUOM! Write:a ei ole toteutettu, koska sit‰ ei ole toteutettu
 *  NFmiSvgPath luokassakaan.
 */
// ======================================================================

#include "NFmiMultiPolygonAreaMask.h"
#include "NFmiBitmapAreaMask.h"
#include "NFmiSvgPath.h"
#include "NFmiSvgTools.h"
#include "NFmiPoint.h"
#include "NFmiGrid.h"

// ----------------------------------------------------------------------
/*!
 * Destructor
 */
// ----------------------------------------------------------------------

NFmiMultiPolygonAreaMask::~NFmiMultiPolygonAreaMask(void) { Clear(); }
// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

NFmiMultiPolygonAreaMask::NFmiMultiPolygonAreaMask(void)
    : NFmiAreaMaskImpl(), itsAreaPolygons(), itsAreaPolygonValues(), fInitialized(false)
{
}

// ----------------------------------------------------------------------
/*!
 * Constructor
 *
 * \param theOperation Undocumented
 * \param theMaskType Undocumented
 * \param theDataType Undocumented
 * \param thePostBinaryOperator Undocumented
 */
// ----------------------------------------------------------------------

NFmiMultiPolygonAreaMask::NFmiMultiPolygonAreaMask(const NFmiCalculationCondition& theOperation,
                                                   Type theMaskType,
                                                   NFmiInfoData::Type theDataType,
                                                   BinaryOperator thePostBinaryOperator)
    : NFmiAreaMaskImpl(theOperation, theMaskType, theDataType, thePostBinaryOperator),
      itsAreaPolygons(),
      itsAreaPolygonValues(),
      fInitialized(false)
{
}

// ----------------------------------------------------------------------
/*!
 *  Katsotaan ensin mink‰ SVGPath-olion sis‰ll‰ annettu piste mahdollisesti
 *  sijaitsee ja palautetaan kyseisen polun (polygonin) arvo.
 *
 * \param theLatLon Undocumented
 */
// ----------------------------------------------------------------------

double NFmiMultiPolygonAreaMask::CalcValueFromLocation(const NFmiPoint& theLatLon) const
{
  int size = itsAreaPolygons.size();
  for (int i = 0; i < size; i++)
    if (NFmiSvgTools::IsInside(*itsAreaPolygons[i], theLatLon)) return itsAreaPolygonValues[i];
  return kFloatMissing;
}

// ----------------------------------------------------------------------
/*!
 *  Ei ole viel‰ toteutettu, palauttaa tyhj‰‰!
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

const NFmiString NFmiMultiPolygonAreaMask::MakeSubMaskString(void) const
{
  NFmiString str;
  return str;
}

// ----------------------------------------------------------------------
/*!
 *  Siivoaa tietorakenteet tyhjiksi. Deletoi polku-olioiden pointterit!
 */
// ----------------------------------------------------------------------

void NFmiMultiPolygonAreaMask::Clear(void)
{
  int size = itsAreaPolygons.size();
  for (int i = 0; i < size; i++)
    delete itsAreaPolygons[i];

  std::vector<NFmiSvgPath*>().swap(
      itsAreaPolygons);  // tyhjennet‰‰n vectorit empty-object-swap -tempulla.
  std::vector<double>().swap(itsAreaPolygonValues);
}

// ----------------------------------------------------------------------
/*!
 * Write the object to the given output stream.
 *
 * \param file The output stream to write to
 * \return The output stream written to
 */
// ----------------------------------------------------------------------

std::ostream& NFmiMultiPolygonAreaMask::Write(std::ostream& file) const
{
  int size = itsAreaPolygons.size();
  for (int i = 0; i < size; i++)
  {
    file << itsAreaPolygonValues[i] << std::endl;
    file << *itsAreaPolygons[i] << std::endl;
  }
  return file;
}

// ----------------------------------------------------------------------
/*!
 *  Luetaan polygonit tiedostosta (NFmiSvgPath-oliot ja niiden alue arvot.)
 *  Formaatti:				/par
 * \code
 *  area1arvo:
 *  area1Polku
 *  area2arvo:
 *  area2Polku
 *  jne...
 * \endcode
 *
 *  area1 esim. "M10 10 L 10 20 20 20 20 10 Z" (simppeli laatikko, pit‰‰ olla heittomerkkeineen!)
 *  HUOM! Ei talleteta lukum‰‰r‰‰ alkuun, vaan jatketaan kunnes eof-tulee.
 */
// ----------------------------------------------------------------------

std::istream& NFmiMultiPolygonAreaMask::Read(std::istream& file)
{
  NFmiSvgPath* tmp = 0;
  double value;
  Clear();
  do
  {
    file >> value;
    if (file.fail()) break;
    tmp = new NFmiSvgPath;
    file >> *tmp;
    if (file.fail())
    {
      delete tmp;
      break;
    }
    itsAreaPolygons.push_back(tmp);
    itsAreaPolygonValues.push_back(value);
  } while (!file.fail());
  return file;
}

// ----------------------------------------------------------------------
/*!
 * \brief
 * T‰ll‰ funktiolla luodaan valmis 'cache'-maski haluttuun projektioon
 * ja hilaan.
 *
 *  Luodaan t‰st‰ multipolygon maskista bitmapMaski. T‰m‰n k‰yttˆ on sitten
 *  nopeampaa kuin se, ett‰ aina kysyt‰‰n, onko joku piste t‰m‰n polygoni-
 *  satsin sis‰ll‰ tietyll‰ ehdolla. Eli t‰ll‰ voidaan luoda cache-maski
 *  tietynlaiselle gridille tietyill‰ ehdoilla.
 *
 * \param theGrid Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

NFmiBitmapAreaMask* NFmiMultiPolygonAreaMask::CreateBitmapAreaMask(const NFmiGrid& theGrid)
{
  NFmiBitmapAreaMask* mask = new NFmiBitmapAreaMask(theGrid, 0, 0);
  if (mask)
  {
    NFmiGrid grid(theGrid);  // tehd‰‰n hilasta kopio, ett‰ voidaan k‰yd‰ hila l‰pi
    for (grid.Reset(); grid.Next();)
    {
      mask->Mask(grid.Index(), this->IsMasked(grid.LatLon()));
    }
  }
  return mask;
}

// ======================================================================
