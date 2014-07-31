/*
Copyright (c) 2006  Alex Tingle

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef NASA_GRID_SQUARE_H
#define NASA_GRID_SQUARE_H

#include "gridsquare.h"

class NasaGridSquare: public GridSquare
{
public:

  NasaGridSquare(const char* filename);

  NasaGridSquare(int lng, int lat);

  height_t height(float lng_fraction, float lat_fraction) const;

private:
  height_t get_height(unsigned int col, unsigned int row) const;
  
  friend class ReducedGridSquare;

}; // end class NasaGridSquare

#endif // NASA_GRID_SQUARE_H
