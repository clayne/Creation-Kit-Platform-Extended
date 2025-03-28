﻿// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "QuitHandlerPatch.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		QuitHandlerPatch::QuitHandlerPatch() : Module(GlobalEnginePtr)
		{}

		bool QuitHandlerPatch::HasOption() const
		{
			return false;
		}

		bool QuitHandlerPatch::HasCanRuntimeDisabled() const
		{
			return false;
		}

		const char* QuitHandlerPatch::GetOptionName() const
		{
			return nullptr;
		}

		const char* QuitHandlerPatch::GetName() const
		{
			return "Quit Handler";
		}

		bool QuitHandlerPatch::HasDependencies() const
		{
			return false;
		}

		Array<String> QuitHandlerPatch::GetDependencies() const
		{
			return {};
		}

		bool QuitHandlerPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
			const char* lpcstrPlatformRuntimeVersion) const
		{
			return true;
		}

		bool QuitHandlerPatch::Activate(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			auto versionPatch = lpRelocationDatabaseItem->Version();

			if (versionPatch == 1)
			{
				for (uint32_t nId = 0; nId < lpRelocationDatabaseItem->Count(); nId++)
					lpRelocator->DetourCall(_RELDATA_RAV(nId),
						(uintptr_t)&CreationKitPlatformExtended::Utils::Quit);

				return true;
			}
			else if (versionPatch == 2)
			{
				lpRelocator->DetourCall(_RELDATA_RAV(0), (uintptr_t)&CreationKitPlatformExtended::Utils::Quit);
				lpRelocator->DetourCall(_RELDATA_RAV(1), (uintptr_t)&CreationKitPlatformExtended::Utils::Quit);

				// Remove useless stuff destroy and free resources
				lpRelocator->PatchNop(_RELDATA_RAV(2), 0x3FC);
			}
			
			return false;
		}

		bool QuitHandlerPatch::Shutdown(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			return false;
		}
	}
}