/**
* @copybrief
* MIT License
* Copyright (c) 2022 NeilKleistGao
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

/// @file main.cc

#ifdef WIN32
#define DLL_PREFIX __declspec(dllexport)
#else
#define DLL_PREFIX
#endif

#include <cstdio>
#include <cstring>

#include "syntax/compiler.h"

extern "C" {
  /**
   * Compile dialogue files.
   * @param p_input: input filename
   * @param p_output: output filename
   * @param p_i18n_prefix: translation files' prefix
   * @return const char*, a string standing for errors(null if no error)
   */
  DLL_PREFIX bool Compile(const char* p_input, const char* p_output, const char* p_i18n_prefix, char* p_err) {
    FILE* fp = fopen(p_input, "r");
    if (fp == nullptr) {
      std::strcpy(p_err, "File doesn't exist.");
      return true;
    }

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char* buffer = new char[size];
    fread(buffer, sizeof(char), size, fp);
    fclose(fp);

    Compiler compiler{};
    const char* res = compiler.Compile(buffer, size, p_i18n_prefix);
    delete[] buffer; buffer = nullptr;
    if (res != nullptr) {
      std::strcpy(p_err, res);
      return true;
    }

    size = compiler.GetSize();
    buffer = new char[size];
    compiler.Write(buffer);
    fp = fopen(p_output, "wb");
    if (fp == nullptr) {
      std::strcpy(p_err, "Can't write the file.");
      return true;
    }

    fwrite(buffer, sizeof(char), size, fp);
    fclose(fp);
    delete[] buffer; buffer = nullptr;

    return false;
  }
}