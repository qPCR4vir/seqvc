#include <fstream>
#include <iostream>

#include <string>
#include <filesystem>

constexpr int desc_LEN = 5000;
char desc[desc_LEN];

constexpr int LEN = 50000;
char seq[LEN];

inline void putseq(std::ifstream& file_in, std::ofstream& file, std::streamsize len)
{
    file.write(desc, len - 1);  // std::streamsize{ 1 }
    file.put('\n');

    if (!file_in.getline(seq, LEN))
    {
        std::cerr << "Warning, end of file ! Len=" << len << '\n';
        char a;
        system("pause");
        exit(-1);
    }
    std::streamsize s_len = file_in.gcount();

    if (1 < s_len && s_len < LEN)
    {
        file.write(seq, s_len - 1);
    }
    else
    {
        std::cerr << "Warning, bad seq ! Description: '" << std::string(desc, len) << "'. Len = " << len << '\n';
        std::cerr << "Seq Len=" << s_len << "Seq: " << std::string(seq, s_len) << '\n';
    }
    file.put('\n');
}

int main(int argc, char** argv)
{
    std::filesystem::path in_fasta;

    if (argc == 2 && argv && argv[1])
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

    while (file_in.getline(desc, desc_LEN))
    {
        std::streamsize len = file_in.gcount();
        if (len < 3)
        {
            std::cerr << "Warning, blank line ! '" << std::string(desc, len) << "'. Len = " << len << '\n';
            continue;
        }
        if (desc[0] != '>')
        {
            std::cerr << "Warning, unknow line ! '" << std::string(desc, len) << "'. Len = " << len << '\n';
            continue;
        }

        if (desc[1] == 'E') 
        { 
            putseq(file_in, file_E, len);
            ++e; 
        }
        else if (desc[2] == '|' and desc[1] == 'N')
        { 
            putseq(file_in, file_N, len);
            ++n; 
        }
        else if (desc[1] == 'S')
        { 
            putseq(file_in, file_S, len);
            ++s; 
        }
        else
        {
            if (!file_in.getline(seq, LEN))
            {
                std::cerr << "Warning, end of file ! Len=" << len << '\n';
                char a;
                system("pause");
                exit(-1);
            }
        }
        if (!(++t & m)) std::cout << "T= " << t << "N= " << n << ", E= " << e << ", Spike= " << s << "\n";
    }
    std::cout << "T= " << t << ". N= " << n << ", E= " << e << ", Spike= " << s << "\n";

    system("pause");
    return 0;
}

