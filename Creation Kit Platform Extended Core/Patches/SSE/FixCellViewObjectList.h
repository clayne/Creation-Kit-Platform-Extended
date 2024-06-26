﻿// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "Core/Module.h"
#include "Core/Relocator.h"
#include "Core/RelocationDatabase.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			using namespace CreationKitPlatformExtended::Core;

			class FixCellViewObjectListPatch : public Module
			{
			public:
				FixCellViewObjectListPatch();

				virtual bool HasOption() const;
				virtual bool HasCanRuntimeDisabled() const;
				virtual const char* GetOptionName() const;
				virtual const char* GetName() const;
				virtual bool HasDependencies() const;
				virtual Array<String> GetDependencies() const;

				static void ListViewSelectItem(HWND ListViewHandle, int ItemIndex, bool KeepOtherSelections);
				static void ListViewFindAndSelectItem(HWND ListViewHandle, void* Parameter, bool KeepOtherSelections);
				static void ListViewDeselectItem(HWND ListViewHandle, void* Parameter);
				static void sub1(uint64_t a1, uint64_t a2, uint64_t a3);
			protected:
				virtual bool QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
					const char* lpcstrPlatformRuntimeVersion) const;
				virtual bool Activate(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
				virtual bool Shutdown(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
			private:
				FixCellViewObjectListPatch(const FixCellViewObjectListPatch&) = default;
				FixCellViewObjectListPatch& operator=(const FixCellViewObjectListPatch&) = default;
			};
		}
	}
}