// ======================================================================
/*!
 * \file NFmiGeomList.h
 * \brief Declaration of struct GeomList
 */
// ======================================================================
/*!
 * \struct GeomList
 *
 * \deprecated
 * N‰ytt‰‰ silt‰, ettei GeomList structia eik‰ GEOMNAMEMAXSIZE m‰‰ritelm‰‰
 * k‰ytet‰‰ miss‰‰n kirjastossa. Siten se tulisi poistaa. Toisaalta koodi
 * on niin C-tyyppist‰, ettei se kuulu C++ kirjastoon.
 *
 * \todo Ota selv‰‰, tarvitaanko t‰t‰ tiedostoa. Ilmeisesti struct on
 * Vilin m‰‰rittelem‰.
 */
// ======================================================================

#ifndef GEOMLIST_H
#define GEOMLIST_H

#define GEOMNAMEMAXSIZE 15

//! Jonkinlainen struct nimien ja kokojen s‰ilyt‰miseen
typedef struct
{
  long size;
  char geomName[GEOMNAMEMAXSIZE][GEOMNAMELENGTH];
} GeomList;

#endif  // GEOMLIST_H

// ======================================================================
