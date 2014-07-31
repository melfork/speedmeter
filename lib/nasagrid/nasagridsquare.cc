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

#include "nasagridsquare.h"

#include "utils.h"

#include <iostream>
#include <string>

NasaGridSquare::NasaGridSquare(const char* filename): GridSquare(0,0)
{
  parse_filename(filename,longitude,latitude);
  load(filename);
  if(has_data())
      num_rows=num_cols=FloodUtils::sqrt(num_bytes/2);
}

NasaGridSquare::NasaGridSquare(int lng, int lat): GridSquare(lng,lat)
{
  using FloodUtils::sqrt;

  std::string filename =make_filename("hgt");
  DB("NasaGridSquare: "<<filename<<" ("<<key()<<")")
  load(filename.c_str());
  if(has_data())
      num_rows=num_cols=sqrt(num_bytes/2);
}

GridSquare::height_t NasaGridSquare::height(float lng_fraction, float lat_fraction) const
{
  if(!data)
      return NO_DATA;
  unsigned int col,row;
  if(!lngLat_to_colRow(lng_fraction,lat_fraction, col,row))
      return NO_DATA;
  return get_height(col,row);
}

GridSquare::height_t NasaGridSquare::get_height(unsigned int col, unsigned int row) const
{
  if(col>=num_cols || row>=num_rows)
      return NO_DATA;
  unsigned int i=( col + row * num_cols );
  unsigned short datum=((unsigned short*)data)[i];
  height_t result=(datum>>8 | datum<<8);
  return result;
}

