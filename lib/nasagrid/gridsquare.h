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

#ifndef GRID_SQUARE_H
#define GRID_SQUARE_H

#include <iostream>

/** Abstract base class for GridSquares.
  * A grid square is 1 latitude degree x 1 longitude degree. */
class GridSquare
{
public:
  typedef short height_t;
  const static height_t NO_DATA =-32768; ///< NASA's magic number for no data.
  
  /** Formula for grid squares' unique keys. */
  static int key(int lng, int lat)
  {
    return 1000*lat + lng;
  }

  virtual ~GridSquare();

  /** Calculate unique key for this object. */
  int key() const {return key(longitude,latitude);}
  unsigned int cols() const {return num_cols;}
  unsigned int rows() const {return num_rows;}
  bool has_data() const {return bool(data);}

  /** Returns the height above sea level for the given location
    * within this grid square. */
  virtual height_t height(float lng_fraction, float lat_fraction) const =0;

protected:
  void* data; ///< mmap'd data file.
  int longitude;
  int latitude;
  int fd; ///< File ID for mmap'd data file.
  unsigned int num_bytes; ///< Length of data.
  unsigned int num_rows;
  unsigned int num_cols;

  GridSquare(int lng, int lat);
  std::string make_filename(const char* ext) const;
  void parse_filename(const char* filename, int& lng, int& lat);
  
  /** Loads the file into memory, pointed to by 'data'. Sets num_bytes. */
  void load(const char* filename);
  void unload();
  /** Converts lng,lat floats to col,row ints. Returns TRUE if result valid. */
  bool lngLat_to_colRow(
    float lng_fraction, float lat_fraction,
    unsigned int& col,  unsigned int& row
  ) const;
}; // end class GridSquare

#endif // GRID_SQUARE_H
