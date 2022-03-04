#include <fstream>
#include <iostream>

#include <string>
#include <filesystem>

constexpr int LEN = 50000;
char line[LEN];


inline void putseq(std::ifstream& file_in, std::ofstream& file, int len)
{
    file.write(line, len - 1);
    file.put('\n');
    if (!file_in.getline(line, LEN))
    {
        std::cerr << "Warning, end of file ! Len=" << len << '\n';
        char a;
        std::cin >> a;
        exit(-1);
    }
    len = file_in.gcount();

    if (1 < len && len < LEN)
    {
        file.write(line, file_in.gcount() - 1);
    }
    else
        std::cerr << "Warning, bad line ! Len=" << len << '\n';

    file.put('\n');
}

int main(int argc, char** argv)
{
    std::filesystem::path in_fasta;

    if (argc == 1 && argv && argv[1])
    {
        in_fasta = std::filesystem::path(argv[1]);
    }
    else
    {
        in_fasta = std::filesystem::path("D:/PMS/CoV/allnuc0209.fasta");
    }
    

    std::cout << "Imput fasta file: " << in_fasta << "\n";

    std::filesystem::path base_dir = in_fasta.parent_path();
    std::string name = in_fasta.stem().string();
    std::cout << "Imput fasta name: " << name << "\n";

    // Initialise a file input object with a FASTA file.
    std::ifstream file_in{ in_fasta };
    std::ofstream file_E{ name + ".E-vc.fasta"};
    std::ofstream file_N{ name + ".N-vc.fasta" };
    std::ofstream file_S{ name + ".S-vc.fasta" };

    long e{ 0L }, n{ 0L }, s{ 0L }, t{ 0L }, m{ (1L << 15) - 1 };
    std::cout << "m= " << m << "\n";

    while (file_in.getline(line, LEN))
    {
        int len = file_in.gcount();
        if (len < 3)
        {
            std::cerr << "Warning, blank line ! '" << std::string(line, len) << "'. Len = " << len << '\n';
            continue;
        }
        if (line[0] != '>')
        {
            std::cerr << "Warning, unknow line ! '" << std::string(line, len) << "'. Len = " << len << '\n';
            continue;
        }

        if (line[1] == 'E') 
        { 
            putseq(file_in, file_E, len);
            ++e; 
        }
        else if (line[2] == '|' and line[1] == 'N')
        { 
            putseq(file_in, file_N, len);
            ++n; 
        }
        else if (line[1] == 'S')
        { 
            putseq(file_in, file_N, len);
            ++s; 
        }
        if (!(++t & m)) std::cout << "T= " << t << "N= " << n << ", E= " << e << ", Spike= " << s << "\n";
    }
    std::cout << "T= " << t << ". N= " << n << ", E= " << e << ", Spike= " << s << "\n";

    char a;
    std::cin >> a;
    return 0;
}

