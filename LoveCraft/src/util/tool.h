﻿#ifndef TOOL_H__
#define TOOL_H__

#include <string>

// TODO ne pas oublier de ne pas definir DEBUGMODE en release
#ifndef DEBUGMODE
#define DEBUGMODE
#endif

#ifdef DEBUGMODE
#	define CHECK_GL_ERROR() Tool::CheckGLError(__FILE__, __LINE__);
#else
#	define CHECK_GL_ERROR()
#endif

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
#define ToRadian(a) (a * PII / 180)
#define ToDegree(a) (a * 180 / PII)
#define ZERO_MEM(a) memset(a, 0, sizeof(a))
#define INVALID_OGL_VALUE 0xFFFFFFFF

class Tool
{
public:
	static void CheckTypes();
	static bool LoadTextFile(const std::string& filename, std::string& buffer);
	static void CheckGLError(const char* file, int line);
	static float RandomFloat();

	template <class T>
	static bool EqualWithEpsilon(const T& v1, const T& v2, T epsilon = T(0.0001))
	{
		return (fabs(v2 - v1) < epsilon);
	}

	template <class T>
	static bool InRangeWithEpsilon(const T& v, const T& vinf, const T& vsup, T epsilon = T(0.0001))
	{
		return (v >= vinf - epsilon && v <= vsup + epsilon);
	}
};


#endif // TOOL_H__
