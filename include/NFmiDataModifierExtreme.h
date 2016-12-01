// ======================================================================
/*!
 * \file NFmiDataModifierExtreme.h
 * \brief abstrakti luokka min/max:ia varten joka my�s antaa ��rirvon
 *  sattumisajan/Lasse
 */
// ======================================================================

#ifndef NFMIDATAMODIFIEREXTREME_H
#define NFMIDATAMODIFIEREXTREME_H

#include "NFmiDataModifier.h"
#include "NFmiMetTime.h"

//! Undocumented
class _FMI_DLL NFmiDataModifierExtreme : public NFmiDataModifier
{
 public:
  virtual ~NFmiDataModifierExtreme(void);
  NFmiDataModifierExtreme(void);
  NFmiDataModifierExtreme(const NFmiDataModifierExtreme &theOther);

  void SetTime(NFmiQueryInfo *theQI);
  const NFmiTime GetTime(void);
  virtual void Calculate(NFmiQueryInfo *theQI);
  virtual void Calculate(float theValue);
  virtual void Calculate(float theValue, NFmiQueryInfo *theQI);

 protected:
  virtual bool IsNewExtreme(float) = 0;

 public:
  float itsExtremeValue;
  NFmiTime itsExtremeTime;

};  // class NFmiDataModifierExtreme

#endif  // NFMIDATAMODIFIEREXTREME_H

// ======================================================================
