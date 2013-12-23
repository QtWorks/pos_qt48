#include "itemtraits.h"


template<> int& Trait::Ref(void) { return my_data.ival; }
template<> double& Trait::Ref(void)  { return my_data.dval; }
template<> std::string& Trait::Ref(void)  { return my_data.sval; }

template<> const int Trait::Convert() const
{
    if(Mode == StringMode) return (is_numeric() ? std::stoi(my_data.sval) : 0);
    if(Mode == DoubleMode) return (int)my_data.dval;
    return my_data.ival;
}

template<> const double Trait::Convert() const { return std::stod(ToString()); }
template<> const std::string Trait::Convert() const { return ToString(); }
