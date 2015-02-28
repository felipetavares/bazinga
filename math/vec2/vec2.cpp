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

#include "vec2.h"
#include <cmath>

vec2::vec2 (double x, double y) {
  this->x = x;
  this->y = y;
}

vec2::vec2 (const vec2& vec) {
  x = vec.x;
  y = vec.y;
}

vec2::vec2 () {
  this->x = 0;
  this->y = 0;
}

vec2::~vec2 () {
  // Nothing to do here
}

double vec2::operator [] (const int i) {
  switch (i) {
  case 0:
    return x;
  case 1:
    return y;
  default:
    return (*this)[abs(i%2)]; // This allows a lot more flexibility in the input number
  }
}

vec2 vec2::operator = (const vec2& vec) {
  x = vec.x;
  y = vec.y;

  return *this;
}

vec2 vec2::operator * (const double s) {
  vec2 r;

  r.x = x*s;
  r.y = y*s;

  return r;
}

vec2 vec2::operator / (const double s) {
  vec2 r;

  r.x = x/s;
  r.y = y/s;

  return r;
}

vec2 vec2::operator + (const vec2& vec) {
  vec2 r;

  r.x = x + vec.x;
  r.y = y + vec.y;

  return r;
}

vec2 vec2::operator - (const vec2& vec) {
  vec2 r;

  r.x = x - vec.x;
  r.y = y - vec.y;

  return r;
}

double vec2::operator * (const vec2& vec) {
  double r;

  r = x*vec.y-y*vec.x;

  return r;
}

vec2 vec2::operator *= (const double s) {
  x *= s;
  y *= s;

  return *this;
}

vec2 vec2::operator /= (const double s) {
  x *= s;
  y *= s;

  return *this;
}

vec2 vec2::operator += (const vec2& vec) {
  x += vec.x;
  y += vec.y;

  return *this;
}

vec2 vec2::operator -= (const vec2& vec) {
  x -= vec.x;
  y -= vec.y;

  return *this;
}

double vec2::dot (const vec2& vec) {
  return x*vec.x + y*vec.y;
}

std::string vec2::str () {
  std::stringstream ss;

  ss << "(";
  ss << x;
  ss << ", ";
  ss << y;
  ss << ")";

  return ss.str();
}

double vec2::length () {
  return sqrt (x*x+y*y);
}

vec2 vec2::normal () {
  vec2 r;
  double l = length();
  
  r.x = x/l;
  r.y = y/l;

  return r;
}

vec2 vec2::normalize () {
  double l = length();

  x /= l;
  y /= l;

  return *this;
}
