/************************************************************************************************************************/
/*	NAME: Michael Wai Kit Tran																							*/
/*	ORGN: Bachelor of Software Engineering, Media Design School															*/
/*	FILE: Main.h																										*/
/*  DATE: Aug 23rd, 2022																								*/
/************************************************************************************************************************/

#pragma once
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <set>

//Window
extern unsigned int e_uViewPortW, e_uViewPortH;
extern GLFWwindow* e_pMainWindow;

//Time
extern float e_fPreviousTimestep;
extern float e_fDeltatime;
extern float e_fMaxDeltatime;

//Input
extern std::set<void(*)(GLFWwindow*,int,int,int,int)> e_setKeyCallbackFunctions;
extern char e_charCodePoint;
extern bool e_bCodePointFound;

extern std::set<void(*)(GLFWwindow*, int, int, int)> e_setMouseCallbackFunctions;
extern glm::vec2 e_v2MousePosition;
extern glm::vec2 e_v2MouseNDCPosition;

void UpdateMousePosition();
void UpdateInputPressed();
void TextInput(GLFWwindow* _pWindow, unsigned int _iCodePoint);