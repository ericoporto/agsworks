//
// Created by erico on 5/18/2024.
//

#ifndef AGSWORKS_UTILS_H
#define AGSWORKS_UTILS_H


namespace AGSWorks
{
    std::string MergeSequences(std::string str, char c);
    void FixupPath(std::string &path);
    std::string ConcatPaths(std::string parent, std::string child);
    std::string GetCwd();
} // namespace AGSWorks


#endif //AGSWORKS_UTILS_H
