// ======================================================================
/*!
 * \file NFmiPoint.cpp
 * \brief Implementation of class NFmiPoint
 */
// ======================================================================
/*!
 * \class NFmiPoint
 *
 * NFmiPoint-luokka pit‰‰ sis‰ll‰‰n tiedon kaksiulotteisesta pisteest‰. Luokalla
 * on tieto x- ja y-sijainnista. Voidaan k‰ytt‰‰ esitt‰m‰‰n mm. suhteelisia ja
 * maantieteellisia sijainteja.
 * Helsingin sijainti voidaan esim. ilmoittaa seuraavasti (lon-lat muodossa):
 * NFmiPoint hki(24.95, 60.1667);
 *
 */
// ======================================================================
/*!
 * \example example_NFmiPoint.cpp
 *
 * Esimerkki NFmiPoint-luokan k‰ytˆst‰.
 *  Polku: newbase/examples/example_NFmiPoint.cpp
 * Luo erilaisia NFmiPoint ilmentymi‰ ja manipuloi niit‰ ja tulostaa
 * olioiden arvoja cout:iin.
 */
// ======================================================================

#include "NFmiPoint.h"
#include <fstream>

const NFmiPoint NFmiPoint::gMissingLatlon = NFmiPoint(kFloatMissing, kFloatMissing);

// ----------------------------------------------------------------------
/*!
 * Set the X- and Y-coordinates of the point
 *
 * \param theX The new X-coordinate
 * \param theY The new Y-coordinate
 */
// ----------------------------------------------------------------------

void NFmiPoint::Set(double theX, double theY)
{
  itsX = theX;
  itsY = theY;
}

// ----------------------------------------------------------------------
/*!
 * Addition operator
 *
 * \param thePoint The coordinates to be added to the point
 * \return The point added to
 */
// ----------------------------------------------------------------------

NFmiPoint& NFmiPoint::operator+=(const NFmiPoint& thePoint)
{
  itsX += thePoint.itsX;
  itsY += thePoint.itsY;
  return *this;
}

// ----------------------------------------------------------------------
/*!
 * -=operator v‰hent‰‰ itsest‰‰n annetun pisteen arvot.
 * X- ja Y-arvot v‰hennet‰‰n erikseen toisistaan ja sijoitetaan
 * this-NFmiPoint:iin.
 *
 * \param thePoint The other object being copied
 * \return The object assigned to
 */
// ----------------------------------------------------------------------

NFmiPoint& NFmiPoint::operator-=(const NFmiPoint& thePoint)
{
  itsX -= thePoint.itsX;
  itsY -= thePoint.itsY;
  return *this;
}

// ----------------------------------------------------------------------
/*!
 * Coordinate-wise multiplication by the given point
 *
 * \param thePoint The other point by which to multiply
 * \return The multiplied point
 */
// ----------------------------------------------------------------------

NFmiPoint& NFmiPoint::operator*=(const NFmiPoint& thePoint)
{
  itsX *= thePoint.itsX;
  itsY *= thePoint.itsY;
  return *this;
}

// ----------------------------------------------------------------------
/*!
 * Coordinate-wise division by the given point
 *
 * \param thePoint The other point by which to divide
 * \return The divided point
 * \bug There is no protection against division by zero
 */
// ----------------------------------------------------------------------

NFmiPoint& NFmiPoint::operator/=(const NFmiPoint& thePoint)
{
  itsX /= thePoint.itsX;
  itsY /= thePoint.itsY;
  return *this;
}

//___________________________________________________________
/*!
 * Laskee miss‰ kulmassa (kUpLeft, kUpRight, kLeftDown ja kRightDown)
 * annettu piste on suhteessa this-pisteeseen. Ei ota huomioon, ett‰
 * voisi olla samassa tasossa joltain kantilta eli vastaus ei voi olla
 * pelkk‰ left, right, top tai bottom.
 *
 * \param thePoint The point whose relative position is to be determined
 * \return The relative direction of the point
 */
// ----------------------------------------------------------------------

FmiDirection NFmiPoint::DirectionOfDifference(const NFmiPoint& thePoint) const
{
  int quarterOfOrigin = static_cast<int>(itsY < thePoint.itsY ? kUp : kDown);
  return FmiDirection(itsX < thePoint.itsX ? quarterOfOrigin | static_cast<int>(kLeft)
                                           : quarterOfOrigin | static_cast<int>(kRight));
}

// ----------------------------------------------------------------------
/*!
 * Write the object to the given output stream
 *
 * \param file The output stream to write to
 * \return The output stream written to
 */
// ----------------------------------------------------------------------

std::ostream& NFmiPoint::Write(std::ostream& file) const
{
  file << itsX << " ";
  file << itsY << std::endl;
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

std::istream& NFmiPoint::Read(std::istream& file)
{
  file >> itsX;
  file >> itsY;
  return file;
}

// ======================================================================
