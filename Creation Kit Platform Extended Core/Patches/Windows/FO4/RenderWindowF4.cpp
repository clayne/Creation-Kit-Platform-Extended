﻿// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Core/RegistratorWindow.h"
#include "RenderWindowF4.h"
#include "MainWindowF4.h"
#include "Editor API/FO4/TESF4.h"
#include "Editor API/FO4/BGSRenderWindow.h"
#include "UITheme/VarCommon.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			using namespace CreationKitPlatformExtended::EditorAPI::Fallout4;

			RenderWindow* GlobalRenderWindowPtr = nullptr;
			RenderWindow::Area rcSafeDrawArea;

			bool RenderWindow::HasOption() const
			{
				return false;
			}

			bool RenderWindow::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* RenderWindow::GetOptionName() const
			{
				return nullptr;
			}

			const char* RenderWindow::GetName() const
			{
				return "Render Window";
			}

			bool RenderWindow::HasDependencies() const
			{
				return false;
			}

			Array<String> RenderWindow::GetDependencies() const
			{
				return {};
			}

			bool RenderWindow::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_FALLOUT_C4_LAST;
			}

			bool RenderWindow::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					*(uintptr_t*)&_oldWndProc =
						voltek::detours_function_class_jump(lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(0)), (uintptr_t)&HKWndProc);
					_TempDrawArea = (Area*)lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(1));
					TESUnknown::Instance = (TESUnknown*)lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(2));
					BGSRenderWindow::Singleton = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(3));
					
					return true;
				}

				return false;
			}

			bool RenderWindow::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			RenderWindow::RenderWindow() : BaseWindow(), Classes::CUIBaseWindow()
			{
				Assert(!GlobalRenderWindowPtr);
				GlobalRenderWindowPtr = this;
			}

			LRESULT CALLBACK RenderWindow::HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
			{
				switch (Message)
				{
					case WM_INITDIALOG:
					{
						GlobalRegistratorWindowPtr->RegisterMajor(Hwnd, "RenderWindow");
						GlobalRenderWindowPtr->m_hWnd = Hwnd;
						return CallWindowProc(GlobalRenderWindowPtr->GetOldWndProc(), Hwnd, Message, wParam, lParam);
					}
					// Don't let us reduce the window too much
					case WM_GETMINMAXINFO:
					{
						if (lParam)
						{
							LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
							lpMMI->ptMinTrackSize.x = 96;	// 96 min tex size
							lpMMI->ptMinTrackSize.y = 96;
						}
						return S_OK;
					}
					case WM_ACTIVATE:
					{
						// Fix bug loss of window size

						if (LOWORD(wParam) == WA_INACTIVE)
							rcSafeDrawArea = *_TempDrawArea;
						else
							*_TempDrawArea = rcSafeDrawArea;
					}
					break;
				}

				return CallWindowProc(GlobalRenderWindowPtr->GetOldWndProc(), Hwnd, Message, wParam, lParam);
			}
		}
	}
}