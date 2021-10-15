#ifndef _ALGEBRA_VECTEUR_HPP_
#define _ALGEBRA_VECTEUR_HPP_
# include <vector>
# include <cstdlib>

namespace Algebra
{
    class Vecteur : public std::vector<double>
    {
    public:
        Vecteur() = default;
        Vecteur( size_t dim ) : std::vector<double>(dim)
        {}
        Vecteur( size_t dim, double val ) : std::vector<double>(dim,val)
        {}
        Vecteur( std::initializer_list<double>& l_init ) : std::vector<double>(l_init)
        {}
        Vecteur( const Vecteur& u ) = default;
        Vecteur( Vecteur&& u ) = default;
        ~Vecteur() = default;

        Vecteur& operator = ( const Vecteur& u ) = default;
        Vecteur& operator = ( Vecteur&& u      ) = default;
        
        double norm() const;
        bool operator <  ( const Vecteur& u ) const;
        bool operator >  ( const Vecteur& u ) const;
        bool operator <= ( const Vecteur& u ) const;
        bool operator >= ( const Vecteur& u ) const;
        bool operator == ( const Vecteur& u ) const;
        bool operator != ( const Vecteur& u ) const;
        
    };
}

#endif
