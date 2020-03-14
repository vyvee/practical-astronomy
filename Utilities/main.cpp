#include <fstream>
#include <iostream>
#include <string>

#define DIRECTORY_VSOP87 (std::string(getenv("HOME")) + "/utils/VSOP87/")

#define FILENAME_IAU1980 \
  (std::string(getenv("HOME")) + "/utils/Nutation/nut_IAU1980.dat.txt")

#define FILENAME_IAU2000B        \
  (std::string(getenv("HOME")) + \
   "/utils/Nutation/Nutation_IAU_2000B_Raw_Data.txt")

bool process_vsop87() {
  std::cout << "Processing VSOP87 files..." << std::endl;

  static constexpr int kDegreeMax{5};
  static const std::string planet_names[]{
      "mercury", "venus",  "earth",  "mars",
      "jupiter", "saturn", "uranus", "neptune",
  };
  static constexpr char coord_index_char[]{'\0', 'l', 'b', 'r'};

  std::ofstream outfile("/tmp/vsop87_internal.dat");
  for (const std::string &planet_name : planet_names) {
    std::string filename{DIRECTORY_VSOP87};
    filename += "VSOP87D.";
    filename += planet_name.substr(0, 3);
    std::ifstream infile(filename);
    if (!infile) {
      std::cout << "ERROR opening " << filename << "!" << std::endl;
      return false;
    }

    int coord_index_curr{1};
    int coord_degree_curr{0};

    std::string line;
    while (true) {
      // Header record
      std::getline(infile, line);
      std::string str_name;
      int coord_index, coord_degree;
      int num_terms;
      if (infile.eof()) {
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
      while (true) {
        outfile << "static constexpr struct PeriodicTerm ";
        outfile << planet_name << "_" << coord_index_char[coord_index_curr]
                << coord_degree_curr << "[]{" << std::endl;
        int coord_seq_curr{(kDegreeMax + 1) * coord_index_curr +
                           coord_degree_curr};
        if (coord_seq_curr >= coord_seq) {
          break;
        }
        outfile << "};" << std::endl;
        outfile << "static constexpr int ";
        outfile << planet_name << "_" << coord_index_char[coord_index_curr]
                << coord_degree_curr << "_size{0};" << std::endl;
        if (coord_degree_curr < kDegreeMax) {
          coord_degree_curr++;
        } else {
          coord_index_curr++;
          coord_degree_curr = 0;
        }
      }

      // Term records
      for (int rec_no = 1; rec_no <= num_terms; rec_no++) {
        std::getline(infile, line);
        std::string str_a{line.substr(79, 18)};
        std::string str_b{line.substr(97, 14)};
        std::string str_c{line.substr(111, 20)};
        outfile << "    { " << str_a << ", " << str_b << ", " << str_c << " },"
                << std::endl;
      }
      outfile << "};" << std::endl;
      outfile << "static constexpr int ";
      outfile << planet_name << "_" << coord_index_char[coord_index_curr]
              << coord_degree_curr << "_size{" << num_terms << "};"
              << std::endl;

      if (coord_degree_curr < kDegreeMax) {
        coord_degree_curr++;
      } else {
        coord_index_curr++;
        coord_degree_curr = 0;
      }

      if (coord_index_curr > 3) {
        break;
      }
    }

    for (coord_index_curr = 1; coord_index_curr <= 3; coord_index_curr++) {
      outfile << "static constexpr struct PeriodicTermTableDegree ";
      outfile << planet_name << '_' << coord_index_char[coord_index_curr]
              << "_table_degrees[]{" << std::endl;
      for (coord_degree_curr = 0; coord_degree_curr <= kDegreeMax;
           coord_degree_curr++) {
        outfile << "    {" << planet_name << '_'
                << coord_index_char[coord_index_curr] << coord_degree_curr
                << ", " << planet_name << '_'
                << coord_index_char[coord_index_curr] << coord_degree_curr
                << "_size}," << std::endl;
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

bool process_iau1980() {
  std::cout << "Processing IAU1980 file..." << std::endl;

  std::ifstream infile(FILENAME_IAU1980);
  if (!infile) {
    std::cout << "ERROR opening " << FILENAME_IAU1980 << "!" << std::endl;
    return false;
  }
  std::ofstream outfile("/tmp/iau1980_internal.dat");

  std::string line;
  do {
    std::getline(infile, line);
  } while (line[2] != 'l');

  while (true) {
    std::getline(infile, line);
    if (infile.eof() || (line[0] == '\0')) {
      break;
    }
    int cols[] = {0, 3, 6, 9, 12, 15, 23, 33, 43, 53, 63};
    std::string cols_strs[10];
    for (int i = 0; i < 10; i++) {
      cols_strs[i] = line.substr(cols[i], cols[i + 1] - cols[i]);
    }
    outfile << "      { ";
    for (int i = 0; i < 5; i++) {
      outfile << cols_strs[i] << ", ";
    }
    for (int i = 6; i < 9; i++) {
      outfile << cols_strs[i] << "_arcsec, ";
    }
    outfile << cols_strs[9] << "_arcsec ";
    outfile << "}," << std::endl;
  }

  infile.close();
  outfile.close();

  return true;
}

bool process_iau2000b() {
  std::cout << "Processing IAU2000B file..." << std::endl;

  std::ifstream infile(FILENAME_IAU2000B);
  if (!infile) {
    std::cout << "ERROR opening " << FILENAME_IAU2000B << "!" << std::endl;
    return false;
  }
  std::ofstream outfile("/tmp/iau2000b_internal.dat");

  std::string line;
  do {
    std::getline(infile, line);
  } while (line[0] != 'i');

  while (true) {
    std::getline(infile, line);
    if (line[0] == '*') {
      break;
    }
    int cols[] = {2, 6, 10, 14, 18, 22, 35, 44, 51, 61, 68, 77};
    std::string cols_strs[11];
    for (int i = 0; i < 11; i++) {
      cols_strs[i] = line.substr(cols[i], cols[i + 1] - cols[i]);
    }
    outfile << "      { ";
    for (int i = 0; i < 5; i++) {
      outfile << cols_strs[i] << ", ";
    }
    for (int i = 5; i < 10; i++) {
      outfile << cols_strs[i] << "_arcsec, ";
    }
    outfile << cols_strs[10] << "_arcsec ";
    outfile << "}," << std::endl;
  }

  infile.close();
  outfile.close();

  return true;
}

int main(void) {
  process_vsop87();
  process_iau1980();
  process_iau2000b();
  return 0;
}
