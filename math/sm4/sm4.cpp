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

#include "sm4.h"

sm4::sm4 () {
}

sm4::sm4 (const vec4& a,
	  const vec4& b,
	  const vec4& c,
	  const vec4& d) {
  this->a = a;
  this->b = b;
  this->c = c;
  this->d = d;
}

sm4 sm4::operator = (const sm4& mat) {
  a = mat.a;
  b = mat.a;
  c = mat.c;
  d = mat.d;
}

sm4 sm4::operator + (const sm4& mat) {
  sm4 r;

  r.a = a+mat.a;
  r.b = b+mat.b;
  r.c = c+mat.c;
  r.d = d+mat.d;

  return r;
}

sm4 sm4::operator - (const sm4& mat) {
  sm4 r;

  r.a = a-mat.a;
  r.b = b-mat.b;
  r.c = c-mat.c;
  r.d = d-mat.d;

  return r;
}

sm4 sm4::operator * (const double s) {
  sm4 r;

  r.a = a*s;
  r.b = b*s;
  r.c = c*s;
  r.d = d*s;

  return r;
}

sm4 sm4::operator * (sm4& mat) {
  sm4 r;

  r.a.x = a.dot(mat.getColumn(0));
  r.a.y = a.dot(mat.getColumn(1));
  r.a.z = a.dot(mat.getColumn(2));
  r.a.w = a.dot(mat.getColumn(3));

  r.b.x = b.dot(mat.getColumn(0));
  r.b.y = b.dot(mat.getColumn(1));
  r.b.z = b.dot(mat.getColumn(2));
  r.b.w = b.dot(mat.getColumn(3));

  r.c.x = c.dot(mat.getColumn(0));
  r.c.y = c.dot(mat.getColumn(1));
  r.c.z = c.dot(mat.getColumn(2));
  r.c.w = c.dot(mat.getColumn(3));  

  r.d.x = d.dot(mat.getColumn(0));
  r.d.y = d.dot(mat.getColumn(1));
  r.d.z = d.dot(mat.getColumn(2));
  r.d.w = d.dot(mat.getColumn(3));  

  return r;
}

sm4 sm4::operator += (const sm4& mat) {
  a += mat.a;
  b += mat.b;
  c += mat.c;
  d += mat.d;

  return *this;  
}


sm4 sm4::operator -= (const sm4& mat) {
  a -= mat.a;
  b -= mat.b;
  c -= mat.c;
  d -= mat.d;

  return *this;  
}

sm4 sm4::operator *= (sm4& mat) {
  vec4 r;

  r.x = a.dot(mat.getColumn(0));
  r.y = a.dot(mat.getColumn(1));
  r.z = a.dot(mat.getColumn(2));
  r.w = a.dot(mat.getColumn(3));
  a = r;

  r.x = b.dot(mat.getColumn(0));
  r.y = b.dot(mat.getColumn(1));
  r.z = b.dot(mat.getColumn(2));
  r.w = b.dot(mat.getColumn(3));
  b = r;

  r.x = c.dot(mat.getColumn(0));
  r.y = c.dot(mat.getColumn(1));
  r.z = c.dot(mat.getColumn(2));
  r.w = c.dot(mat.getColumn(3));  
  c = r;

  r.x = d.dot(mat.getColumn(0));
  r.y = d.dot(mat.getColumn(1));
  r.z = d.dot(mat.getColumn(2));
  r.w = d.dot(mat.getColumn(3));  
  d = r;

  return *this;
}

sm4 sm4::operator *= (const double s) {
  a *= s;
  b *= s;
  c *= s;
  d *= s;

  return *this;
}

vec4 sm4::operator * (vec4 v) {
  vec4 r;

  r.x = v.dot(getColumn(0));
  r.y = v.dot(getColumn(1));
  r.z = v.dot(getColumn(2));
  r.w = v.dot(getColumn(3));

  return r;
}

sm4 sm4::t () {
  sm4 r;

  r.a.x = a.x;
  r.b.x = a.y;
  r.c.x = a.z;
  r.d.x = a.w;

  r.a.x = b.x;
  r.b.x = b.y;
  r.c.x = b.z;
  r.d.x = b.w;

  r.a.x = c.x;
  r.b.x = c.y;
  r.c.x = c.z;
  r.d.x = c.w;

  r.a.x = d.x;
  r.b.x = d.y;
  r.c.x = d.z;
  r.d.x = d.w;

  return r;
}

vec4 sm4::getColumn (const unsigned int i) {
  switch (i) {
  case 0:
    return vec4 {a.x,b.x,c.x,d.x};
  case 1:
    return vec4 {a.y,b.y,c.y,d.y};
  case 2:
    return vec4 {a.z,b.z,c.z,d.z};
  case 3:
    return vec4 {a.w,b.w,c.w,d.w};
  default:
    return getColumn (abs(i%4));
  }
}

void sm4::identity () {
  a = vec4 {1,0,0,0};
  b = vec4 {0,1,0,0};
  c = vec4 {0,0,1,0};
  d = vec4 {0,0,0,1};
}

std::string sm4::str () {
 std::stringstream ss;

  ss << "[";
  ss << a.str() << "," << std::endl;
  ss << b.str() << "," << std::endl;
  ss << c.str() << "," << std::endl;
  ss << d.str();
  ss << "]";

  return ss.str();
}

sm4 sm4::rotateX (double a) {
  a = a*M_PI/180;

  return sm4 {
    {1,0,0,0},
      {0,cos(a),-sin(a),0},
	{0,sin(a),cos(a),0},
	  {0,0,0,1}
  };
}

sm4 sm4::rotateY (double a) {
  a = a*M_PI/180;

  return sm4 {
    {cos(a),0,sin(a),0},
      {0,1,0,0},
	{-sin(a),0,cos(a),0},
	  {0,0,0,1}
  };
}

sm4 sm4::rotateZ (double a) {
  a = a*M_PI/180;

  return sm4 {
    {cos(a),-sin(a),0,0},
      {sin(a),cos(a),0,0},
	{0,0,1,0},
	  {0,0,0,1}
  };
}
