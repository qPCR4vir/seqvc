#include <fstream>
#include <iostream>

#include <string>
#include <filesystem>


int main()
{
    std::filesystem::path base_dir{ "D:/PMS/CoV" }; // get the directory
    std::cout << 1 << base_dir << "\n";

    // Initialise a file input object with a FASTA file.
    std::ifstream file_in{ base_dir / "allnuc0209.fasta" };
    std::ofstream file_E{ base_dir / "Evc.20220209.fasta" };
    std::ofstream file_N{ base_dir / "Nvc.20220209.fasta" };
    std::ofstream file_S{ base_dir / "Svc.20220209.fasta" };

    long e{ 0L }, n{ 0L }, s{ 0L }, t{ 0L }, m{ (1L << 15) - 1 };
    std::cout << "m= " << m << "\n";
    constexpr int LEN = 20000;
    char line[LEN];

    while (file_in.getline(line, LEN))
    {
        if (line[1] == 'E') 
        { 
            file_E.write(line, file_in.gcount()-1); 
            file_E.put('\n');
            file_in.getline(line, LEN);
            file_E.write(line, file_in.gcount()-1);
            file_E.put('\n');
            ++e; 
        }
        else if (line[2] == '|' and line[1] == 'N')
        { 
            file_N.write(line, file_in.gcount()-1);
            file_N.put('\n');
            file_in.getline(line, LEN);
            file_N.write(line, file_in.gcount()-1);
            file_N.put('\n');
            ++n; 
        }
        else if (line[1] == 'S')
        { 
            file_S.write(line, file_in.gcount()-1);
            file_S.put('\n');
            file_in.getline(line, LEN);
            file_S.write(line, file_in.gcount()-1);
            file_S.put('\n');
            ++s; 
        }
        if (!(++t & m)) std::cout << "T= " << t << "N= " << n << ", E= " << e << ", Spike= " << s << "\n";
    }
    std::cout << "T= " << t << ". N= " << n << ", E= " << e << ", Spike= " << s << "\n";

    return 0;
}

