/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   FuturaStdMedium_otf;
    const int            FuturaStdMedium_otfSize = 26892;

    extern const char*   MonoSynthIconLarge_png;
    const int            MonoSynthIconLarge_pngSize = 29576;

    extern const char*   MonoSynthIconSmall_png;
    const int            MonoSynthIconSmall_pngSize = 1273;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Number of elements in the namedResourceList array.
    const int namedResourceListSize = 3;

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes) throw();
}
