/*
  Copyright (C) 2013 by Felipe Tavares

  This file is part of Render TRI.
  
  Render TRI is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  Render TRI is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with Render TRI.  If not, see <http://www.gnu.org/licenses/>. 
*/

#ifndef RI_VEC3_H
#define RI_VEC3_H

#include <string>
#include <sstream>
#include "../vec2/vec2.h"

class vec3 {
public:
  double x,y,z;

  double operator [] (const int);

  vec3 operator = (const vec3&);
  vec3 operator * (const double);
  vec3 operator / (const double);
  vec3 operator + (const vec3&);
  vec3 operator - (const vec3&);

  vec3 operator * (const vec3&);	

  vec3 operator /= (const double);
  vec3 operator *= (const double);
  vec3 operator += (const vec3&);
  vec3 operator -= (const vec3&);

  vec3 operator *= (const vec3&);

  double dot (const vec3&);
  
  double length ();
  vec3 normal ();
  vec3 normalize();

  std::string str ();

  // Constructors!
  vec3 ();
  vec3 (double,double,double);
  vec3 (const vec3&);
  vec3 (const vec2&,double);

  vec2 toVec2 ();

  // Destructor
  ~vec3 ();
};

#endif /* RI_VEC3_H */
