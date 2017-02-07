// ======================================================================
/*!
 * \file NFmiProducerIdLister.cpp
 * \brief Implementation of class NFmiProducerIdLister
 */
// ======================================================================
/*!
 * \class NFmiProducerIdLister
 *
 *  Author         : pietarin
 *  Creation Date  : Wed - Nov 27, 2002
 *  Editoituun QueryDataan on tarkoitus laitta tieto, mit‰ alkuper‰isi‰ tuottajia
 *  on k‰ytetty kunkin ajanhetken pohjana. Tieto talletetaan QDatan QInfon
 *  stringlist:aan itsHeaderText jonkin 'avaimen' alle.
 *  T‰m‰ luokka joka osaa tehd‰ tuottaja ID:eill‰ t‰ytetyn stringin, jossa
 *  on myˆs tiedot alkuajasta ja aikaaskeleesta ja aika-askelten lukum‰‰r‰st‰
 *  ja lis‰ksi viel‰ list‰ tuottaja ID:t‰.
 */
// ======================================================================

#include "NFmiProducerIdLister.h"
#include "NFmiQueryDataUtil.h"
#include "NFmiQueryInfo.h"
#include "NFmiTimeList.h"
#include <cmath>
#include <sstream>

using namespace std;

//--------------------------------------------------------
// Constructor/Destructor
//--------------------------------------------------------
NFmiProducerIdLister::NFmiProducerIdLister(void)
    : itsTimes(),
      itsProducerString(),
      itsProducerIds(itsTimes.Size(), -1),
      itsModelOriginTimes(itsTimes.Size(), NFmiMetTime(1900, 1, 1, 0, 0)),
      itsDefaultProducerId(-1)
{
}

NFmiProducerIdLister::NFmiProducerIdLister(const NFmiTimeDescriptor &theTimes,
                                           int theDefaultProducerId)
    : itsTimes(theTimes),
      itsProducerString(),
      itsProducerIds(theTimes.Size(), theDefaultProducerId),
      itsModelOriginTimes(itsTimes.Size(), NFmiMetTime(1900, 1, 1, 0, 0)),
      itsDefaultProducerId(theDefaultProducerId)
{
}

NFmiProducerIdLister::NFmiProducerIdLister(const std::string &theProducersString)
    : itsTimes(),
      itsProducerString(),
      itsProducerIds(),
      itsModelOriginTimes(),
      itsDefaultProducerId(-1)
{
  IntepretProducerIdString(theProducersString);
  // pit‰isikˆ paluuarvo tarkistaa ja jos ep‰onnistui, heitt‰‰ poikkeus?
}

NFmiProducerIdLister::NFmiProducerIdLister(const NFmiProducerIdLister &theOther)
    : itsTimes(theOther.itsTimes),
      itsProducerString(theOther.itsProducerString),
      itsProducerIds(theOther.itsProducerIds),
      itsModelOriginTimes(theOther.itsModelOriginTimes),
      itsDefaultProducerId(theOther.itsDefaultProducerId)
{
}

// Huom! pit‰isi olla const info, mutta pari metodia pit‰‰ muuttaa ensin
NFmiProducerIdLister::NFmiProducerIdLister(NFmiQueryInfo &theInfo)
    : itsTimes(),
      itsProducerString(),
      itsProducerIds(),
      itsModelOriginTimes(),
      itsDefaultProducerId(-1)
{
  if (theInfo.FindFirstKey(NFmiQueryDataUtil::GetOfficialQueryDataProdIdsKey()))
    IntepretProducerIdString(string(theInfo.GetCurrentKeyValue()));
}
//--------------------------------------------------------
// GetProducerIdString
//--------------------------------------------------------
/*!
 * Rakentaa timebagin ja producerId vectorin mukaisen stringin,
 * tallettaa sen dataosaksi ja palauttaa kopion siihen. Jos ei voinut
 * rakentaa stringi‰ jostain syyst‰, palauttaa tyhj‰n stringin.
 */
const std::string NFmiProducerIdLister::MakeProducerIdString(void)
{
  if (itsProducerIds.size() != itsTimes.Size()) return "";
  ostringstream out;
  out << itsTimes.FirstTime() << " ";
  int vsize = itsTimes.Size();  // t‰m‰ on myˆs id ja origintime vektoreiden koko
  out << vsize << " ";
  if (itsTimes.UseTimeList())
  {
    out << "1 ";  // timelist-merkki
    for (itsTimes.Reset(); itsTimes.Next();)
      out << static_cast<long>(itsTimes.Resolution()) << " ";
  }
  else
  {
    out << "0 ";  // timebag-merkki
    out << static_cast<long>(itsTimes.Resolution()) << " ";
  }
  int i = 0;
  for (i = 0; i < vsize; i++)
  {
    out << itsProducerIds[i];
    out << " ";  // t‰ss‰ laitetaan myˆs viimeisen per‰‰n space, koska jatketaan t‰m‰n per‰‰n
                 // origintimeilla
  }
  for (i = 0; i < vsize; i++)
  {
    out << itsModelOriginTimes[i];
    if (i < vsize - 1)  // viimeiseen ei en‰‰ laiteta spacea v‰liin
      out << " ";
  }
  itsProducerString = out.str();
  return itsProducerString;
}
//--------------------------------------------------------
// IntepretProducerIdString
//--------------------------------------------------------
/*!
 * Purkaa annetun stringin jos mahdollista ja muodostaa siit‰ timebagi, ja producer id vectorin.
 * Palauttaa false, jos ei voinut purkaa/tulkata
 * annettua stringi‰.
 */
bool NFmiProducerIdLister::IntepretProducerIdString(const std::string &theString)
{
  istringstream in(theString);
  NFmiMetTime firstTime;
  in >> firstTime;
  int vSize = 0;
  in >> vSize;
  int timeDescType = 0;  // 0=timebag ja 1=timelist
  in >> timeDescType;
  if (in.fail()) return false;
  if (timeDescType == 0)
  {
    int timeResolutionInMinutes = 0;
    in >> timeResolutionInMinutes;
    if (in.fail()) return false;
    NFmiMetTime lastTime(firstTime);
    lastTime.ChangeByMinutes(timeResolutionInMinutes * (vSize - 1));
    NFmiTimeBag times(firstTime, lastTime, timeResolutionInMinutes);
    NFmiTimeDescriptor timesDesc(firstTime, times);
    itsTimes = timesDesc;
  }
  else
  {
    NFmiTimeList timeList;
    int timeResolutionInMinutes = 0;
    NFmiMetTime currentTime(firstTime);
    timeList.Add(new NFmiMetTime(currentTime));
    for (int i = 0; i < vSize - 1;
         i++)  // tama menee yhta vajaa loppuun asti, koska viimeinen aika ero ei merkitse mitaan
    {
      if (i == 0)
        in >>
            timeResolutionInMinutes;  // 1. aikaresoluutio on turha ja se luetaan vain alta pois!!!
      in >> timeResolutionInMinutes;
      if (in.fail()) return false;
      currentTime.ChangeByMinutes(timeResolutionInMinutes);
      timeList.Add(new NFmiMetTime(currentTime));
    }
    NFmiTimeDescriptor timesDesc(firstTime, timeList);
    itsTimes = timesDesc;
  }

  checkedVector<int> prodIds(vSize, -1);
  int prodId = -1;
  int i = 0;
  for (i = 0; i < vSize; i++)
  {
    in >> prodId;
    prodIds[i] = prodId;
  }
  NFmiMetTime orgTime(1900, 1, 1, 0, 0);
  checkedVector<NFmiMetTime> originTimes(vSize, orgTime);
  for (i = 0; i < vSize; i++)
  {
    in >> orgTime;
    if (in.fail()) break;  // pieleen meni ei jatketa
    originTimes[i] = orgTime;
  }

  if (!in.fail())
  {
    itsProducerString = theString;
    itsProducerIds.swap(prodIds);
    itsModelOriginTimes.swap(originTimes);
    return true;
  }
  return false;
}

//--------------------------------------------------------
// ChangeTimeResolution
//--------------------------------------------------------
/*!
 * Muuttaa esim. 3h resoluutioisen olion tunti resoluutioiseksi.
 * Tekee uuden timebagin ja tuottaja id vektorin ja t‰ytt‰‰ sen niin,
 * ett‰ ID:t levi‰v‰t taaksep‰in, eli eiv‰t interpoloidu v‰lille.
 * Uuden ja vanhan resoluution pit‰‰ olla toistens‰ suhteen tasa
 * jaollisia. Tarkoittaen sit‰, ett‰ jos vanha oli 60 ja uusi on 180,
 * t‰m‰ sallittua, koska 180 on jaollinen 60:ll‰. Mutta jos vanha on
 * 60 ja uusi on 150, se ei k‰y, koska 150/60=2.5. Isomman luvun pit‰‰
 * olla jaollinen tasan pienemm‰ll‰.
 */
bool NFmiProducerIdLister::ChangeTimeResolution(int theNewResolutionInMinutes)
{
  if (theNewResolutionInMinutes <= 0) return false;
  int currentRes = itsTimes.Resolution();
  double ratio = static_cast<double>(currentRes) / theNewResolutionInMinutes;
  NFmiTimeBag newTimeBag(itsTimes.FirstTime(), itsTimes.LastTime(), theNewResolutionInMinutes);
  if (ratio == 0)
    return false;
  else if (ratio == 1)
    return true;  // ei tarvitse tehd‰ mit‰‰n!
  else if (ratio > 1 && currentRes % theNewResolutionInMinutes != 0)
    return false;
  else if (ratio < 1 && theNewResolutionInMinutes % currentRes != 0)
    return false;

  int vSize = newTimeBag.GetSize();
  checkedVector<int> newProdIds(vSize, -1);
  int i = 0;
  checkedVector<NFmiMetTime> newOriginTimes(vSize, NFmiMetTime(1900, 1, 1, 0, 0));
  newTimeBag.Reset();
  for (i = 0; i < vSize; i++)
  {
    newTimeBag.Next();
    newProdIds[i] = ProducerId(newTimeBag.CurrentTime(), true);
    newOriginTimes[i] = ModelOriginTime(newTimeBag.CurrentTime(), true);
  }

  itsTimes = NFmiTimeDescriptor(newTimeBag.FirstTime(), newTimeBag);
  itsProducerIds.swap(newProdIds);
  itsModelOriginTimes.swap(newOriginTimes);

  return true;
}

//--------------------------------------------------------
// ProducerId
//--------------------------------------------------------
int NFmiProducerIdLister::ProducerId(const NFmiMetTime &theTime, bool fAllowInterpolation)
{
  if (itsTimes.Time(theTime)) return itsProducerIds[itsTimes.Index()];
  if (fAllowInterpolation)
    if (itsTimes.TimeToNearestStep(theTime)) return itsProducerIds[itsTimes.Index()];
  return -1;  // -1 on virhekoodi/puuttuva koodi
}

//--------------------------------------------------------
// ProducerId
//--------------------------------------------------------
int NFmiProducerIdLister::ProducerId(int theIndex) const
{
  if (theIndex >= 0 && static_cast<unsigned int>(theIndex) < itsProducerIds.size())
    return itsProducerIds[theIndex];
  return -1;  // -1 on virhekoodi
}
//--------------------------------------------------------
// ProducerId
//--------------------------------------------------------
void NFmiProducerIdLister::ProducerId(int theIndex, int theProducerId)
{
  if (theIndex >= 0 && static_cast<unsigned int>(theIndex) < itsProducerIds.size())
    itsProducerIds[theIndex] = theProducerId;
  //	else
  // pit‰isikˆ heitt‰‰ poikkeus?
}

//--------------------------------------------------------
// ProducerId
//--------------------------------------------------------
void NFmiProducerIdLister::ProducerId(const NFmiMetTime &theTime, int theProducerId)
{
  if (itsTimes.Time(theTime)) itsProducerIds[itsTimes.Index()] = theProducerId;
  //	else
  // pit‰isikˆ heitt‰‰ poikkeus?
}

const NFmiMetTime &NFmiProducerIdLister::ModelOriginTime(const NFmiMetTime &theTime,
                                                         bool fAllowInterpolation)
{
  static NFmiMetTime dummy(1900, 1, 1, 0, 0);
  if (itsTimes.Time(theTime)) return itsModelOriginTimes[itsTimes.Index()];
  if (fAllowInterpolation)
    if (itsTimes.TimeToNearestStep(theTime)) return itsModelOriginTimes[itsTimes.Index()];
  return dummy;
}

const NFmiMetTime &NFmiProducerIdLister::ModelOriginTime(int theIndex) const
{
  static NFmiMetTime dummy(1900, 1, 1, 0, 0);
  if (theIndex >= 0 && static_cast<unsigned int>(theIndex) < itsModelOriginTimes.size())
    return itsModelOriginTimes[theIndex];
  return dummy;
}

void NFmiProducerIdLister::ModelOriginTime(const NFmiMetTime &theTime,
                                           const NFmiMetTime &theOriginTime)
{
  if (itsTimes.Time(theTime)) itsModelOriginTimes[itsTimes.Index()] = theOriginTime;
}

void NFmiProducerIdLister::ModelOriginTime(int theIndex, const NFmiMetTime &theOriginTime)
{
  if (theIndex >= 0 && static_cast<unsigned int>(theIndex) < itsModelOriginTimes.size())
    itsModelOriginTimes[theIndex] = theOriginTime;
}

bool NFmiProducerIdLister::IsEmpty(void) const { return itsProducerIds.empty(); }
