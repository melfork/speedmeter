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

#include "gridsquare.h"

#include "utils.h"

#include <errno.h>
#include <fcntl.h>
#include <iomanip>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

GridSquare::~GridSquare()
{
  if(data)
      unload();
}

GridSquare::GridSquare(int lng, int lat):
  data(NULL), longitude(lng), latitude(lat),
  fd(-1), num_bytes(0), num_rows(1), num_cols(1)
{}

std::string GridSquare::make_filename(const char* ext) const
{
  using namespace std;
  char EW =(longitude>=0?'E':'W');
  char NS =(latitude>=0?'N':'S');
  stringstream ss;
#ifdef ROOT_PATH
  static const char* root_path =ROOT_PATH;
#else
  static const char* root_path =FloodUtils::getenvz("ROOT_PATH",".");
#endif
  ss<<setfill('0')
    << root_path
    <<"/data/"
    <<NS
    <<setw(2)<<abs(latitude)<<setw(0)
    <<EW
    <<setw(3)<<abs(longitude)<<setw(0)
    <<'.'<<ext;
  return ss.str();
}

void GridSquare::parse_filename(const char* filename, int& lng, int& lat)
{
  char fn[12]; // N51E001.xyz
  FloodUtils::strlcpy(fn,filename,sizeof(fn));
  lng=( fn[3]=='E'? 1: -1 );
  lat=( fn[0]=='N'? 1: -1 );
  fn[3]=fn[7]='\0';
  lng *= ::atoi(fn+4);
  lat *= ::atoi(fn+1);
}

void GridSquare::load(const char* filename)
{
  const std::string gzipfile =std::string(filename)+".gz";
  bool triedZipped =false;
  while(true)
  {
    fd=::open(filename,O_RDONLY);
    if(fd>=0)
        break;
    DEBUG_ONLY(int saved_errno =errno;)
    if(ENOENT!=errno || triedZipped || !FloodUtils::exists(gzipfile.c_str()))
    {
      DB("Failed to open file "<<filename<<": "<<::strerror(saved_errno))
      return;
    }
    // Unzip and try again
    DB("Unzipping "<<gzipfile)
    std::stringstream ss;
    ss<<"/bin/gzip -dc "<<gzipfile<<" > "<<filename;
    int status =::system(ss.str().c_str());
    if(-1 == status)
    {
      ERR("Failed to execute shell command ("<<ss.str()<<"): "<<::strerror(errno))
      ::exit(-1);
    }
    else if(0 != WEXITSTATUS(status))
    {
      ERR("Failed to unzip file "<<gzipfile<<": "<<WEXITSTATUS(status))
      ::exit(-1);
    }
    triedZipped=true;
  }

  // Get file length
  struct stat file_stat;
  if( ::fstat(fd,&file_stat)<0 )
  {
    ::perror("fstat error");
    ::exit(-1);
  }
  num_bytes=file_stat.st_size;
  if(!num_bytes)
  {
    ::close(fd);
    return;
  }

  data =::mmap(0,num_bytes,PROT_READ,MAP_SHARED,fd,0);
  if(MAP_FAILED==data)
  {
    ::perror("mmap error");
    ::exit(-1);
  }
}

void GridSquare::unload()
{
  if(::munmap(data,num_bytes)<0)
  {
    ::perror("munmap error");
    ::exit(-1);
  }
  ::close(fd);
}

bool GridSquare::lngLat_to_colRow(
  float lng_fraction, float lat_fraction,
  unsigned int& col,  unsigned int&  row
) const
{
  col =             int( 0.5 + lng_fraction * float(num_cols-1) );
  row =num_rows-1 - int( 0.5 + lat_fraction * float(num_rows-1) );
  return( col<num_cols && row<num_rows );
}
