#include "plot_utils.h"
#include <fstream>  
#include <vector>
#include <string>
#include <cstdlib>

void plotWithGnuplot(const std::string &filename, const std::string &color, int linewidth) {
    std::string command =
        "gnuplot -p -e \""
        "set terminal wxt; "
        "set xlabel 'X'; "
        "set ylabel 'Y'; "
        "set zlabel 'Z'; "
        "splot '" + filename + "' with linespoints linecolor '" + color +
        "' linewidth " + std::to_string(linewidth) + "\"";
    system(command.c_str());
}

void saveVerticesToFile(const std::vector<std::vector<double>>& vertices, const std::string& filename) {
    std::ofstream out(filename);
    for (const auto& point : vertices) {
        out << point[0] << " " << point[1] << " " << point[2] << "\n";
    }
    out.close();
}