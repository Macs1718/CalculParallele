# include <cmath>
# include "Vecteur.hpp"

double
Algebra::Vecteur::norm() const
{
    double val = 0.;
    for ( const auto& x : *this )
    {
        val += x*x;
    }
    return std::sqrt(val);
}
// ---------------------------------------------------------------------
bool
Algebra::Vecteur::operator < ( const Vecteur& u ) const
{
    double nrm  = norm();
    double nrmu = u.norm();
    if ( nrm == nrmu ) {
        bool flag = false;
        for ( size_t i = 0; 
              (i < std::min(size(),u.size())) && (flag == false); ++i )
            if ( (*this)[i] < u[i] ) flag = true;
        if (flag == false ) flag = (size() < u.size());
        return flag;
    }
    return nrm < nrmu;
}
// ---------------------------------------------------------------------
bool
Algebra::Vecteur::operator <= ( const Vecteur& u ) const
{
    double nrm  = norm();
    double nrmu = u.norm();
    if ( nrm == nrmu ) {
        bool flag = false;
        for ( size_t i = 0; 
              (i < std::min(size(),u.size())) && (flag == false); ++i )
            if ( (*this)[i] < u[i] ) flag = true;
        if (flag == false ) flag = (size() <= u.size());
        return flag;
    }
    return nrm <= nrmu;
}
// ---------------------------------------------------------------------
bool
Algebra::Vecteur::operator > ( const Vecteur& u ) const
{
    double nrm  = norm();
    double nrmu = u.norm();
    if ( nrm == nrmu ) {
        bool flag = false;
        for ( size_t i = 0; 
              (i < std::min(size(),u.size())) && (flag == false); ++i )
            if ( (*this)[i] > u[i] ) flag = true;
        if (flag == false ) flag = (size() > u.size());
        return flag;
    }
    return nrm > nrmu;
}
// ---------------------------------------------------------------------
bool
Algebra::Vecteur::operator >= ( const Vecteur& u ) const
{
    double nrm  = norm();
    double nrmu = u.norm();
    if ( nrm == nrmu ) {
        bool flag = false;
        for ( size_t i = 0; 
              (i < std::min(size(),u.size())) && (flag == false); ++i )
            if ( (*this)[i] > u[i] ) flag = true;
        if (flag == false ) flag = (size() >= u.size());
        return flag;
    }
    return nrm >= nrmu;
}
// ---------------------------------------------------------------------
bool
Algebra::Vecteur::operator == ( const Vecteur& u ) const
{
    if ( size() != u.size() ) return false;
    bool flag = true;
    for ( size_t i = 0; (i < size()) && (flag == true); ++i ) {
        flag = ( (*this)[i] == u[i] );
    }
    return flag;
}
// ---------------------------------------------------------------------
// ---------------------------------------------------------------------
bool
Algebra::Vecteur::operator != ( const Vecteur& u ) const
{
    if ( size() != u.size() ) return true;
    bool flag = false;
    for ( size_t i = 0; (i < size()) && (flag == false); ++i ) {
        flag = ( (*this)[i] != u[i] );
    }
    return flag;
}
