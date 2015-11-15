#pragma once
#include "../stdafx.h"
#include "SourceFile.h"

using namespace SimplyCpp::Core;

SourceFile::SourceFile(std::string filename)
{
    SetFileName(filename);
}

void SourceFile::SetFileName(std::string filename)
{
    m_filename = filename;
}

std::string SourceFile::GetFileName()
{
    return m_filename;
}

