/* Start Header****************************************************************/
/*!
\file HTColor.h
\author Primary Author : Lim Chu Yan, chuyan.lim
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains some code for our custom color struct.

\par Contribution : CY     - 100.00%

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#pragma once
struct HTColor final
{
	float red   = 1.0f;
	float green = 1.0f;
	float blue  = 1.0f;
	float alpha = 1.0f;

};

/*union HTColor final
{
  unsigned int c;    // packed representation
  struct Argb_t        // component-wise representation
  {
    unsigned char a;
    unsigned char r;  
    unsigned char g;
    unsigned char b;
  };
  Argb_t argb;
  float GetRed()
  {
    return argb.r / 255.0f;
  }
  float GetGreen()  
  {
    return argb.g / 255.0f;
  }
  float GetBlue()
  {
    return argb.b / 255.0f;
  }
  float GetAlpha()
  {
    return argb.a / 255.0f;
  }
  void SetRed(float r)
  {
    argb.r = r * 255.0f;
  }
  void SetGreen(float g)
  {
    argb.g = g * 255.0f;
  }
  void SetBlue(float b)
  {
    argb.b = b * 255.0f;
  }  
  void SetAlpha(float a)
  {
    argb.a = a * 255.0f;
  }
};*/