#pragma once
#include "includes.h"
#include <string>
#include <vector>
#include "Vec2.h"
#include <fstream>
static const float PI = 3.141592654f;
class Utils
{
public:
	
	class_scope void Tokenize(std::vector<std::string>* tokens, std::string s, std::string delimiter)
	{
		// check that is still valid
		while (s.find(delimiter, 0) != std::string::npos) {
			// get pos of next delimiter
			size_t pos = s.find(delimiter, 0);
			// create a token
			std::string token = s.substr(0, pos);
			// erase the current token 
			s.erase(0, pos + 1);
			// add the token to  vector
			tokens->push_back(token);
		}
		// push the last token
		tokens->push_back(s);
	}
	class_scope __inline float GetAngleBetweenPoints(Vec2f firstPoint, Vec2f secondPoint) {

		if ((secondPoint.x > firstPoint.x)) {

			return ((float)atan2((secondPoint.x - firstPoint.x), (firstPoint.y - secondPoint.y)) * 180.0f / PI);

		}
		else if ((secondPoint.x < firstPoint.x)) {

			return 360.0f - ((float)atan2((firstPoint.x - secondPoint.x), (firstPoint.y - secondPoint.y)) * 180.0f / PI);

		}

		return atan2(0.0f, 0.0f);

	}
	class_scope __inline float GetRadiansBetweenPoints(Vec2f firstPoint, Vec2f secondPoint)
	{
		return DegToRad(GetAngleBetweenPoints(firstPoint, secondPoint));
	}
	class_scope __inline float DegToRad(float angle) { return angle * PI / 180.0f; }
	class_scope __inline float RadToDeg(float rad) { return rad * 180.0f / PI; }
	class_scope __inline std::vector<std::string> LoadTextFile(std::string filename)
	{
		std::fstream txtfile(filename.c_str());
		assert(txtfile.good());
		std::vector<std::string> text;
		if (txtfile.good())
		{
			std::string str;
			while (std::getline(txtfile, str))
			{
				text.push_back(str);
			}
			txtfile.close();
		}
		return std::move(text);
	}
};