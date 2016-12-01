// ======================================================================
/*!
 * \file NFmiAngle.h
 * \brief Deklaraatiot luokille NFmiAngle, NFmiWindDirection, NFmiLatitude
 *        ja NFmiLongitude.
 */
// ======================================================================

#ifndef NFMIANGLE_H
#define NFMIANGLE_H

#include "NFmiGlobals.h"
#include <cmath>

//! Tyyppim‰‰ritelm‰ kulmayksikˆille.
typedef enum
{
  kDeg,
  kRad
} FmiAngleUnit;

//! Yleinen kulman m‰‰rittely perusluokka

class _FMI_DLL NFmiAngle
{
 public:
  //! Destruktori
  virtual ~NFmiAngle() {}
  //! Konstruktori
  NFmiAngle(double theAngle = 0.0, FmiAngleUnit theUnit = kDeg);

  //! Kulman asetus optionaalisessa yksikˆss‰
  void SetValue(double theValue, FmiAngleUnit theUnit = kDeg);

  //! Kulman palautus
  double Value(void) const { return itsValue; }
  //! Suurempi kuin vertailuoperaattori
  bool operator>(const NFmiAngle &theAngle) const;

  //! Lis‰ysoperaattori
  NFmiAngle &operator+=(const NFmiAngle &theAngle);

  //! V‰hennysoperaattori
  NFmiAngle &operator-=(const NFmiAngle &theAngle);

  //! Muunnos radiaaneiksi
  double ToRad(void) const { return itsValue / 180.0 * kPii; }
  //! Muunnos asteiksi
  double ToDeg(void) const { return itsValue; }
  // 14.5.2002/Marko+Mika MSVC++:ssa ei sin ja muut vastaavat ole std:ssa, siit‰ using viritys.

  //! Kulman sini
  double Sin(void) const
  {
    using namespace std;
    return sin(ToRad());
  }

  //! Kulman kosini
  double Cos(void) const
  {
    using namespace std;
    return cos(ToRad());
  }

  //! Kulman tangentti
  double Tan(void) const
  {
    using namespace std;
    return tan(ToRad());
  }

  //! Kulman arcustangentti
  double Atan(void) const
  {
    using namespace std;
    return atan(ToRad());
  }

 protected:
  //! Itse kulma asteissa.
  double itsValue;
};

// ======================================================================
/*!
 * \class NFmiWindDirection
 *
 * M‰‰rittely yleiselle kulmasuureelle. Luokan keskeinen ominaisuus on, ett‰
 * luokan metodit huolehtivat automaattisesti, ett‰ kulma pysyy sallituissa
 * rajoissa jokaisen operaation j‰lkeen, eli operaatiot toimivat modulo 360.
 *
 * Suunnan vaihtelurajat ovat 0 <= arvo <= 360, jossa arvo=0 tarkoittaa
 * vaihtelevaa tuulta.
 */
// ======================================================================

//! Tuulen suunnan esitysluokka

class _FMI_DLL NFmiWindDirection : public NFmiAngle
{
 public:
  //! Konstruktori suunnan perusteella
  NFmiWindDirection(double theAngle, FmiAngleUnit theUnit = kDeg);

  //! Konstruktori suunnan U- ja V-komponenttien perusteella
  NFmiWindDirection(double theU, double theV);

  //! Kulman v‰hennysoperaattori
  NFmiWindDirection &operator-=(const NFmiWindDirection &theAngle);
};

// ======================================================================
/*!
 * \class NFmiLatitude
 *
 * M‰‰rittely yleiselle latitudisuureelle. Luokan keskeinen ominaisuus on, ett‰
 * luokan metodit huolehtivat automaattisesti, ett‰ kulma pysyy sallituissa
 * rajoissa jokaisen operaation j‰lkeen, eli operaatiot toimivat symmetrisen‰
 * modulona 180.
 *
 * Latitudin vaihtelurajat ovat -90 <= latitudi <= 90
 */
// ======================================================================

//! Latitudin esitysluokka

class _FMI_DLL NFmiLatitude : public NFmiAngle
{
 public:
  //! Konstruktori
  NFmiLatitude(double theAngle, FmiAngleUnit theUnit = kDeg);

  NFmiLatitude() {}
  //! Latitudin muutosmetodi
  void SetValue(double theAngle, FmiAngleUnit theUnit = kDeg);

  //! Latitudin v‰hennysoperaattori
  NFmiLatitude &operator-=(const NFmiLatitude &theAngle);

  //! Latitudin kasvatusoperaattori
  NFmiLatitude &operator+=(const NFmiLatitude &theAngle);

 private:
  //! Latitudin v‰hennysoperaattori annetun kulman perusteella
  NFmiLatitude &operator-=(const double &theAngle);

  //! Latitudin kasvatusoperaattori annetun kulman perusteella
  NFmiLatitude &operator+=(const double &theAngle);
};

// ======================================================================
/*!
 * \class NFmiLongitude
 *
 * M‰‰rittely yleiselle longitudisuureelle. Luokan keskeinen ominaisuus on, ett‰
 * luokan metodit huolehtivat automaattisesti, ett‰ kulma pysyy sallituissa
 * rajoissa jokaisen operaation j‰lkeen, eli operaatiot toimivat symmetrisen‰
 * modulona 360.
 *
 * Longitudin vaihtelurajat ovat -180 <= latitudi <= 180
 */
// ======================================================================

//! Longitudin esitysluokka

class _FMI_DLL NFmiLongitude : public NFmiAngle
{
 public:
  //! Konstruktori
  NFmiLongitude(double theAngle, bool usePacificView, FmiAngleUnit theUnit = kDeg);

  NFmiLongitude() : fPacificView(false) {}
  //! Longitudin muutosmetodi
  void SetValue(double theValue, FmiAngleUnit theUnit = kDeg);

  //! Longitudin v‰hennysoperaattori
  NFmiLongitude &operator-=(const NFmiLongitude &theAngle);

  //! Longitudin lis‰ysoperaattori
  NFmiLongitude &operator+=(const NFmiLongitude &theAngle);

 private:
  //! Longitudin v‰hennysoperaattori annetulla kulmalla
  NFmiLongitude &operator-=(const double &theAngle);

  //! Longitudin lis‰ysoperaattori annetulla kulmalla
  NFmiLongitude &operator+=(const double &theAngle);

  bool fPacificView;
};

#endif  // NFMIANGLE_H

// ======================================================================
