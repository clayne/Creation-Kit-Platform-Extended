﻿// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Version/resource_version2.h"
#include "Engine.h"
#include "ModuleManager.h" 
#include "RelocationDatabase.h"
#include "Relocator.h"

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		ModuleManager::ModuleManager()
		{
			auto Path = Utils::GetApplicationPath() + "CreationKitPlatformExtendedFilter.txt";
			if (Utils::FileExists(Path.c_str()))
			{
				auto Stream = _fsopen(Path.c_str(), "rt", _SH_DENYRD);
				Utils::ScopeFileStream fileStream(Stream);
				auto Line = std::make_unique<char[]>(260);
				auto LinePtr = Line.get();
				while (fgets(LinePtr, 260, Stream))
				{
					LinePtr[259] = 0;
					_filter.push_back(Utils::Trim(LinePtr));
				}
			}
		}

		ModuleManager::~ModuleManager()
		{
			Clear();
		}

		bool ModuleManager::Has(const char* name) const
		{
			for (auto It = _modules.begin(); It != _modules.end(); It++)
			{
				if (!_stricmp(It->first.c_str(), name))
					return true;
			}

			return false;
		}

		bool ModuleManager::Append(Module* lpModule)
		{
			if (!lpModule || !lpModule->GetName() || Has(lpModule->GetName())) 
				return false;

			_modules.insert(std::make_pair<String, SmartPointer<Module>>(lpModule->GetName(), lpModule));
			return true;
		}

		void ModuleManager::Append(std::initializer_list<Module*> modulesList)
		{
			for (auto It = modulesList.begin(); It != modulesList.end(); It++)
				Append(*It);
		}

		void ModuleManager::Remove(const char* name)
		{
			_modules.erase(name);
		}

		SmartPointer<Module> ModuleManager::GetByName(const char* name) const
		{
			for (auto It = _modules.begin(); It != _modules.end(); It++)
			{
				if (!_stricmp(It->first.c_str(), name))
					return It->second;
			}

			return nullptr;
		}

		void ModuleManager::Clear()
		{
			_modules.clear();
			_filter.clear();
		}

		void ModuleManager::QueryAll()
		{
			Array<Map<String, SmartPointer<Module>>::iterator> RejectedModules;

			for (auto It = _modules.begin(); It != _modules.end(); It++)
			{
				if (std::find(_filter.begin(), _filter.end(), It->first.c_str()) != _filter.end())
				{
					RejectedModules.push_back(It);

					_WARNING("The patch \"%s\" exists in list exclude. Skip.", It->first.c_str());
					continue;
				}

				if (!It->second->Query(GlobalEnginePtr->GetEditorVersion(), VER_FILE_VERSION_STR))
					RejectedModules.push_back(It);
			}

			// Удаляем отбракованные
			for (auto It = RejectedModules.begin(); It != RejectedModules.end(); It++)
				_modules.erase(*It);
		}

		void ModuleManager::EnableAll()
		{
			uint32_t count = 0;

			for (auto It = _modules.begin(); It != _modules.end(); It++)
			{
				auto Patch = GlobalRelocationDatabasePtr->GetByName(It->first.c_str());
				if (Patch.Empty())
				{
					_WARNING("No patch data was found in the database: \"%s\"", It->first.c_str());
					continue;
				}

				It->second->Enable(GlobalRelocatorPtr, *Patch);
				if (It->second->HasActive())
					count++;
			}

			_MESSAGE("Modules installed: %u from %llu", count, _modules.size());
		}
	}
}