﻿// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Editor API/EditorUI.h"
#include "Editor API/FO4/TESFileF4.h"
#include "Editor API/FO4/BSResourceArchive2.h"
#include "BSArchiveManagerModdedF4.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			namespace BSResource
			{
				extern bool NoTextureLoad;
				extern uintptr_t pointer_Archive2_sub1;
				extern uintptr_t pointer_Archive2_sub2;
				extern uintptr_t pointer_Archive2_sub3;
			}
		}
	}

	namespace Patches
	{
		namespace Fallout4
		{
			using namespace CreationKitPlatformExtended::EditorAPI;

			Array<const TESFile*> g_SelectedFilesArray;
			uintptr_t pointer_BSArchiveManagerModded_sub = 0;
			bool IsLoaded;
			uint8_t supportedBA2Version = 8;

			static constexpr char szArchiveList[] = "Fallout4 - Voices1.ba2, Fallout4 - Voices2.ba2, Fallout4 - Meshes.ba2, Fallout4 - Animations.ba2, Fallout4 - Interface.ba2, Fallout4 - Misc.ba2, Fallout4 - Sounds.ba2";
			static constexpr char szArchiveList2[] = "Fallout4 - Voices.ba2, Fallout4 - Interface.ba2, Fallout4 - Meshes.ba2, Fallout4 - MeshesExtra.ba2, Fallout4 - Misc.ba2, Fallout4 - Sounds.ba2, Fallout4 - Materials.ba2";

			BSArchiveManagerModdedPatch::BSArchiveManagerModdedPatch() : Module(GlobalEnginePtr)
			{}

			bool BSArchiveManagerModdedPatch::HasOption() const
			{
				return false;
			}

			bool BSArchiveManagerModdedPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* BSArchiveManagerModdedPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* BSArchiveManagerModdedPatch::GetName() const
			{
				return "BSArchiveManager Modded";
			}

			bool BSArchiveManagerModdedPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> BSArchiveManagerModdedPatch::GetDependencies() const
			{
				return {};
			}

			bool BSArchiveManagerModdedPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST;
			}

			bool BSArchiveManagerModdedPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				auto verPatch = lpRelocationDatabaseItem->Version();

				if ((verPatch == 1) || (verPatch == 2))
				{
					EditorAPI::Fallout4::BSResource::pointer_Archive2_sub1 = _RELDATA_ADDR(6);
					EditorAPI::Fallout4::BSResource::pointer_Archive2_sub2 = _RELDATA_ADDR(0);
					EditorAPI::Fallout4::BSResource::pointer_Archive2_sub3 = 
						lpRelocator->DetourFunctionClass(_RELDATA_RAV(10), EditorAPI::Fallout4::BSResource::Archive2::HKLoadArchive);
					EditorAPI::Fallout4::BSResource::Archive2::Initialize();

					if (verPatch == 1)
						// Первая версия патча для 1.10.162.0
						lpRelocator->DetourCall(_RELDATA_RAV(1),
							(uintptr_t)&EditorAPI::Fallout4::BSResource::Archive2::HKLoadStreamArchive);
					else
						lpRelocator->DetourCall(_RELDATA_RAV(1),
							(uintptr_t)&EditorAPI::Fallout4::BSResource::Archive2::HKLoadStreamArchiveEx);

					lpRelocator->DetourCall(_RELDATA_RAV(2), (uintptr_t)&LoadTesFile);
					lpRelocator->DetourJump(_RELDATA_RAV(3), (uintptr_t)&LoadTesFileFinal);

					pointer_BSArchiveManagerModded_sub = _RELDATA_ADDR(4);

					ScopeRelocator text;

					// Пропуск загрузки архивов, что не имеют отношения к загружаемому моду, но является предыдущей работой
					lpRelocator->Patch(_RELDATA_RAV(5), { 0xC3 });

					// Исключение из списков архив шейдеров
					lpRelocator->PatchStringRef(_RELDATA_RAV(8), szArchiveList);
					lpRelocator->PatchStringRef(_RELDATA_RAV(9), szArchiveList2);

					// Так как разница между первой и 8 версией лишь, то что был удалён GNF формат для PlayStation.
					// То очевидно, 8 версии с GNF форматом просто не будет, то вполне безопасно, открывать любые версии архивы.
					if (verPatch == 1)
						// Первая версия патча для 1.10.162.0
						lpRelocator->Patch(_RELDATA_RAV(7), &supportedBA2Version, 1);

					return true;
				}

				return false;
			}

			bool BSArchiveManagerModdedPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			bool BSArchiveManagerModdedPatch::HasLoaded()
			{
				return IsLoaded;
			}

			void BSArchiveManagerModdedPatch::AttachBA2File(LPCSTR _filename)
			{
				if (EditorAPI::Fallout4::BSResource::Archive2::IsAvailableForLoad(_filename))
					goto attach_ba2;
				return;
			attach_ba2:
				EditorAPI::Fallout4::BSResource::Archive2::LoadArchive(_filename);
			}

			void BSArchiveManagerModdedPatch::LoadTesFile(const TESFile* load_file)
			{
				IsLoaded = false;
				// Sometimes duplicated
				if (std::find(g_SelectedFilesArray.begin(), g_SelectedFilesArray.end(), load_file) ==
					g_SelectedFilesArray.end()) 
				{
					if (load_file->IsActive())
						_CONSOLE("Load active file %s...", load_file->GetFileName().c_str());
					else if (load_file->IsMaster() || load_file->IsSmallMaster())
						_CONSOLE("Load master file %s...", load_file->GetFileName().c_str());
					else
						_CONSOLE("Load file %s...", load_file->GetFileName().c_str());

					g_SelectedFilesArray.push_back(load_file);
				}

				auto sname = load_file->GetFileName();
				sname.Copy(0, sname.FindLastOf('.'));

				AttachBA2File(*(sname + " - Main.ba2"));
				if (!EditorAPI::Fallout4::BSResource::NoTextureLoad) AttachBA2File(*(sname + " - Textures.ba2"));

				((void(__fastcall*)(const TESFile*))pointer_BSArchiveManagerModded_sub)(load_file);
			}

			void BSArchiveManagerModdedPatch::LoadTesFileFinal(HWND hWnd, UINT uMsg, LPARAM lParam, WPARAM wParam)
			{
				EditorUI::HKSendMessageA(hWnd, uMsg, lParam, wParam);
				g_SelectedFilesArray.clear();
				IsLoaded = true;
			}
		}
	}
}