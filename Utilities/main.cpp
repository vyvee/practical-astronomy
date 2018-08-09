#include <fstream>
#include <iostream>
#include <string>

#define DIRECTORY_VSOP87 (std::string(getenv("HOME")) + "/utils/VSOP87/")

bool VSOP87ProcessDataFiles()
{
    static constexpr int kDegreeMax{5};
    static const std::string planet_names[]{
        "mercury", "venus",  "earth",  "mars",
        "jupiter", "saturn", "uranus", "neptune",
    };
    static constexpr char coord_index_char[]{'\0', 'l', 'b', 'r'};

    std::ofstream outfile("/tmp/vsop87_internal.dat");
    for(const std::string &planet_name : planet_names) {
        std::string filename{DIRECTORY_VSOP87};
        filename += "VSOP87D.";
        filename += planet_name.substr(0, 3);
        std::ifstream infile(filename);
        if(!infile) {
            std::cout << "ERROR opening " << filename << "!" << std::endl;
            return false;
        }

        int coord_index_curr{1};
        int coord_degree_curr{0};

        std::string line;
        while(true) {
            // Header record
            std::getline(infile, line);
            std::string str_name;
            int coord_index, coord_degree;
            int num_terms;
            if(infile.eof()) {
                coord_index = 3;
                coord_degree = kDegreeMax;
                num_terms = 0;
            } else {
                str_name = line.substr(22, 7);
                coord_index = std::stoi(line.substr(41, 1));
                coord_degree = std::stoi(line.substr(59, 1));
                num_terms = std::stoi(line.substr(60, 7));
            }

            int coord_seq{(kDegreeMax + 1) * coord_index + coord_degree};
            while(true) {
                outfile << "static constexpr struct PeriodicTerm ";
                outfile << planet_name << "_"
                        << coord_index_char[coord_index_curr]
                        << coord_degree_curr << "[]{" << std::endl;
                int coord_seq_curr{(kDegreeMax + 1) * coord_index_curr +
                                   coord_degree_curr};
                if(coord_seq_curr >= coord_seq) {
                    break;
                }
                outfile << "};" << std::endl;
                outfile << "static constexpr int ";
                outfile << planet_name << "_"
                        << coord_index_char[coord_index_curr]
                        << coord_degree_curr << "_size{0};" << std::endl;
                if(coord_degree_curr < kDegreeMax) {
                    coord_degree_curr++;
                } else {
                    coord_index_curr++;
                    coord_degree_curr = 0;
                }
            }

            // Term records
            for(int rec_no = 1; rec_no <= num_terms; rec_no++) {
                std::getline(infile, line);
                std::string str_a{line.substr(79, 18)};
                std::string str_b{line.substr(97, 14)};
                std::string str_c{line.substr(111, 20)};
                outfile << "    { " << str_a << ", " << str_b << ", " << str_c
                        << " }," << std::endl;
            }
            outfile << "};" << std::endl;
            outfile << "static constexpr int ";
            outfile << planet_name << "_" << coord_index_char[coord_index_curr]
                    << coord_degree_curr << "_size{" << num_terms << "};"
                    << std::endl;

            if(coord_degree_curr < kDegreeMax) {
                coord_degree_curr++;
            } else {
                coord_index_curr++;
                coord_degree_curr = 0;
            }

            if(coord_index_curr > 3) {
                break;
            }
        }

        for(coord_index_curr = 1; coord_index_curr <= 3; coord_index_curr++) {
            outfile << "static constexpr struct PeriodicTermTableDegree ";
            outfile << planet_name << '_' << coord_index_char[coord_index_curr]
                    << "_table_degrees[]{" << std::endl;
            for(coord_degree_curr = 0; coord_degree_curr <= kDegreeMax;
                coord_degree_curr++) {
                outfile << "    {" << planet_name << '_'
                        << coord_index_char[coord_index_curr]
                        << coord_degree_curr << ", " << planet_name << '_'
                        << coord_index_char[coord_index_curr]
                        << coord_degree_curr << "_size}," << std::endl;
            }
            outfile << "};" << std::endl;

            outfile << "static constexpr struct PeriodicTermTable ";
            outfile << planet_name << '_' << coord_index_char[coord_index_curr]
                    << "_table ";
            outfile << '{';
            outfile << ".degrees = " << planet_name << '_'
                    << coord_index_char[coord_index_curr] << "_table_degrees, ";
            outfile << ".size = " << (kDegreeMax + 1) << ", ";
            outfile << ".method = PeriodicTermTable::Method::kCos ";
            outfile << "};" << std::endl;
        }

        infile.close();
    }

    outfile.close();

    return true;
}

int main(void)
{
    std::cout << "Processing VSOP87 files...";
    if(VSOP87ProcessDataFiles()) {
        std::cout << " OK!" << std::endl;
    } else {
        std::cout << " ERROR!" << std::endl;
    }

    return 0;
}
