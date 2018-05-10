#include "../stdafx.h"
#include "Project.h"

using namespace SimplyCpp::Core;

void Project::SetName(std::string name)
{
    m_name = name;
}

std::string Project::GetName()
{
    return m_name;
}

std::vector<SourceFile>& Project::GetFiles()
{
    return m_files;
}

void Project::LoadFile(std::string fileName)
{
    // TODO: Load the project from a file here
}

void Project::SaveFile(std::string fileName)
{
    // TODO: Save the project to a file here
}
