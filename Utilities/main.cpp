#include <cassert>
#include <fstream>
#include <iostream>
#include <string>

#define DIRECTORY_VSOP87 (std::string(getenv("HOME")) + "/utils/VSOP87/")

#define FILENAME_IAU1980 \
  (std::string(getenv("HOME")) + "/utils/Nutation/nut_IAU1980.dat.txt")

#define FILENAME_IAU2000B        \
  (std::string(getenv("HOME")) + \
   "/utils/Nutation/Nutation_IAU_2000B_Raw_Data.txt")

#define DIRECTORY_ELPMPP02 \
  (std::string(getenv("HOME")) + "/utils/2_lunar_solutions/2_elpmpp02/")

bool process_vsop87() {
  std::cout << "Processing VSOP87 files..." << std::endl;

  static const std::string planet_names[]{
      // "earth",
      "mercury", "venus",  "earth",  "mars",
      "jupiter", "saturn", "uranus", "neptune",
  };
  static constexpr char variable_char[]{'l', 'b', 'r'};

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

    std::string line;
    int table_sizes[3][7] = {
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
    };

    while (!std::getline(infile, line).eof()) {
      // Header record
      int variable, degree;
      int table_size;
      variable = std::stoi(line.substr(41, 1)) - 1;
      degree = std::stoi(line.substr(59, 1));
      table_size = std::stoi(line.substr(60, 7));
      table_sizes[variable][degree] = table_size;

      // Term Records
      outfile << "static constexpr struct PeriodicTerm ";
      outfile << planet_name << "_" << variable_char[variable] << degree << "["
              << table_size << "]{" << std::endl;
      for (int rec_no = 1; rec_no <= table_size; rec_no++) {
        std::getline(infile, line);
        std::string str_a{line.substr(79, 18)};
        std::string str_b{line.substr(97, 14)};
        std::string str_c{line.substr(111, 20)};
        outfile << "    { " << str_a << ", " << str_b << ", " << str_c << " },"
                << std::endl;
      }
      outfile << "};" << std::endl;
    }

    for (int v = 0; v < 3; v++) {
      std::string prefix{planet_name + '_' + variable_char[v]};

      outfile << "static constexpr struct PeriodicTermTableDegree ";
      outfile << prefix << "_table_degrees[]{" << std::endl;
      int degree;
      for (degree = 0; table_sizes[v][degree] > 0; degree++) {
        outfile << "    {" << prefix << degree << ", " << table_sizes[v][degree]
                << "}," << std::endl;
      }
      outfile << "};" << std::endl;

      outfile << "static constexpr struct PeriodicTermTable ";
      outfile << prefix << "_table ";
      outfile << '{';
      outfile << ".degrees = " << prefix << "_table_degrees, ";
      outfile << ".size = " << degree << ", ";
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

bool process_elpmpp02() {
  std::cout << "Processing ELPMPP02 file..." << std::endl;

  std::ofstream outfile{"/tmp/elpmpp02_internal.dat"};

  for (int i = 1; i <= 3; i++) {
    std::string in_filename{DIRECTORY_ELPMPP02};
    in_filename += "ELP_MAIN.S" + std::to_string(i);
    std::ifstream infile{in_filename};
    if (!infile) {
      std::cout << "ERROR opening " << in_filename << "!" << std::endl;
      return false;
    }

    std::string line;

    std::getline(infile, line);
    if (infile.eof() || (line[0] == '\0')) {
      std::cout << "ERROR abnormally short file!" << std::endl;
      return false;
    }
    int num_terms;
    num_terms = std::stoi(line.substr(30, 5));

    std::string suffix{(i <= 2) ? "_arcsec" : ""};

    outfile << "static constexpr struct PeriodicTerm ";
    outfile << "elp_main_s" << i << "[" << num_terms << "]{" << std::endl;
    for (int k = 0; k < num_terms; k++) {
      std::getline(infile, line);
      if (infile.eof() || (line[0] == '\0')) {
        std::cout << "ERROR abnormally short file!" << std::endl;
        return false;
      }

      int cols[] = {0, 3, 6, 9, 12, 14, 27, 39, 51, 63, 75, 87, 99};
      std::string cols_strs[12];
      for (int i = 0; i < 11; i++) {
        cols_strs[i] = line.substr(cols[i], cols[i + 1] - cols[i]);
      }

      outfile << "  { ";
      for (int i = 0; i < 4; i++) {
        outfile << cols_strs[i] << ", ";
      }
      assert(cols_strs[4] == "  ");
      for (int i = 5; i < 11; i++) {
        outfile << cols_strs[i] << suffix << ", ";
      }
      outfile << cols_strs[11] << suffix << " ";
      outfile << "}," << std::endl;
    }
    outfile << "};" << std::endl;

    infile.close();
  }

  for (int i = 1; i <= 3; i++) {
    std::string in_filename{DIRECTORY_ELPMPP02};
    in_filename += "ELP_PERT.S" + std::to_string(i);
    std::ifstream infile{in_filename};
    if (!infile) {
      std::cout << "ERROR opening " << in_filename << "!" << std::endl;
      return false;
    }

    std::string line;

    outfile << "static constexpr struct PeriodicTerm ";
    outfile << "elp_pert_s" << i << "[4]{" << std::endl;
    for (int n = 0; n < 4; n++) {
      std::getline(infile, line);
      if (infile.eof() || (line[0] == '\0')) {
        std::cout << "ERROR abnormally short file!" << std::endl;
        return false;
      }
      int num_terms;
      num_terms = std::stoi(line.substr(30, 5));

      std::string suffix{(i <= 2) ? "_arcsec" : ""};

      outfile << "  {" << std::endl;
      for (int k = 0; k < num_terms; k++) {
        std::getline(infile, line);
        if (infile.eof() || (line[0] == '\0')) {
          std::cout << "ERROR abnormally short file!" << std::endl;
          return false;
        }

        int cols[] = {
            0,  5,  21, 22, 25, 41, 42, 45, 48, 51, 54, 57,
            60, 63, 66, 69, 72, 75, 78, 81, 84, 87, 90, 93,
        };
        std::string cols_strs[23];
        for (int i = 0; i < 23; i++) {
          cols_strs[i] = line.substr(cols[i], cols[i + 1] - cols[i]);
        }

        outfile << "    { ";
        outfile << cols_strs[1] << "E" << cols_strs[3] << ", ";
        outfile << cols_strs[4] << "E" << cols_strs[6] << ", ";
        for (int i = 7; i < 19; i++) {
          outfile << cols_strs[i] << ", ";
        }
        outfile << cols_strs[19] << " ";
        assert(cols_strs[20] == "  0");
        assert(cols_strs[21] == "  0");
        assert(cols_strs[22] == "  0");
        outfile << "}," << std::endl;
      }
      outfile << "  }," << std::endl;
    }
    outfile << "};" << std::endl;

    infile.close();
  }

  outfile.close();

  return true;
}

int main(void) {
  process_vsop87();
  process_iau1980();
  process_iau2000b();
  process_elpmpp02();
  return 0;
}
