// ======================================================================
/*!
 * \file NFmiMetBoxQueryData.h
 * \brief Interface of class NFmiMetBoxQueryData
 */
// ======================================================================

#pragma once

#include "NFmiQueryData.h"
#include "NFmiMetBox.h"

//! Undocumented
class _FMI_DLL NFmiMetBoxQueryData : public NFmiQueryData
{
 public:
  ~NFmiMetBoxQueryData(void);
  NFmiMetBoxQueryData(void);
  NFmiMetBoxQueryData(const NFmiMetBoxQueryData& theData);
  NFmiMetBoxQueryData(NFmiQueryInfo& theInfo);

  virtual NFmiMetBox* Value(void);

 private:
  NFmiMetBoxQueryData& operator=(const NFmiMetBoxQueryData& theData);

  NFmiMetBox* itsMetBox;

};  // class NFmiMetBoxQueryData


// ======================================================================
