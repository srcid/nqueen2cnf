#include <iostream>
#include <thread>
#include <fstream>
#include <vector>

using namespace std;

constexpr unsigned sqr(unsigned n)
{
    return n*n;
}

constexpr unsigned pos(unsigned n, unsigned row, unsigned col)
{
    return row * n + col - n;
}

void rows(ofstream &file, int nq)
{
    unsigned i = 1, j = 1, k;

    for( ; i <= nq; i++)
    {
        k = j;

        for( ; j <= i * nq; j++)
        {
            file << j << " ";
        }

        file << "0\n";

        for( ; k <= j; k++)
        {
            for(unsigned l = k + 1; l < j; l++)
            {
                file << "-" << k << " " << "-" << l << " 0\n";
            }
        }
    }
}

void columns(ofstream &file, int nq)
{
    unsigned i = 1, j = 1, k;

    for( ; i <= nq; i++)
    {
        k = j;
        unsigned j_init = j;

        for( ; j <= (sqr(nq)) - (nq - i) ; j += nq)
        {
            file << j << " ";
        }

        file << "0\n";

        for( ; k <= j; k += nq)
        {
            for(unsigned l = k + nq; l < j; l += nq)
            {
                file << "-" << k << " -" << l << " 0\n";
            }
        }

        j = j_init + 1;
    }
}

void diagonal(ofstream &file, unsigned nq)
{
    //Principal

    unsigned d, aux = 1, excep = (2 * nq - 2) / 2;

    for(d = 1; d <= excep; d++)
    {
        for(unsigned i = nq - d; i <= nq - d + (d - 1) * (nq + 1); i += nq + 1)
        {
            for(unsigned j = i + nq + 1; j <= nq - d + d * (nq + 1); j += nq + 1)
            {
                file << "-" << i << " -" << j <<  " 0\n";
            }
        }
    }

    for(d = excep + 1; d <= 2 * nq - 3; d++)
    {
        for(unsigned i = nq - d + aux * (nq + 1); i <= nq - d + (d - 1 - aux) * (nq + 1); i += nq + 1)
        {
            for(unsigned j = i + nq + 1; j <= nq - d + (d - aux) * (nq + 1); j += nq + 1)
            {
                file << "-" << i << " -" << j <<  " 0\n";
            }
        }

        aux++;
    }
}

void diagonal_inverse(ofstream &file, unsigned nq)
{
    // Inversa

    unsigned d,
             excep = (nq * 2 - 2) / 2;

    for(d = 1; d <= excep; d++)
    {
        for(unsigned i = d + 1; i <= d + 1  + (d - 1) * (nq - 1); i += nq - 1)
        {
            for(unsigned j = i + nq - 1; j <= d + 1 + d * (nq - 1); j += nq - 1)
            {
                file << "-" << i << " -" << j << " 0\n";
            }
        }
    }

    for(d = 2; d <= nq - 1; d++)
    {
        for (unsigned i = pos(nq,d,nq); i <= sqr(nq) - nq; i += nq - 1)
        {
            for(unsigned j = i + nq - 1; j < sqr(nq); j += nq - 1)
            {
                file << "-" << i << " -" << j << " 0\n";
            }
        }
    }
}


int main(int argc, char const * argv[])
{

    unsigned nq = (unsigned)atoi(argv[1]), nc = 0;

    //This part of code calculate the number of clausules
    nc = 2 * nq * (((sqr(nq) - nq) / 2) + 1) + 2 * ((sqr(nq) - nq) / 2);

    for(unsigned i = 1, aux = 1; i <= 4; i++, aux = 1)
    {
        while(nq - aux > 1)
        {
            nc += ((nq - aux) * (nq - aux) - (nq - aux)) / 2;
            aux++;
        }
    }
    //End

    ofstream output_file(argv[2]);

    if(output_file.is_open()) {
        output_file <<  "p cnf " + to_string(sqr(nq)) + " " + to_string(nc) + "\n";
        rows(output_file, nq);
        columns(output_file, nq);
        diagonal(output_file, nq);
        diagonal_inverse(output_file, nq);
    } else {
        cout << "Erro, unable to open or generate output file." << endl;
    }

    output_file.close();

    return 0;
}