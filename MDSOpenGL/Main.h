#pragma once
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

//Window
extern unsigned int e_uViewPortW, e_uViewPortH;
extern GLFWwindow* e_pMainWindow;

//Time
extern float e_fPreviousTimestep;
extern float e_fDeltatime;
extern float e_fMaxDeltatime;

//Input
extern int e_iKey;
extern int e_iKeyScanCode;
extern int e_iKeyAction;
extern int e_iKeyMods;
extern bool e_bKeyPressed;
extern char e_charCodePoint;
extern bool e_bCodePointFound;

extern glm::vec2 e_v2MousePosition;
extern int e_iMouseButton;
extern int e_iMouseAction;
extern int e_iMouseMods;
extern bool e_bMousePressed;

extern bool e_bTextInputEnabled;

void KeyFunction(GLFWwindow* _pWindow, int _iKey, int _iScanCode, int _iAction, int _iMods);
void UpdateMousePosition(GLFWwindow* _pWindow);
void MouseButtonFunction(GLFWwindow* _pWindow, int _iButton, int _iAction, int _iMods);
void UpdateInputPressed();
void TextInput(GLFWwindow* _pWindow, unsigned int _iCodePoint);