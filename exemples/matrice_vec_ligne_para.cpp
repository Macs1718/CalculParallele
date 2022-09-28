// Produit matrice-vecteur
# include <cassert>
# include <vector>
# include <iostream>
# include <mpi.h>

// ---------------------------------------------------------------------
class Matrix : public std::vector<double>
{
public:
    Matrix (int dim);
    Matrix( int nrows, int ncols, int firstRow = 0 );
    Matrix( const Matrix& A ) = delete;
    Matrix( Matrix&& A ) = default;
    ~Matrix() = default;

    Matrix& operator = ( const Matrix& A ) = delete;
    Matrix& operator = ( Matrix&& A ) = default;
    
    double& operator () ( int i, int j ) {
        return m_arr_coefs[i + j*m_nrows];
    }
    double  operator () ( int i, int j ) const {
        return m_arr_coefs[i + j*m_nrows];
    }
    
    std::vector<double> operator * ( const std::vector<double>& u ) const;
    
    std::ostream& print( std::ostream& out ) const
    {
        const Matrix& A = *this;
        out << "[\n";
        for ( int i = 0; i < m_nrows; ++i ) {
            out << " [ ";
            for ( int j = 0; j < m_ncols; ++j ) {
                out << A(i,j) << " ";
            }
            out << " ]\n";
        }
        out << "]";
        return out;
    }
private:
    int m_nrows, m_ncols;
    std::vector<double> m_arr_coefs;
};
// ---------------------------------------------------------------------
inline std::ostream& 
operator << ( std::ostream& out, const Matrix& A )
{
    return A.print(out);
}
// ---------------------------------------------------------------------
inline std::ostream&
operator << ( std::ostream& out, const std::vector<double>& u )
{
    out << "[ ";
    for ( const auto& x : u )
        out << x << " ";
    out << " ]";
    return out;
}
// ---------------------------------------------------------------------
std::vector<double> 
Matrix::operator * ( const std::vector<double>& u ) const
{
    const Matrix& A = *this;
    assert( u.size() == unsigned(m_ncols) );
    std::vector<double> v(m_nrows, 0.);
    for ( int i = 0; i < m_nrows; ++i ) {
        for ( int j = 0; j < m_ncols; ++j ) {
            v[i] += A(i,j)*u[j];
        }            
    }
    return v;
}

// =====================================================================
Matrix::Matrix (int dim) : m_nrows(dim), m_ncols(dim),
                           m_arr_coefs(dim*dim)
{
    for ( int i = 0; i < dim; ++ i ) {
        for ( int j = 0; j < dim; ++j ) {
            (*this)(i,j) = (i+j)%dim;
        }
    }
}
// ---------------------------------------------------------------------
Matrix::Matrix( int nrows, int ncols, int firstRow ) : m_nrows(nrows), m_ncols(ncols),
                                         m_arr_coefs(nrows*ncols)
{
    int dim = (nrows > ncols ? nrows : ncols );
    for ( int i_loc = 0; i_loc < nrows; ++ i_loc ) {
        for ( int j = 0; j < ncols; ++j ) {
            int i_glob = i_loc + firstRow;
            (*this)(i_loc,j) = (i_glob+j)%dim;
        }
    }    
}
// =====================================================================
int main( int nargs, char* argv[] )
{
    char filename[256];
    int rank, nbp;
    MPI_Init(&nargs, &argv);
    MPI_Comm world;
    MPI_Comm_dup(MPI_COMM_WORLD, &world);

    MPI_Comm_rank(world, &rank);
    MPI_Comm_size(world, &nbp);

    sprintf(filename, "output%03d.txt", rank);
    FILE* fich = fopen(filename, "w");

    const int N = 12;

    int Nloc = N/nbp;
    int firstRow = rank*Nloc;

    fprintf(fich, "Nombre de lignes locales : %d, Premiere ligne : %d\n", Nloc, firstRow);
    Matrix A(Nloc, N, firstRow);
    std::cout  << "A : " << A << std::endl;
    std::vector<double> u( N );
    for ( int i = 0; i < N; ++i ) u[i] = i+1;
    std::cout << " u : " << u << std::endl;
    std::vector<double> v_loc = A*u;
    std::vector<double> v_glob(N);

    MPI_Allgather(v_loc.data(), Nloc, MPI_DOUBLE, 
                  v_glob.data(), Nloc, MPI_DOUBLE, world);

    std::cout << "A.u = " << v_glob << std::endl;

    fclose(fich);
    MPI_Finalize();
    return EXIT_SUCCESS;
}
