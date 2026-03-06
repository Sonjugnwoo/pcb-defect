#pragma once
#include "pch.h"
//-------------------------------------------------------------
// SHBrowseForFolder 콜백 함수
// Windows 폴더 선택 대화상자에서 사용자 정의 동작을 위해 사용
// 주로 초기 폴더 설정, 선택 변경 이벤트 처리에 사용
//-------------------------------------------------------------
int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);