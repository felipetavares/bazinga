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

#ifndef RI_SM4_H
#define RI_SM4_H

#include <string>
#include <sstream>
#include "../vec4/vec4.h"
#include <cmath>

/* Squared Fourth Dimensional Matrix */
class sm4 {
public:
  vec4 a,b,c,d; // Lines. LINES!!!
  // a.x a.y a.z a.w
  // b.x b.y b.z b.w
  // c.x c.y c.z c.w
  // d.x d.y d.z d.w
 
  sm4 ();
  sm4 (const vec4&,
       const vec4&,
       const vec4&,
       const vec4&);

  sm4 operator = (const sm4&);
  sm4 operator + (const sm4&);
  sm4 operator - (const sm4&);

  sm4 operator * (const double);
  sm4 operator * (sm4&);
  vec4 operator * (vec4);

  sm4 operator += (const sm4&);
  sm4 operator -= (const sm4&);
  sm4 operator *= (sm4&);
  sm4 operator *= (const double);

  static sm4 rotateX (double);
  static sm4 rotateY (double);
  static sm4 rotateZ (double);
  

  sm4 t();
  void identity();

  vec4 getColumn (const unsigned int);

  std::string str();
};

#endif /* RI_SM4_H */
