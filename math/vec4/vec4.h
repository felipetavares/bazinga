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

#ifndef RI_VEC4_H
#define RI_VEC4_H

#include <string>
#include <sstream>
#include "../vec2/vec2.h"
#include "../vec3/vec3.h"

class vec4 {
public:
  double x,y,z,w;

  double operator [] (const int);

  vec4 operator = (const vec4&);
  vec4 operator * (const double);
  vec4 operator / (const double);
  vec4 operator + (const vec4&);
  vec4 operator - (const vec4&);

  vec4 operator /= (const double);
  vec4 operator *= (const double);
  vec4 operator += (const vec4&);
  vec4 operator -= (const vec4&);

  double dot (const vec4&);
  
  double length ();
  vec4 normal ();
  vec4 normalize();

  std::string str ();

  // Constructors!
  vec4 ();
  vec4 (double,double,double,double);
  vec4 (const vec4&);
  vec4 (const vec2&,double,double);
  vec4 (const vec2&,const vec2&);
  vec4 (const vec3&, double);

  vec2 toVec2 ();
  vec3 toVec3 ();

  // Destructor
  ~vec4 ();
};

#endif /* RI_VEC4_H */
