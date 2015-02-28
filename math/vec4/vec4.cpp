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

#include "vec4.h"
#include <cmath>

vec4::vec4 () {
  x = 0;
  y = 0;
  z = 0;
  w = 0;
}

vec4::vec4 (double x, double y, double z, double w) {
  this->x = x;
  this->y = y;
  this->z = z;
  this->w = w;
}

vec4::vec4 (const vec4& vec) {
  x = vec.x;
  y = vec.y;
  z = vec.z;
  w = vec.w;
}

vec4::vec4 (const vec2& vec, double z, double w) {
  x = vec.x;
  y = vec.y;

  this->z = z;
  this->w = w;
}

vec4::vec4 (const vec2& vec, const vec2& vec2) {
  x = vec.x;
  y = vec.y;

  z = vec2.x;
  w = vec2.y;
}

vec4::vec4 (const vec3& vec, double w) {
  x = vec.x;
  y = vec.y;
  z = vec.z;

  this->w = w;
}

vec4::~vec4 () {
  // Nothing to do here
}

vec2 vec4::toVec2 () {
  vec2 r (x,y);

  return r;
}

vec3 vec4::toVec3 () {
  vec3 r (x,y,z);

  return r;
}

double vec4::operator [] (const int i) {
  switch (i) {
  case 0:
    return x;
  case 1:
    return y;
  case 2:
    return z;
  case 3:
    return w;
  default:
    return (*this)[abs(i%4)]; // This allows a lot more flexibility in the input number
  }
}

std::string vec4::str () {
  std::stringstream ss;
  
  ss << "(";
  ss << x;
  ss << ", ";
  ss << y;
  ss << ", ";
  ss << z;
  ss << ", ";
  ss << w;
  ss << ")";

  return ss.str();
}

double vec4::dot (const vec4& vec) {
  return x*vec.x+y*vec.y+z*vec.z+w*vec.w;
}

vec4 vec4::operator = (const vec4& vec) {
  x = vec.x;
  y = vec.y;
  z = vec.z;
  w = vec.w;

  return *this;
}

vec4 vec4::operator * (const double s) {
  vec4 r;

  r.x = x*s;
  r.y = y*s;
  r.z = z*s;
  r.w = w*s;

  return r;
}

vec4 vec4::operator / (const double s) {
  vec4 r;

  r.x = x/s;
  r.y = y/s;
  r.z = z/s;
  r.w = w/s;

  return r;
}

vec4 vec4::operator + (const vec4& vec) {
  vec4 r;

  r.x = x+vec.x;
  r.y = y+vec.y;
  r.z = z+vec.z;
  r.w = w+vec.w;

  return r;
}

vec4 vec4::operator - (const vec4& vec) {
  vec4 r;

  r.x = x+vec.x;
  r.y = y+vec.y;
  r.z = z+vec.z;
  r.w = w+vec.w;

  return r;
}

vec4 vec4::operator *= (const double s) {
  x *= s;
  y *= s;
  z *= s;
  w *= s;

  return *this;
}

vec4 vec4::operator /= (const double s) {
  x /= s;
  y /= s;
  z /= s;
  w /= s;

  return *this;
}

vec4 vec4::operator += (const vec4& vec) {
  x += vec.x;
  y += vec.y;
  z += vec.z;
  w += vec.z;

  return *this;
}

vec4 vec4::operator -= (const vec4& vec) {
  x -= vec.x;
  y -= vec.y;
  z -= vec.z;
  w -= vec.z;

  return *this;
}

double vec4::length () {
  return sqrt (x*x+y*y+z*z+w*w);
}

vec4 vec4::normal () {
  vec4 r;
  double l = length();

  r.x = x/l;
  r.y = y/l;
  r.z = z/l;
  r.w = w/l;

  return r;
}

vec4 vec4::normalize () {
  double l = length();

  x /= l;
  y /= l;
  z /= l;
  w /= l;

  return *this;  
}
