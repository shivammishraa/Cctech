#ifndef PLOT_UTILS_H
#define PLOT_UTILS_H

#include <string>
#include <vector>


void saveVerticesToFile(const std::vector<std::vector<double>>& vertices, const std::string& filename);
void plotWithGnuplot(const std::string &filename, const std::string &color = "blue", int linewidth = 2);

#endif
