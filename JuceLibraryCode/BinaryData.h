/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   OscNoiseWaveSymbol_svg;
    const int            OscNoiseWaveSymbol_svgSize = 638;

    extern const char*   MonoSynthIconLarge_png;
    const int            MonoSynthIconLarge_pngSize = 29576;

    extern const char*   MonoSynthIconSmall_png;
    const int            MonoSynthIconSmall_pngSize = 1273;

    extern const char*   HKNovaMedium_ttf;
    const int            HKNovaMedium_ttfSize = 79744;

    extern const char*   NowMedium_ttf;
    const int            NowMedium_ttfSize = 49004;

    extern const char*   HKSuperMedium_ttf;
    const int            HKSuperMedium_ttfSize = 78588;

    extern const char*   NowBlack_ttf;
    const int            NowBlack_ttfSize = 49224;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 7;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
