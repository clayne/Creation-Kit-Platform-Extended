// Copyright В© 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

// https://stackoverflow.com/questions/638053/how-to-increment-visual-studio-build-number-using-c

#define STRINGIZE2(s) #s
#define STRINGIZE(s) STRINGIZE2(s)

#define VERSION_MAJOR             0
#define VERSION_MINOR             5
#define VERSION_REVISION          0
#define VERSION_BUILD             389

#define VER_FILE_VERSION		VERSION_MAJOR, VERSION_MINOR, VERSION_REVISION, VERSION_BUILD
#define VER_PRODUCT_VERSION		VERSION_MAJOR, VERSION_MINOR, VERSION_REVISION, 0

#define VER_FILE_VERSION_STR        STRINGIZE(VERSION_MAJOR)        \
"." STRINGIZE(VERSION_MINOR)    \
"." STRINGIZE(VERSION_REVISION) \
"." STRINGIZE(VERSION_BUILD)    

#define VER_PRODUCT_VERSION_STR        STRINGIZE(VERSION_MAJOR)        \
"." STRINGIZE(VERSION_MINOR)    \
"." STRINGIZE(VERSION_REVISION) \
".0"

