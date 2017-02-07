// ======================================================================
/*!
 * \file NFmiAngle.cpp
 * \brief M‰‰rittelyt luokkien NFmiAngle, NFmiWindDirection, NFmiLatitude
 *        ja NFmiLongitude metodeille.
 */
// ======================================================================
/*!
 * \class NFmiAngle
 *
 * M‰‰rittely yleiselle kulmasuureelle. Luokan keskeinen ominaisuus on, ett‰
 * luokan metodit huolehtivat automaattisesti, ett‰ kulma pysyy sallituissa
 * rajoissa jokaisen operaation j‰lkeen.
 */
// ======================================================================

#include "NFmiAngle.h"
#include <iostream>

using namespace std;

// ======================================================================
/*!
 * Konstruktori luokalle NFmiAngle asettaa sis‰iset arvot k‰ytt‰en SetValue
 * metodia, jotta mahdollinen kulmamuunnos tulee suoritetuksi, ja kulman
 * rajat tarkistetuiksi.
 *
 * \param theValue Undocumented
 * \param theUnit Undocumented
 */
// ======================================================================

NFmiAngle::NFmiAngle(double theValue, FmiAngleUnit theUnit)
    : itsValue(0)  // dummy to prevent advanced warnings
{
  SetValue(theValue, theUnit);
}

// ======================================================================
/*!
 * Muutosmetodi yleiselle kulmalle. Asetettu arvo muutetaan asteiksi,
 * jos annettu yksikkˆ on radiaani. Asteille suoritetaan ainoastaan
 * negatiivisuus tarkistus, mink‰ voi olettaa bugiksi. Jostain syyst‰
 * korrekti koodi on vaan kommentoitu pois. Ei pit‰isi luottaa
 * k‰ytt‰j‰‰n n‰in paljon.
 *
 * \bug Ei varmista kunnolla, ett‰ kulma on sallituissa rajoissa.
 *
 * \param theValue Undocumented
 * \param theUnit Undocumented
 */
// ======================================================================

void NFmiAngle::SetValue(double theValue, FmiAngleUnit theUnit)
{
  if (theUnit == kRad)
  {
    itsValue = 180 / kPii * theValue;
    if (itsValue != 0 && fmod(itsValue, 360) == 0)
      itsValue = 360;
    else
      itsValue = fmod(itsValue, 360);
  }
  else
    itsValue = theValue;  // fmod(theValue, 360);

  if (itsValue < 0) itsValue = itsValue + 360;
}

// ======================================================================
/*!
 * Suurempi-kuin vertailuoperaattori kulmille. T‰m‰ on periaatteessa
 * v‰‰rin, standardi komposiitit vaativat nimenomaan < ja == operaattorien
 * m‰‰rittelyn. Eli, nyt kulmia ei saa esim. laitettua listaan.
 *
 * \param theAngle Undocumented
 * \return Undocumented
 */
// ======================================================================

bool NFmiAngle::operator>(const NFmiAngle& theAngle) const
{
  return (itsValue > theAngle.itsValue);
}

// ======================================================================
/*!
 * Lis‰ysoperaattori.
 *
 * \bug Toteutus vaikuttaa tyhm‰lt‰. Miksei k‰ytet‰ SetValue metodia,
 * vaan luodaan turha temppi?
 *
 * \param theAngle Undocumented
 * \return Undocumented
 */
// ======================================================================

NFmiAngle& NFmiAngle::operator+=(const NFmiAngle& theAngle)
{
  itsValue = NFmiAngle(itsValue + theAngle.itsValue).itsValue;
  return *this;
}

// ======================================================================
/*!
 * V‰hennysoperaattori.
 *
 * \bug Toteutus vaikuttaa tyhm‰lt‰. Miksei k‰ytet‰ SetValue metodia,
 * vaan luodaan turha temppi?
 *
 * \param theAngle Undocumented
 * \return Undocumented
 */
// ======================================================================

NFmiAngle& NFmiAngle::operator-=(const NFmiAngle& theAngle)
{
  itsValue = NFmiAngle(itsValue - theAngle.itsValue).itsValue;
  return *this;
}

// ======================================================================
/*!
 * Konstruktori luokalle NFmiWindDirection asettaa sis‰iset arvot k‰ytt‰en
 * NFmiAngle luokan SetValue metodia, jotta mahdollinen kulmamuunnos tulee
 * suoritetuksi, ja kulman rajat tarkistetuiksi.
 *
 * \param theValue Undocumented
 * \param theUnit Undocumented
 */
// ======================================================================

NFmiWindDirection::NFmiWindDirection(double theValue, FmiAngleUnit theUnit)
    : NFmiAngle(theValue, theUnit)
{
}

// ======================================================================
/*!
 * Spesiaalikonstruktori luokalle NFmiWindDirection asettaa sis‰iset arvot
 * perustuen tuulen U- ja V-komponentteihin. Jos kumpi tahansa komponentti
 * on kFloatMissing, asetetaan myˆs kulma puuttuvaksi.
 *
 * \param theU Undocumented
 * \param theV Undocumented
 */
// ======================================================================

NFmiWindDirection::NFmiWindDirection(double theU, double theV) : NFmiAngle()
{
  if (theU == kFloatMissing || theV == kFloatMissing)
  {
    itsValue = kFloatMissing;
    return;
  }

  if (theU == 0 && theV == 0)
  {
    SetValue(0, kDeg);
  }
  else
  {
    SetValue(atan2(theU, theV) + kPii, kRad);
  }
}

// ======================================================================
/*!
 * V‰hennysoperaattori.
 *
 * \bug Siis, jos suunta on operaation j‰lkeen > 180, esim. 200, niin
 * 360-200 = 160, uusi suunta. T‰m‰h‰n on peilausta eik‰ moduloa.
 * Mik‰ hiton logiikka t‰ss‰ on? Onkos n‰iss‰ luokissa kaikki vialla?
 * Itse toteutuskin on omituinen temppiluokkineen. Koko perint‰
 * on hieman omituista, kaikki virtuaalimetodit puuttuvat.
 *
 * \param theAngle Undocumented
 * \return Undocumented
 */
// ======================================================================

NFmiWindDirection& NFmiWindDirection::operator-=(const NFmiWindDirection& theAngle)
{
  itsValue = NFmiAngle(NFmiAngle::operator-=(theAngle)).Value();
  if (itsValue > 180) itsValue = 360 - itsValue;
  return *this;
}

// ======================================================================
/*!
 * Konstruktori luokalle NFmiLatitude asettaa sis‰iset arvot k‰ytt‰en
 * NFmiLatitude luokan SetValue metodia, jotta mahdollinen kulmamuunnos tulee
 * suoritetuksi, ja kulman rajat tarkistetuiksi.
 *
 * \param theAngle Undocumented
 * \param theUnit Undocumented
 */
// ======================================================================

NFmiLatitude::NFmiLatitude(double theAngle, FmiAngleUnit theUnit) { SetValue(theAngle, theUnit); }
// ======================================================================
/*!
 * Asetusmetodi latitudille. Metodi varmistaa, ett‰ annettu arvo
 * rajoitetaan v‰lille -90 <= latitudi <= 90.
 *
 * \bug Annettu kulmayksikkˆ j‰tet‰‰n t‰ysin huomioimatta, selv‰stikin
 * oletetaan, ett‰ yksikkˆ on asteina. Lis‰ksi oletettujen
 * pyˆristysvirheiden korjailu on aina riskaabelia. Lis‰ksi SetValue
 * kutsuu joskus itse‰‰n uudelleen, mik‰ on v‰h‰n ihmeellist‰, kun
 * arvo kerran tunnetaan.
 *
 * \param theAngle Undocumented
 * \param theUnit Undocumented
 */
// ======================================================================

void NFmiLatitude::SetValue(double theAngle, FmiAngleUnit theUnit)
{
  if (fabs(theAngle - 90) < 0.0000001)
    itsValue = 90;
  else if (fabs(theAngle + 90) < 0.0000001)
    itsValue = -90;
  else if (theAngle > 90)
    SetValue(90, theUnit);
  else if (theAngle < -90)
    SetValue(-90, theUnit);
  else
    itsValue = theAngle;
}

// ======================================================================
/*!
 * V‰hennysoperaattori.
 *
 * \bug Ja taas luodaan turha temppi SetValue metodin kutsumisen sijaan.
 *
 * \param theAngle Undocumented
 * \return Undocumented
 */
// ======================================================================

NFmiLatitude& NFmiLatitude::operator-=(const NFmiLatitude& theAngle)
{
  itsValue = NFmiLatitude(itsValue - theAngle.itsValue).itsValue;
  return *this;
}

// ======================================================================
/*!
 * Lis‰ysoperaattori
 *
 * \bug Ja taas luodaan turha temppi SetValue metodin kutsumisen sijaan.
 *
 * \param theAngle Undocumented
 * \return Undocumented
 */
// ======================================================================

NFmiLatitude& NFmiLatitude::operator+=(const NFmiLatitude& theAngle)
{
  itsValue = NFmiLatitude(itsValue + theAngle.itsValue).itsValue;
  return *this;
}

// ======================================================================
/*!
 * V‰hennysoperaattori annetulla kulmalla.
 *
 * \bug Ja taas luodaan turha temppi SetValue metodin kutsumisen sijaan.
 *
 * \param theAngle Undocumented
 * \return Undocumented
 */
// ======================================================================

NFmiLatitude& NFmiLatitude::operator-=(const double& theAngle)
{
  itsValue = NFmiLatitude(itsValue - theAngle).itsValue;
  return *this;
}

// ======================================================================
/*!
 * Lis‰ysoperaattori
 *
 * \bug Ja taas luodaan turha temppi SetValue metodin kutsumisen sijaan.
 *
 * \param theAngle Undocumented
 * \return Undocumented
 */
// ======================================================================

NFmiLatitude& NFmiLatitude::operator+=(const double& theAngle)
{
  itsValue = NFmiLatitude(itsValue + theAngle).itsValue;
  return *this;
}

// ======================================================================
/*!
 * Konstruktori luokalle NFmiLongitude asettaa sis‰iset arvot k‰ytt‰en
 * NFmiLongitude luokan SetValue metodia, jotta mahdollinen kulmamuunnos tulee
 * suoritetuksi, ja kulman rajat tarkistetuiksi.
 *
 * \param theAngle Undocumented
 * \param theUnit Undocumented
 */
// ======================================================================

NFmiLongitude::NFmiLongitude(double theAngle, bool usePacificView, FmiAngleUnit theUnit)
    : fPacificView(usePacificView)
{
  SetValue(theAngle, theUnit);
}

// ======================================================================
/*!
 * Asetusmetodi longitudille. Metodi varmistaa, ett‰ annettu arvo
 * rajoitetaan v‰lille -180 <= latitudi <= 180.
 *
 * \bug Annettu kulmayksikkˆ j‰tet‰‰n t‰ysin huomioimatta, selv‰stikin
 * oletetaan, ett‰ yksikkˆ on asteina. Lis‰ksi k‰ytet‰‰n rekursiota
 * rajaehtojen toteuttamiseksi, kun pit‰isi k‰ytt‰‰ oikeata mod
 * k‰sky‰. Muuten k‰sky voi kest‰‰ tarpeettoman kauan. Mit‰ jos
 * joku antaa arvon 100000? Monta iteraatiota menee...?
 *
 * \param theAngle Undocumented
 * \param theUnit Undocumented
 */
// ======================================================================

void NFmiLongitude::SetValue(double theAngle, FmiAngleUnit theUnit)
{
  if (fPacificView)
  {
    if (::fabs(theAngle - 360.) < 0.0000001)
      itsValue = 360.;
    else if (::fabs(theAngle + 360.) < 0.0000001)
      itsValue = 0.;
    else if (theAngle > 360)
      SetValue(theAngle - 360, theUnit);
    else if (theAngle < 0)
      SetValue(theAngle + 360, theUnit);
    else
      itsValue = theAngle;
  }
  else
  {
    if (::fabs(theAngle - 180.) < 0.0000001)
      itsValue = 180.;
    else if (::fabs(theAngle + 180.) < 0.0000001)
      itsValue = -180.;
    else if (theAngle > 180)
      SetValue(theAngle - 360, theUnit);
    else if (theAngle < -180)
      SetValue(theAngle + 360, theUnit);
    else
      itsValue = theAngle;
  }
}

// ======================================================================
/*!
 * V‰hennysoperaattori.
 *
 * \bug Ja taas luodaan turha temppi SetValue metodin kutsumisen sijaan.
 *
 * \param theAngle Undocumented
 * \return Undocumented
 */
// ======================================================================

NFmiLongitude& NFmiLongitude::operator-=(const NFmiLongitude& theAngle)
{
  itsValue = NFmiLongitude(itsValue - theAngle.itsValue, fPacificView).itsValue;
  return *this;
}

// ======================================================================
/*!
 * Lis‰ysoperaattori
 *
 * \bug Ja taas luodaan turha temppi SetValue metodin kutsumisen sijaan.
 *
 * \param theAngle Undocumented
 * \return Undocumented
 */
// ======================================================================

NFmiLongitude& NFmiLongitude::operator+=(const NFmiLongitude& theAngle)
{
  itsValue = NFmiLongitude(itsValue + theAngle.itsValue, fPacificView).itsValue;
  return *this;
}

// ======================================================================
/*!
 * V‰hennysoperaattori annetulla kulmalla.
 *
 * \bug Ja taas luodaan turha temppi SetValue metodin kutsumisen sijaan.
 *
 * \param theAngle Undocumented
 * \return Undocumented
 */
// ======================================================================

NFmiLongitude& NFmiLongitude::operator-=(const double& theAngle)
{
  itsValue = NFmiLongitude(itsValue - theAngle, fPacificView).itsValue;
  return *this;
}

// ======================================================================
/*!
 * Lis‰ysoperaattori annetulla kulmalla.
 *
 * \bug Ja taas luodaan turha temppi SetValue metodin kutsumisen sijaan.
 *
 * \param theAngle Undocumented
 * \return Undocumented
 */
// ======================================================================

NFmiLongitude& NFmiLongitude::operator+=(const double& theAngle)
{
  itsValue = NFmiLongitude(itsValue + theAngle, fPacificView).itsValue;
  return *this;
}

// ======================================================================
